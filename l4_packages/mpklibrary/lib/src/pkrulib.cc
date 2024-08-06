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