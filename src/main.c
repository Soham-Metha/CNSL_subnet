#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    printf("Enter IP:");
    IP ip   = { 0 };
    int cnt = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[0], &ip.octet[1], &ip.octet[2], &ip.octet[3]);

    if (cnt < 4) {
        printf("ERROR: Only read %d octets!\n", cnt);
    }
    IP_Class class = lookup(ip);
    printf("%d %s\n", class.start_at, class.class_name);

    return 0;
}
