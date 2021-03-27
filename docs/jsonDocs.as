// Generated on Sat Mar 27 22:01:42 2021

namespace json
{
    // ********** Functions **********

    // Parses a JSON string to a JSON value
    JSON Parse(const string&in jsonStr);

    // ********** Objects **********

    // A JSON class for storing a JSON value
    class JSON
    {

        JSON();
        JSON(const string&in value);
        JSON(int64 value);
        JSON(uint64 value);
        JSON(double value);
        JSON(bool value);
        JSON(dictionary@ value);
        JSON(array<int8>@ value);
        JSON(array<int16>@ value);
        JSON(array<int>@ value);
        JSON(array<int64>@ value);
        JSON(array<uint8>@ value);
        JSON(array<uint16>@ value);
        JSON(array<uint>@ value);
        JSON(array<uint64>@ value);
        JSON(array<bool>@ value);
        JSON(array<double>@ value);
        JSON(array<float>@ value);
        JSON(array<string>@ value);
        JSON(array<JSON>@ value);
        string ToString(int indent = 0);
        string opImplConv() const;
    };
}
