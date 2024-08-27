#pragma once

class PKRUlib
{
public:
    enum Key
    {
        Key_0 = 0b11,
        Key_1 = 0b1100,
        Key_2 = 0b110000,
        Key_3 = 0b11000000,
        Key_4 = 0b1100000000,
        Key_5 = 0b110000000000,
        Key_6 = 0b11000000000000,
        Key_7 = 0b1100000000000000,
        Key_8 = 0b110000000000000000,
        Key_9 = 0b11000000000000000000,
        Key_10 = 0b1100000000000000000000,
        Key_11 = 0b110000000000000000000000,
        Key_12 = 0b11000000000000000000000000,
        Key_13 = 0b1100000000000000000000000000,
        Key_14 = 0b110000000000000000000000000000,
        Key_15 = 0b11000000000000000000000000000000,
    };
    enum Rights
    {
        AllRights = 0,
        AccessDisable = 0b01010101010101010101010101010101,
        WriteDisable = 0b10101010101010101010101010101010,
        AccessWriteDisable = AccessDisable | WriteDisable,
    };

    enum RightsResult
    {
        ResultAllRights = 0,
        ResultAccessDisable = 0b01,
        ResultWriteDisable = 0b10,
        ResultAccessWriteDisable = 0b11,
    };

    static void write(unsigned int new_value);
    static void write_partial(unsigned int new_value, unsigned int key_mask);
    static unsigned int read(void);

    static void write(Key key, Rights rights);
    static bool has_rights(Key key, Rights rights);
    static RightsResult rights(Key key);
};