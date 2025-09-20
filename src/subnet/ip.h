#ifndef PROJ
#define PROJ
#include <assert.h>
#include <inttypes.h>

#define SET_BIT(val, pos)                 \
    do {                                  \
        assert((pos) >= 0 && (pos) < 32); \
        (val) |= (1U << (pos));           \
    } while (0)

#define SET_BITS(val, start, stop)                \
    do {                                          \
        for (int i = (start); i <= (stop); i++) { \
            SET_BIT(val, i);                      \
        }                                         \
    } while (0)

typedef enum IP_classes {
    CLASS_A,
    CLASS_B,
    CLASS_C,
    CLASS_D,
    CLASS_E,
    CLASS_CNT,
} IP_classes;

typedef struct IP_Class IP_Class;
typedef union IP IP;

union IP {
    unsigned char octet[4];
    uint32_t as_int;
};

struct IP_Class {
    const char* class_name;
    unsigned char end_at;
    unsigned char mask_start_at;
};

IP_Class lookup(IP ip);
unsigned char get_bit_cnt(unsigned char subnet_cnt);
const char* ip_to_str(IP ip, IP mask);

#endif
