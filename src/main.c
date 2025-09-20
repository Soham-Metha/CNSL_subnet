#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    printf("Enter IP:");
    IP ip          = { 0 };
    int cnt        = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[0], &ip.octet[1], &ip.octet[2], &ip.octet[3]);
    IP_Class class = lookup(ip);

    if (cnt < 4) {
        printf("\nWARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }
    printf("\n----------------------------------------");
    printf("\nIP    |  %hhu.%hhu.%hhu.%hhu            ", ip.octet[0], ip.octet[1], ip.octet[2], ip.octet[3]);
    printf("\nCLASS |  %s                             ", class.class_name);
    printf("\n----------------------------------------\n\n");

    return 0;
}
