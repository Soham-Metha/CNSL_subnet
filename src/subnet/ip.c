#include <stdio.h>
#include <string.h>
#include <subnet/ip.h>

static IP_Class IP_LUT[CLASS_CNT] = {
    [CLASS_A] = { .name = "CLASS A", .end_at = 127, .nw_addr_lsb = 24 },
    [CLASS_B] = { .name = "CLASS B", .end_at = 191, .nw_addr_lsb = 16 },
    [CLASS_C] = { .name = "CLASS C", .end_at = 223, .nw_addr_lsb = 8  },
    [CLASS_D] = { .name = "CLASS D", .end_at = 239, .nw_addr_lsb = 0  },
    [CLASS_E] = { .name = "CLASS E", .end_at = 255, .nw_addr_lsb = 0  },
};

IP_Class lookup(IP ip)
{
    for (int i = CLASS_A; i < CLASS_CNT; i++) {
        if (ip.octet[3] <= IP_LUT[i].end_at) {
            return IP_LUT[i];
        }
    }
    assert(0 && "INVALID IP ADDRESS!!!");
}

unsigned char get_bit_cnt(unsigned char subnet_cnt)
{
    unsigned char pow = 0;
    while ((1 << pow++) < subnet_cnt) { }
    return pow - 1;
}

const char* ip_to_str(IP ip, unsigned char nw_addr_lsb, unsigned char subnet_bit_cnt)
{
    static char buf[345];
    char* ptr = buf;

    ptr += sprintf(ptr, "\033[32m");
    for (int i = 3; i >= 0; i--) {

        ptr += sprintf(ptr, "%3hhu", ip.octet[i]);
        if (i != 0)
            ptr += sprintf(ptr, ".");

        if ((8 * i) == nw_addr_lsb)
            ptr += sprintf(ptr, "\033[33m");
        if ((8 * i) == nw_addr_lsb - subnet_bit_cnt)
            ptr += sprintf(ptr, "\033[31m");
    }
    ptr += sprintf(ptr, "\033[0m │ \033[32m");

    for (int i = 31; i >= 0; i--) {

        ptr += sprintf(ptr, "%c", TEST_BIT(ip.as_int, i) ? '1' : '0');
        if (i % 8 == 0 && i != 0)
            ptr += sprintf(ptr, ".");

        if (i == nw_addr_lsb)
            ptr += sprintf(ptr, "\033[33m");
        if (i == nw_addr_lsb - subnet_bit_cnt)
            ptr += sprintf(ptr, "\033[31m");
    }
    ptr += sprintf(ptr, "\033[0m ");

    return buf;
}
