#pragma once
#include "helpers/angelscript.h"

// CryptoPP doesn't work on Linux so no Crypto on Linux
// todo: fix this lol
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

    #define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
    #include "cryptopp/include/cryptlib.h"
    #include "cryptopp/include/hex.h"
    #include "cryptopp/include/md5.h"
    #include "cryptopp/include/sha.h"
    #include "cryptopp/include/whrlpool.h"

namespace Crypto
{
    enum class HashAlgorithm : uint8_t
    {
        SHA256,
        SHA512,
        MD5,
        Whirlpool
    };

    template<class T>
    static std::string GetHash(const std::string& input)
    {
        T              hash;
        CryptoPP::byte digest[T::DIGESTSIZE];
        hash.CalculateDigest(digest, (CryptoPP::byte*)input.c_str(), input.length());

        CryptoPP::HexEncoder encoder;
        std::string          output;
        encoder.Attach(new CryptoPP::StringSink(output));
        encoder.Put(digest, sizeof(digest));
        encoder.MessageEnd();

        return output;
    }

    #define HASH_ALGORITHM_CASE(algorithm) \
        case Crypto::HashAlgorithm::algorithm: return GetHash<algorithm>(input)
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
}  // namespace Crypto
#endif
