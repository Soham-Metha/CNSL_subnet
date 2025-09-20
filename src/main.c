#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    printf("Enter IP:");
    IP ip          = { 0 };
    int cnt        = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[0], &ip.octet[1], &ip.octet[2], &ip.octet[3]);
    IP_Class class = lookup(ip);

    if (cnt < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }
	printf("----------------------------------------");
	printf("IP    |  %hhu.%hhu.%hhu.%hhu            ", &ip.octet[0], &ip.octet[1], &ip.octet[2], &ip.octet[3]);
    printf("%d %s\n", class.end_at, class.class_name);

    return 0;
}
