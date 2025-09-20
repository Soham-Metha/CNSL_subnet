#include <stdbool.h>
#include <stdio.h>
#include <subnet/ip.h>

#define IP_ADDRESS_SIZE 32
int main()
{
    //===========================================================================================
    printf("Enter IP: ");
    IP ip           = { 0 };
    int octets_read = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    IP_Class class  = lookup(ip);
    IP mask         = { 0 };
    SET_BITS(mask.addr, class.nw_addr_lsb, IP_ADDRESS_SIZE - 1);

    if (octets_read < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octet(s) to 0.\n", octets_read);
    }

    //===========================================================================================
    printf("Enter Subnet Count: ");
    unsigned char sub_cnt        = 0;
    bool sub_cnt_read            = scanf("%hhu", &sub_cnt);
    unsigned char subnet_bit_cnt = get_bit_cnt(sub_cnt);
    unsigned char subnet_lsb     = class.nw_addr_lsb - subnet_bit_cnt;
    IP sub_mask                  = { 0 };
    SET_BITS(sub_mask.addr, subnet_lsb, IP_ADDRESS_SIZE - 1);

    if (sub_cnt_read < 1) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    //===========================================================================================
    int range      = 1 << subnet_lsb;
    IP subnet_strt = { .addr = ip.addr & sub_mask.addr };
    IP subnet_end  = { .addr = ip.addr & sub_mask.addr };
    SET_BITS(subnet_end.addr, 0, subnet_lsb - 1);

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-344s │", ip_to_str(ip, class.nw_addr_lsb, 0));
    printf("\n│ MASK         │  %-344s │", ip_to_str(mask, class.nw_addr_lsb, 0));
    printf("\n│ CLASS        │  %-53s  │", class.name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-344s │", ip_to_str(sub_mask, class.nw_addr_lsb, subnet_bit_cnt));
    printf("\n│ SUBNET BITS  │  %-53d  │", subnet_bit_cnt);
    printf("\n│ RANGE        │  %-53d  │", range);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    for (int i = 1; i <= sub_cnt; i++) {
        printf("\n│              │  %-53s  │", "");
        printf("\n│ SUBNET NO.   │  %-53d  │", i);
        printf("\n│ SUBNET START │  %-344s │", ip_to_str(subnet_strt, class.nw_addr_lsb, subnet_bit_cnt));
        printf("\n│ SUBNET END   │  %-344s │", ip_to_str(subnet_end, class.nw_addr_lsb, subnet_bit_cnt));
        subnet_strt.addr += range;
        subnet_end.addr += range;
    }
    printf("\n│              │  %-53s  │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
