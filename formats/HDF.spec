/*
    This format is considered public domain, without any patent, with
    the following restriction:
    Use of the format is permitted as is, but if you alter the format,
    you must change the header from HDF to something else, as to prevent
    format incompatiblities.
    
    No header is used in network transfer.
    File based serialization has the following header:
    char[3] "HDF"
*/

//Always big endian
//Special
0x00 Null
0x01 Undefined
0x02 Not A Number
0x03 Infinity
0x04 True
0x05 False
0x06 Date //YYYYYYYY YYYYYYYM MMMDDDDD (Bits / 3 bytes / Unix timestamp)
0x07 Time //HHHHHMMM MMMTTTTT (Bits / 2 bytes / Unix timestamp)
0x08 STime //HHHHHMMM MMMSSSS SSUUUUUU (Bits / 3 bytes / hour, minute, second, precision(div by 64) / Unix timestamp)
0x09 List //Integer type of length, followed by length, folowed by list data
0x0A KeyValue //Type of key followed by value of key, Type of value followed by value of value
0x0E Structure definition //First byte ID, second byte length, followed by types
0x0F Use structure //First byte ID, then data

//Integers
0x10 int8_t
0x11 uint8_t
0x12 int16_t
0x13 uint16_t
0x14 int32_t
0x15 uint32_t
0x16 int64_t
0x17 uint64_t
0x18 int128_t
0x19 uint128_t
0x1A varint

//Floats
0x20 Half-Float (2-bytes)
0x21 Float (4-bytes)
0x22 Double (8-bytes)
0x23 Long Double (16-bytes)

//Strings
0x30 Empty string
0x31 Null terminated string
0x32 Tiny string:
    uint8_t length (0x11)
    char[length]
0x33 String:
    uint16_t length (0x13)
    char[length]
0x34 Long String:
    uint32_t length (0x15)
    char[length]
0x35 Huge String:
    uint64_t length (0x16)
    char[length]
0x38 Null terminated UTF-8 string
0x39 Tiny UTF-8 string:
    uint8_t length (0x11)
    char[length]
0x3A UTF-8 String:
    uint16_t length (0x13)
    char[length]
0x3B Long UTF-8 String:
    uint32_t length (0x15)
    char[length]
0x3C Huge UTF-8 String:
    uint64_t length (0x16)
    char[length]
    
//Vectors(And Quaternion)
0x40 Empty vector(All values are zero)
0x41 Mini integer vector (Group of 3 int8_ts)
0x42 Half integer vector (Group of 3 int16_ts)
0x43 Integer vector (Group of 3 int32_ts)
0x44 Double integer vector (Group of 3 int64_ts)
0x45 Huge integer vector (Group of 3 int128s_ts)
0x46 Half vector (Group of 3 Half-Floats)
0x47 Vector (Group of 3 Floats)
0x48 Double vector (Group of 3 Doubles)
0x49 Huge vector (Group of 3 Long doubles)
0x4A Half Quaternion (Group of 4 Half-Floats)
0x4B Quaternion (Group of 4 Floats)
0x4C Double Quaternion (Group of 4 Doubles)
0x4D Huge Quaternion (Group of 4 Long doubles)
