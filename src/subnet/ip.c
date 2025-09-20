#include <subnet/ip.h>

IP_Class lookup(IP_addr ip)
{
    if (ip.octet[3] <= 127)
        return (IP_Class) { .class_name = "CLASS A", .lsb_pos = 24 };
    if (ip.octet[3] <= 191)
        return (IP_Class) { .class_name = "CLASS B", .lsb_pos = 16 };
    if (ip.octet[3] <= 223)
        return (IP_Class) { .class_name = "CLASS C", .lsb_pos = 8 };
    if (ip.octet[3] <= 239)
        return (IP_Class) { .class_name = "CLASS D", .lsb_pos = 0 };
    else
        return (IP_Class) { .class_name = "CLASS E", .lsb_pos = 0 };
}

uint8_t get_bit_cnt(uint8_t subnet_cnt)
{
    uint8_t pow = 0;
    while ((1 << pow++) < subnet_cnt) { }
    return pow - 1;
}

const char* ip_to_str(IP_addr ip, uint8_t nw_addr_lsb, uint8_t subnet_lsb)
{
    static char buf[345];
    char* ptr = buf;

    {     // Decimal notation
        ptr += sprintf(ptr, "\033[32m"); // Color for the nw addr
        for (int8_t i = 3; i >= 0; i--) {

            ptr += sprintf(ptr, "%3hhu", ip.octet[i]);
            if (i != 0) ptr += sprintf(ptr, ".");

            if ((8 * i) == nw_addr_lsb) ptr += sprintf(ptr, "\033[34m"); // if current bit is lsb of nw addr, next bit is the start of subnet addr
            if ((8 * i) == subnet_lsb)  ptr += sprintf(ptr, "\033[31m"); // if current bit is lsb of subnet addr, next bit is the start of host addr
        }
        ptr += sprintf(ptr, "\033[0m │ ");
    }

    {     // binary notation
        ptr += sprintf(ptr, "\033[32m"); // Color for the nw addr
        for (int8_t i = 31; i >= 0; i--) {

            ptr += sprintf(ptr, "%c", TEST_BIT(ip.addr, i) ? '1' : '0');
            if (i % 8 == 0 && i != 0)  ptr += sprintf(ptr, ".");

            if (i == nw_addr_lsb) ptr += sprintf(ptr, "\033[34m"); // if current bit is lsb of nw addr, next bit is the start of subnet addr
            if (i == subnet_lsb)  ptr += sprintf(ptr, "\033[31m"); // if current bit is lsb of subnet addr, next bit is the start of host addr
        }
        ptr += sprintf(ptr, "\033[0m ");
    }
    return buf;
}
