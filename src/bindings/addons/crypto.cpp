#include <random>
#include "../../helpers/module.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/include/cryptlib.h"
#include "cryptopp/include/hex.h"
#include "cryptopp/include/md5.h"
#include "cryptopp/include/sha.h"
#include "cryptopp/include/whrlpool.h"
#include "cryptopp/include/pwdbased.h"

using namespace Helpers;

enum class HashAlgorithm : uint8_t
{
    SHA256,
    SHA512,
    MD5,
    Whirlpool
};

enum class EncryptAlgorithm : uint8_t
{
    PBKDF2
};

template<class T>
static std::string GetHash(const std::string& input)
{
    T hash;
    CryptoPP::byte digest[T::DIGESTSIZE];
    hash.CalculateDigest(digest, (CryptoPP::byte*)input.c_str(), input.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return output;
}

template<class Encryption>
static std::string GetEncrypted(const std::string& input, const std::string& salt, uint32_t rounds = 1024)
{
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    Encryption encryption;
    encryption.DeriveKey(digest, sizeof(digest), 0, (CryptoPP::byte*)input.c_str(), input.length(), (CryptoPP::byte*)salt.c_str(), salt.length(), rounds, 0.0f);

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return output + "$" + salt + "$" + std::to_string(rounds);
}

#define HASH_ALGORITHM_CASE(algorithm) case ::HashAlgorithm::algorithm: return GetHash<algorithm>(input)
static std::string GetNamedHash(HashAlgorithm algorithm, const std::string& input)
{
    using namespace CryptoPP;
    using namespace CryptoPP::Weak;
    switch(algorithm)
    {
        HASH_ALGORITHM_CASE(SHA256);
        HASH_ALGORITHM_CASE(SHA512);
        HASH_ALGORITHM_CASE(Whirlpool);
        HASH_ALGORITHM_CASE(MD5);
        default: 
        {
            THROW_ERROR("Invalid algorithm specified");
            return "";
        }
    }
}

#define ENCRYPTION_ALGORITHM_CASE(enumVal, algorithm) case ::EncryptAlgorithm::enumVal: return GetEncrypted<algorithm<SHA256>>(input, salt, rounds)
static std::string GetNamedEncryption(EncryptAlgorithm algorithm, const std::string& input, const std::string& salt, uint32_t rounds = 1024)
{
    using namespace CryptoPP;
    switch(algorithm)
    {
        ENCRYPTION_ALGORITHM_CASE(PBKDF2, PKCS5_PBKDF2_HMAC);
        default: 
        {
            THROW_ERROR("Invalid algorithm specified");
            return "";
        }
    }
}

static std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter)
{
    std::vector<std::string> parts;
    size_t last = 0, next = 0;
    while((next = input.find(delimiter, last)) != std::string::npos) 
    {
        parts.push_back(input.substr(last, next-last));
        last = next + 1; 
    } 
    parts.push_back(input.substr(last));
    return parts;
}

static bool VerifyEncryptedString(EncryptAlgorithm algorithm, const std::string& encrypted, const std::string& input)
{
    std::vector<std::string> parts = SplitString(encrypted, "$");
    std::string hash = parts[0];
    std::string salt = parts[1];
    uint32_t rounds = std::stoul(parts[2]);

    auto result = GetNamedEncryption(algorithm, input, salt, rounds);
    return result == hash;
}

static std::string GenerateRandomString(uint32_t size)
{
    std::string availableChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<size_t> distribution(0, availableChars.size() - 1);

    std::string random_string;
    for (std::size_t i = 0; i < size; ++i)
    {
        random_string += availableChars[distribution(generator)];
    }

    return random_string;
}

static ModuleExtension cryptoExtension("crypto", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_ENUM("HashAlgorithm", "An enum with all the available hash algorithms");
    REGISTER_ENUM_VALUE("HashAlgorithm", "SHA256", HashAlgorithm::SHA256);
    REGISTER_ENUM_VALUE("HashAlgorithm", "SHA512", HashAlgorithm::SHA512);
    REGISTER_ENUM_VALUE("HashAlgorithm", "MD5", HashAlgorithm::MD5);
    REGISTER_ENUM_VALUE("HashAlgorithm", "Whirlpool", HashAlgorithm::Whirlpool);

    REGISTER_GLOBAL_FUNC("string Hash(crypto::HashAlgorithm algorithm, const string&in input)", GetNamedHash, "Hashes a string using the specified hash algorithm");

    REGISTER_ENUM("EncryptAlgorithm", "An enum with all the available encryption algorithms");
    REGISTER_ENUM_VALUE("EncryptAlgorithm", "PBKDF2", EncryptAlgorithm::PBKDF2);

    REGISTER_GLOBAL_FUNC(
        "string Encrypt(crypto::EncryptAlgorithm encryptAlgorithm, const string&in input, const string&in salt, uint rounds = 1024", 
        GetNamedEncryption, "Encrypts a string using the specified encryption algorithm");
    REGISTER_GLOBAL_FUNC("bool Verify(crypto::EncryptAlgorithm encryptAlgorithm, const string&in encryptedInput, const string&in input)", 
        VerifyEncryptedString, "Verifies if the given input and the given encrypted input are the same");

    REGISTER_GLOBAL_FUNC("string GenerateRandomString(uint length)", GenerateRandomString, "Generates a random character sequence");
});
