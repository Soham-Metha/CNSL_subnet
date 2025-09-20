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
    return pow + 1;
}

char byte_str[8] = "00000000";
char* get_bits(unsigned char byte)
{
    for (int i = 7; i >= 0; i--) {
        byte_str[8 - i - 1] = (byte & (1 << i)) ? '1' : '0';
    }
    return byte_str;
}

const char* ip_to_str(IP ip, IP mask)
{
    static char buf[128];
    char* ptr = buf;

    ptr += sprintf(ptr, "%3hhu.%3hhu.%3hhu.%3hhu │ ",
        ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);

    for (int i = 3; i >= 0; i--) {
        unsigned char oct = ip.octet[i];
        unsigned char msk = mask.octet[i];

        for (int bit = 7; bit >= 0; bit--) {
            if (msk & (1 << bit)) {
                ptr += sprintf(ptr, "\033[32m%c\033[0m", (oct & (1 << bit)) ? '1' : '0');
            } else {
                ptr += sprintf(ptr, "\033[31m%c\033[0m", (oct & (1 << bit)) ? '1' : '0');
            }
        }
        if (i > 0)
            ptr += sprintf(ptr, ".");
        else
            ptr += sprintf(ptr, " ");
    }

    return buf;
}
