#ifndef PROJ
#define PROJ

#define SET_BIT(val, pos) val = val & 1 << pos;

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
};

IP_Class lookup(IP ip);

#endif
