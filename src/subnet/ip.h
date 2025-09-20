#ifndef PROJ
#define PROJ
#include <assert.h>

#define SET_BIT(val, pos)             \
    {                                 \
        assert(pos >= 0 && pos < 32); \
        val = val | (1 << pos);       \
    }

#define SET_BITS(val, start, stop)            \
    {                                         \
        for (int i = start; i <= stop; i++) { \
            SET_BIT(val, i)                   \
        }                                     \
    }

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
    int as_int;
};

struct IP_Class {
    const char* class_name;
    unsigned char end_at;
    unsigned char mask_start_at;
};

IP_Class lookup(IP ip);

#endif
