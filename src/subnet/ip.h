#ifndef PROJ
#define PROJ
#include <assert.h>
#include <inttypes.h>

#define TEST_BIT(val, pos) (val) & (1 << (pos))

#define SET_BIT(val, pos)                 \
    do {                                  \
        assert((pos) >= 0 && (pos) < 32); \
        (val) |= (1 << (pos));            \
    } while (0)

#define SET_BITS(val, start, stop)                \
    do {                                          \
        for (int i = (start); i <= (stop); i++) { \
            SET_BIT(val, i);                      \
        }                                         \
    } while (0)

typedef struct NW_Class NW_Class;
typedef union IP IP;

union IP {
    unsigned char octet[4];
    uint32_t addr;
};

struct NW_Class {
    const char* name;
    unsigned char nw_addr_lsb;
};

NW_Class lookup(IP ip);
unsigned char get_bit_cnt(unsigned char subnet_cnt);
const char* ip_to_str(IP ip, unsigned char mask_start_at, unsigned char subnet_bit_cnt);

#endif
