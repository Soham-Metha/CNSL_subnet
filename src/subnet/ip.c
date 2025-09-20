#include <stdio.h>
#include <string.h>
#include <subnet/ip.h>

IP_Class lookup(IP ip)
{
    if (ip.octet[3] <= 127)
        return (IP_Class) { .name = "CLASS A", .nw_addr_lsb = 24 };
    if (ip.octet[3] <= 191)
        return (IP_Class) { .name = "CLASS B", .nw_addr_lsb = 16 };
    if (ip.octet[3] <= 223)
        return (IP_Class) { .name = "CLASS C", .nw_addr_lsb = 8 };
    if (ip.octet[3] <= 239)
        return (IP_Class) { .name = "CLASS D", .nw_addr_lsb = 0 };
    else
        return (IP_Class) { .name = "CLASS E", .nw_addr_lsb = 0 };
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
