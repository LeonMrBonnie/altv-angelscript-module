// Generated on Sun Mar 21 23:45:28 2021

namespace crypto
{
    // ********** Enums **********

    // An enum with all the available crypto algorithms
    enum Algorithm
    {
        SHA256 = 0,
        SHA512 = 1,
        MD5 = 2,
        Whirlpool = 3,
    };

    // ********** Functions **********

    // Hashes a string using the specified algorithm
    string Hash(crypto::Algorithm algorithm, const string&in input);
}
