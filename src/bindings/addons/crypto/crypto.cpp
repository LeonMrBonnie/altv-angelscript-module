#include "helpers/module.h"
#include "./encrypt.h"
#include "./hash.h"

using namespace Helpers;

static ModuleExtension cryptoExtension("crypto", [](asIScriptEngine* engine, DocsGenerator* docs) {
    // CryptoPP doesn't work on Linux so no Crypto on Linux
    // todo: fix this lol
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    using namespace Crypto;

    REGISTER_ENUM("HashAlgorithm", "An enum with all the available hash algorithms");
    REGISTER_ENUM_VALUE("HashAlgorithm", "SHA256", HashAlgorithm::SHA256);
    REGISTER_ENUM_VALUE("HashAlgorithm", "SHA512", HashAlgorithm::SHA512);
    REGISTER_ENUM_VALUE("HashAlgorithm", "MD5", HashAlgorithm::MD5);
    REGISTER_ENUM_VALUE("HashAlgorithm", "Whirlpool", HashAlgorithm::Whirlpool);

    REGISTER_GLOBAL_FUNC("string Hash(crypto::HashAlgorithm algorithm, const string&in input)", GetNamedHash, "Hashes a string using the specified hash algorithm");

    REGISTER_ENUM("EncryptAlgorithm", "An enum with all the available encryption algorithms");
    REGISTER_ENUM_VALUE("EncryptAlgorithm", "PBKDF1", EncryptAlgorithm::PBKDF1);
    REGISTER_ENUM_VALUE("EncryptAlgorithm", "PBKDF2", EncryptAlgorithm::PBKDF2);
    REGISTER_ENUM_VALUE("EncryptAlgorithm", "HKDF", EncryptAlgorithm::HKDF);
    REGISTER_ENUM_VALUE("EncryptAlgorithm", "SCRYPT", EncryptAlgorithm::SCRYPT);

    REGISTER_GLOBAL_FUNC(
        "string Encrypt(crypto::EncryptAlgorithm encryptAlgorithm, const string&in input, const string&in salt, uint rounds = 1024)", 
        GetNamedEncryption, "Encrypts a string using the specified encryption algorithm");
    REGISTER_GLOBAL_FUNC("bool Verify(crypto::EncryptAlgorithm encryptAlgorithm, const string&in encryptedInput, const string&in input)", 
        VerifyEncryptedString, "Verifies if the given input and the given encrypted input are the same");

    REGISTER_GLOBAL_FUNC("string GenerateRandomString(uint length)", GenerateRandomString, "Generates a random character sequence");
    #endif
});
