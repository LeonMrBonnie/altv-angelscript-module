// Generated on Thu Mar 25 15:14:10 2021

namespace crypto
{
    // ********** Enums **********

    // An enum with all the available hash algorithms
    enum HashAlgorithm
    {
        SHA256 = 0,
        SHA512 = 1,
        MD5 = 2,
        Whirlpool = 3,
    };

    // An enum with all the available encryption algorithms
    enum EncryptAlgorithm
    {
        PBKDF1 = 0,
        PBKDF2 = 1,
        HKDF = 2,
        SCRYPT = 3,
    };

    // ********** Functions **********

    // Hashes a string using the specified hash algorithm
    string Hash(crypto::HashAlgorithm algorithm, const string&in input);

    // Encrypts a string using the specified encryption algorithm
    string Encrypt(crypto::EncryptAlgorithm encryptAlgorithm, const string&in input, const string&in salt, uint rounds = 1024);

    // Verifies if the given input and the given encrypted input are the same
    bool Verify(crypto::EncryptAlgorithm encryptAlgorithm, const string&in encryptedInput, const string&in input);

    // Generates a random character sequence
    string GenerateRandomString(uint length);

}
