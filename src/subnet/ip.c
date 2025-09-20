#include <stdio.h>
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
    while ((1 << pow) <= subnet_cnt)
        pow++;
    return pow;
}

void print_ip(IP ip)
{
    printf("%3hhu.%3hhu.%3hhu.%3hhu | ", ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);
    printf("%b.%b.%b.%b", ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);
}