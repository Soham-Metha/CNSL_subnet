#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    printf("Enter IP:");
    IP ip          = { 0 };
    IP mask        = { 0 };
    int cnt        = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    IP_Class class = lookup(ip);

    SET_BITS(mask.as_int, class.mask_start_at, 31);

    if (cnt < 4) {
        printf("\nWARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }
    printf("\n----------------------------------------");
    printf("\nIP    |  %3hhu.%3hhu.%3hhu.%3hhu        ", ip.octet[3], ip.octet[2], ip.octet[1], ip.octet[0]);
    printf("\nMASK  |  %3hhu.%3hhu.%3hhu.%3hhu        ", mask.octet[3], mask.octet[2], mask.octet[1], mask.octet[0]);
    printf("\nCLASS |  %s                             ", class.class_name);
    printf("\n----------------------------------------\n\n");
    return 0;
}
