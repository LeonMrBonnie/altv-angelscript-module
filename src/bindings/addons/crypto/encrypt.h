#pragma once
#include "helpers/angelscript.h"
#include "helpers/convert.h"

#include "cryptopp/include/hex.h"
#include "cryptopp/include/filters.h"
#include "cryptopp/include/sha.h"
#include "cryptopp/include/pwdbased.h"
#include "cryptopp/include/hkdf.h"
#include "cryptopp/include/scrypt.h"

namespace Crypto 
{
    enum class EncryptAlgorithm : uint8_t
    {
        PBKDF1,
        PBKDF2,
        HKDF,
        SCRYPT
    };

    template<class Encryption>
    static std::string GetEncrypted(const std::string& input, const std::string& salt, uint32_t rounds = 1024)
    {
        CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
        Encryption encryption;

        // todo: maybe not use this ugly hack here
        if constexpr(std::is_same_v<Encryption, CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256>> || std::is_same_v<Encryption, CryptoPP::PKCS5_PBKDF1<CryptoPP::SHA256>>) 
            encryption.DeriveKey(digest, sizeof(digest), 0, (CryptoPP::byte*)input.c_str(), input.length(), (CryptoPP::byte*)salt.c_str(), salt.length(), rounds, 0.0f);
        else if constexpr(std::is_same_v<Encryption, CryptoPP::HKDF<CryptoPP::SHA256>>)
        {
            std::string info = std::to_string(rounds);
            encryption.DeriveKey(digest, sizeof(digest), (CryptoPP::byte*)input.c_str(), input.length(), (CryptoPP::byte*)salt.c_str(), salt.length(), (CryptoPP::byte*)info.c_str(), info.length());
        }
        else if constexpr(std::is_same_v<Encryption, CryptoPP::Scrypt>)
            encryption.DeriveKey(digest, sizeof(digest), (CryptoPP::byte*)input.c_str(), input.length(), (CryptoPP::byte*)salt.c_str(), salt.length(), rounds);

        CryptoPP::HexEncoder encoder;
        std::string output;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();

        return output + "$" + salt + "$" + std::to_string(rounds);
    }

    #define ENCRYPTION_ALGORITHM_CASE(enumVal, algorithm) case Crypto::EncryptAlgorithm::enumVal: return GetEncrypted<algorithm>(input, salt, rounds)
    static std::string GetNamedEncryption(EncryptAlgorithm algorithm, const std::string& input, const std::string& salt, uint32_t rounds = 1024)
    {
        using namespace CryptoPP;
        switch(algorithm)
        {
            ENCRYPTION_ALGORITHM_CASE(PBKDF1, PKCS5_PBKDF1<SHA256>);
            ENCRYPTION_ALGORITHM_CASE(PBKDF2, PKCS5_PBKDF2_HMAC<SHA256>);
            ENCRYPTION_ALGORITHM_CASE(HKDF, HKDF<SHA256>);
            ENCRYPTION_ALGORITHM_CASE(SCRYPT, Scrypt);
            default: 
            {
                THROW_ERROR("Invalid algorithm specified");
                return "";
            }
        }
    }

    static bool VerifyEncryptedString(EncryptAlgorithm algorithm, const std::string& encrypted, const std::string& input)
    {
        std::vector<std::string> parts = SplitString(encrypted, "$");
        std::string hash = parts[0];
        std::string salt = parts[1];
        uint32_t rounds = std::stoul(parts[2]);

        auto result = GetNamedEncryption(algorithm, input, salt, rounds);
        return result == encrypted;
    }
}
