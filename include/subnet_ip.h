#ifndef PROJ
#define PROJ
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#define IP_ADDRESS_SIZE 32

#define TEST_BIT(val, pos) ((val) & (1 << (pos)))

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

typedef union  IP_addr      IP_addr;
typedef struct Subnet_Range Subnet_Range;
typedef struct Subnet  Subnet;
typedef struct IP_Class     IP_Class;
typedef struct IP           IP;

union IP_addr {
    uint8_t  octet[4];
    uint32_t addr;
};

struct IP_Class {
    const char* class_name;
    uint8_t     lsb_pos;
};

struct Subnet {
    IP_addr  mask;
    uint32_t cnt;
    uint8_t  bit_cnt;
    uint8_t  lsb_pos;
};

struct Subnet_Range {
    IP_addr  strt;
    IP_addr  end;
    uint32_t size;
};

struct IP {
    IP_addr  addr;
    IP_addr  mask;
    IP_Class nw;
    Subnet   subnet;
};

IP_Class lookup(IP_addr ip);
uint8_t get_bit_cnt(uint32_t subnet_cnt);
const char* ip_to_str(IP_addr ip, uint8_t mask_start_at, uint8_t subnet_lsb);

#endif
