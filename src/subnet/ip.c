#include <stdio.h>
#include <subnet/ip.h>
#include <assert.h>

static IP_Class IP_LUT[CLASS_CNT] = {
    [CLASS_E] = { .class_name = "CLASS E", .start_at = 248 },
    [CLASS_D] = { .class_name = "CLASS D", .start_at = 223 },
    [CLASS_C] = { .class_name = "CLASS C", .start_at = 192 },
    [CLASS_B] = { .class_name = "CLASS B", .start_at = 128 },
    [CLASS_A] = { .class_name = "CLASS A", .start_at = 0   },
};

IP_Class lookup(IP ip)
{
    for (int i = CLASS_E; i > -1; i--) {
        if (ip.octet[0] >= IP_LUT[i].start_at) {
            return IP_LUT[i];
        }
    }
    assert(0 && "INVALID IP ADDRESS!!!");
}
