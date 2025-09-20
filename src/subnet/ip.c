#include <stdio.h>
#include <string.h>
#include <subnet/ip.h>

static IP_Class IP_LUT[CLASS_CNT] = {
    [CLASS_A] = { .class_name = "CLASS A", .end_at = 127, .mask_start_at = 24 },
    [CLASS_B] = { .class_name = "CLASS B", .end_at = 191, .mask_start_at = 16 },
    [CLASS_C] = { .class_name = "CLASS C", .end_at = 223, .mask_start_at = 8  },
    [CLASS_D] = { .class_name = "CLASS D", .end_at = 239, .mask_start_at = 0  },
    [CLASS_E] = { .class_name = "CLASS E", .end_at = 255, .mask_start_at = 0  },
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
    while ((1 << pow) < subnet_cnt) {
        pow += 1;
    }
    return pow;
}

const char* ip_to_str(IP ip, unsigned char mask_start_at, unsigned char subnet_bit_cnt)
{
    static char buf[128];
    char* ptr = buf;

    ptr += sprintf(ptr, "%3hhu.%3hhu.%3hhu.%3hhu │ ",
        ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);

    for (int i = 31; i >= 0; i--) {
        if (i >= mask_start_at)
            ptr += sprintf(ptr, "\033[32m");
        else if (i >= mask_start_at - subnet_bit_cnt)
            ptr += sprintf(ptr, "\033[33m");
        else
            ptr += sprintf(ptr, "\033[31m");

        ptr += sprintf(ptr, "%c\033[0m", (ip.as_int & (1 << i)) ? '1' : '0');

        if (i % 8 == 0 && i != 0)
            ptr += sprintf(ptr, ".");
    }

    return buf;
}
