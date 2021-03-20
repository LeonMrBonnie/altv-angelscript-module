#include "../../helpers/module.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/include/cryptlib.h"
#include "cryptopp/include/hex.h"
#include "cryptopp/include/md5.h"
#include "cryptopp/include/sha.h"
#include "cryptopp/include/whrlpool.h"

using namespace Helpers;

enum class Algorithm : uint8_t
{
    SHA256,
    SHA512,
    MD5,
    Whirlpool
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

static std::string GetNamedHash(Algorithm algorithm, const std::string& input)
{
    using namespace CryptoPP;
    switch(algorithm)
    {
        case ::Algorithm::SHA256: return GetHash<SHA256>(input);
        case ::Algorithm::SHA512: return GetHash<SHA512>(input);
        case ::Algorithm::MD5: return GetHash<Weak::MD5>(input);
        case ::Algorithm::Whirlpool: return GetHash<Whirlpool>(input);
        default: 
        {
            THROW_ERROR("Invalid algorithm specified");
            return "";
        }
    }
}

static ModuleExtension cryptoExtension("crypto", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_ENUM("Algorithm", "An enum with all the available crypto algorithms");
    REGISTER_ENUM_VALUE("Algorithm", "SHA256", Algorithm::SHA256);
    REGISTER_ENUM_VALUE("Algorithm", "SHA512", Algorithm::SHA512);
    REGISTER_ENUM_VALUE("Algorithm", "MD5", Algorithm::MD5);
    REGISTER_ENUM_VALUE("Algorithm", "Whirlpool", Algorithm::Whirlpool);

    REGISTER_GLOBAL_FUNC("string Hash(crypto::Algorithm algorithm, const string&in input)", GetNamedHash, "Hashes a string using the specified algorithm");
});
