#include "../../include/pkrulib.h"

extern "C" unsigned int pkru_lib_read_PKRU(void);
extern "C" void pkru_lib_write_PKRU(unsigned int pkruValue);

void PKRUlib::write(unsigned int new_value)
{
    pkru_lib_write_PKRU(new_value);
}

unsigned int PKRUlib::read(void)
{
    return pkru_lib_read_PKRU();
}

void PKRUlib::write_partial(unsigned int new_value, unsigned int mask)
{
    unsigned int current_pkru = PKRUlib::read();
    unsigned int zeroed_mask_current_pkru = current_pkru & ~mask; // zero to change bits with or
    unsigned int masked_new_value = new_value & mask;             // only change bits represented by the mask
    PKRUlib::write(zeroed_mask_current_pkru | masked_new_value);
}

void PKRUlib::write(Key key, Rights rights)
{
    write_partial(rights, key);
}

bool PKRUlib::has_rights(Key key, Rights rights)
{
    unsigned int current_pkru = PKRUlib::read();
    unsigned int only_key_rights = current_pkru & key;
    unsigned int desired_key_rights = key & rights;

    // there is no bit operator to check for allrights (well not or but why use that) or Access(and)WriteDisable
    if (rights == PKRUlib::Rights::AllRights || rights == PKRUlib::Rights::AccessWriteDisable)
        return only_key_rights == desired_key_rights;
    else
        return only_key_rights & desired_key_rights;
}

PKRUlib::RightsResult PKRUlib::rights(Key key)
{
    unsigned int current_pkru = PKRUlib::read();
    unsigned int only_key_rights = current_pkru & key;

    switch (key)
    {
    case Key_0:
        /* no change */
        break;
    case Key_1:
        only_key_rights = only_key_rights >> 2;
        break;
    case Key_2:
        only_key_rights = only_key_rights >> 4;
        break;
    case Key_3:
        only_key_rights = only_key_rights >> 6;
        break;
    case Key_4:
        only_key_rights = only_key_rights >> 8;
        break;
    case Key_5:
        only_key_rights = only_key_rights >> 10;
        break;
    case Key_6:
        only_key_rights = only_key_rights >> 12;
        break;
    case Key_7:
        only_key_rights = only_key_rights >> 14;
        break;
    case Key_8:
        only_key_rights = only_key_rights >> 16;
        break;
    case Key_9:
        only_key_rights = only_key_rights >> 18;
        break;
    case Key_10:
        only_key_rights = only_key_rights >> 20;
        break;
    case Key_11:
        only_key_rights = only_key_rights >> 22;
        break;
    case Key_12:
        only_key_rights = only_key_rights >> 24;
        break;
    case Key_13:
        only_key_rights = only_key_rights >> 26;
        break;
    case Key_14:
        only_key_rights = only_key_rights >> 28;
        break;
    case Key_15:
        only_key_rights = only_key_rights >> 30;
        break;
    }
    return static_cast<RightsResult>(only_key_rights);
}