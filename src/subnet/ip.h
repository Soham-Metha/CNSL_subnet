#ifndef PROJ
#define PROJ
#include <assert.h>
#include <inttypes.h>

#define IP_ADDRESS_SIZE 32
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

typedef struct Subnet_Range Subnet_Range;
typedef struct Subnet_Info Subnet_Info;
typedef struct IP_Class IP_Class;
typedef struct IP IP;
typedef union IP_addr IP_addr;

union IP_addr {
    unsigned char octet[4];
    uint32_t addr;
};

struct IP_Class {
    const char* class_name;
    unsigned char lsb_pos;
};

struct Subnet_Info {
    unsigned char cnt;
    unsigned char bit_cnt;
    unsigned char lsb_pos;
    IP_addr mask;
};

struct Subnet_Range {
    unsigned int size;
    IP_addr strt;
    IP_addr end;
};

struct IP {
    IP_addr as;
    IP_addr mask;
    IP_Class nw;
    Subnet_Info subnet;
    Subnet_Range range;
};

IP_Class lookup(const IP* ip);
unsigned char get_bit_cnt(unsigned char subnet_cnt);
const char* ip_to_str(const IP* ip, unsigned char mask_start_at, unsigned char subnet_lsb);

#endif
