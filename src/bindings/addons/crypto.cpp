#include "../../helpers/module.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/include/cryptlib.h"
#include "cryptopp/include/hex.h"
#include "cryptopp/include/md5.h"
#include "cryptopp/include/sha.h"
#include "cryptopp/include/whrlpool.h"

using namespace Helpers;

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

static ModuleExtension cryptoExtension("crypto", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_GLOBAL_FUNC("string SHA256Hash(string&in input)", GetHash<CryptoPP::SHA256>, "Hashes a string using the SHA-256 algorithm");
    REGISTER_GLOBAL_FUNC("string SHA512Hash(string&in input)", GetHash<CryptoPP::SHA512>, "Hashes a string using the SHA-512 algorithm");
    REGISTER_GLOBAL_FUNC("string MD5Hash(string&in input)", GetHash<CryptoPP::Weak::MD5>, "Hashes a string using the MD5 algorithm");
    REGISTER_GLOBAL_FUNC("string WhirlpoolHash(string&in input)", GetHash<CryptoPP::Whirlpool>, "Hashes a string using the Whirlpool algorithm");
});
