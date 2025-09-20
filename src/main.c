#include <stdio.h>
#include <subnet/ip.h>
#define IP_ADDRESS_SIZE 32
int main()
{
    IP ip                        = { 0 };
    IP mask                      = { 0 };
    IP sub_mask                  = { 0 };
    IP subnet_strt               = { 0 };
    IP subnet_end                = { 0 };
    IP_Class class               = { 0 };
    unsigned char sub_cnt        = 0;
    unsigned char subnet_bit_cnt = 0;
    unsigned char subnet_lsb     = 0;
    int cnt                      = 0;

    //===========================================================================================
    printf("Enter IP: ");
    cnt   = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    class = lookup(ip);
    SET_BITS(mask.addr, class.nw_addr_lsb, IP_ADDRESS_SIZE - 1);

    if (cnt < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octet(s) to 0.\n", cnt);
    }

    //===========================================================================================
    printf("Enter Subnet Count: ");
    cnt            = scanf("%hhu", &sub_cnt);
    subnet_bit_cnt = get_bit_cnt(sub_cnt);
    subnet_lsb     = class.nw_addr_lsb - subnet_bit_cnt;
    SET_BITS(sub_mask.addr, subnet_lsb, IP_ADDRESS_SIZE - 1);

    if (cnt < 1) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    //===========================================================================================
    subnet_strt.addr = ip.addr & sub_mask.addr;
    subnet_end.addr  = ip.addr & sub_mask.addr;
    SET_BITS(subnet_end.addr, 0, subnet_lsb - 1);
    cnt = subnet_end.addr - subnet_strt.addr + 1;

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-344s │", ip_to_str(ip, class.nw_addr_lsb, 0));
    printf("\n│ MASK         │  %-344s │", ip_to_str(mask, class.nw_addr_lsb, 0));
    printf("\n│ CLASS        │  %-53s  │", class.name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-344s │", ip_to_str(sub_mask, class.nw_addr_lsb, subnet_bit_cnt));
    printf("\n│ SUBNET BITS  │  %-53d  │", subnet_bit_cnt);
    printf("\n│ RANGE        │  %-53d  │", cnt);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    for (int i = 1; i <= sub_cnt; i++) {
        printf("\n│              │  %-53s  │", "");
        printf("\n│ SUBNET NO.   │  %-53d  │", i);
        printf("\n│ SUBNET START │  %-344s │", ip_to_str(subnet_strt, class.nw_addr_lsb, subnet_bit_cnt));
        printf("\n│ SUBNET END   │  %-344s │", ip_to_str(subnet_end, class.nw_addr_lsb, subnet_bit_cnt));
        subnet_strt.addr += cnt;
        subnet_end.addr += cnt;
    }
    printf("\n│              │  %-53s  │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
