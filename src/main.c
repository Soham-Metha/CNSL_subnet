#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    printf("Enter IP:");
    IP ip;
    scanf("%hhd.%hhd.%hhd.%hhd", &ip.octet[0], &ip.octet[1], &ip.octet[2], &ip.octet[3]);
    IP_Class class = lookup(ip);
    printf("%d %s\n", class, class.class_name);

    return 0;
}
