#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    IP ip                 = { 0 };
    IP mask               = { 0 };
    IP sub_mask           = { 0 };
    IP_Class class        = { 0 };
    unsigned char sub_cnt = 0;
    int cnt               = 0;

    printf("Enter IP:");
    cnt   = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    class = lookup(ip);
    SET_BITS(mask.as_int, class.mask_start_at, 31);

    if (cnt < 4) {
        printf("\nWARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }

    printf("Enter Subnet Count:");
    cnt     = scanf("%hhu", &sub_cnt);
    sub_cnt = get_bit_cnt(sub_cnt);
    SET_BITS(sub_mask.as_int, class.mask_start_at - sub_cnt, 31);

    if (cnt < 1) {
        printf("\nWARN: Subnet count not entered, defaulted to 0.\n");
    }

    printf("\n----------------------------------------");
    printf("\nIP    |  %3hhu.%3hhu.%3hhu.%3hhu        ", ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);
    printf("\nMASK  |  %3hhu.%3hhu.%3hhu.%3hhu        ", mask.octet[3], mask.octet[2], mask.octet[1], mask.octet[0]);
    printf("\nCLASS |  %s                             ", class.class_name);
    printf("\nSUBNET|  %3hhu.%3hhu.%3hhu.%3hhu        ", sub_mask.octet[3], sub_mask.octet[2], sub_mask.octet[1], sub_mask.octet[0]);
    printf("\n----------------------------------------\n\n");
    return 0;
}
