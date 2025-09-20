#include <stdbool.h>
#include <stdio.h>
#include <subnet/ip.h>

#define IP_ADDRESS_SIZE 32
int main()
{
    //===========================================================================================
    printf("Enter IP: ");
    IP ip            = { 0 };
    IP mask          = { 0 };
    IP_Class nw_addr = { 0 };
    int octets_read  = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    if (octets_read < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octet(s) to 0.\n", octets_read);
    }

    nw_addr = lookup(ip);
    SET_BITS(mask.addr, nw_addr.lsb, IP_ADDRESS_SIZE - 1);

    //===========================================================================================
    printf("Enter Subnet Count: ");
    unsigned char subnet_cnt    = 0;
    unsigned char subnet_bitcnt = 0;
    unsigned char subnet_lsb    = 0;
    IP subnet_mask              = { 0 };
    bool subnet_cnt_read        = scanf("%hhu", &subnet_cnt);
    if (subnet_cnt_read != true) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    subnet_bitcnt = get_bit_cnt(subnet_cnt);
    subnet_lsb    = nw_addr.lsb - subnet_bitcnt;

    SET_BITS(subnet_mask.addr, subnet_lsb, IP_ADDRESS_SIZE - 1);

    //===========================================================================================
    int range      = 1 << subnet_lsb;
    IP subnet_strt = { .addr = ip.addr & subnet_mask.addr };
    IP subnet_end  = { .addr = ip.addr & subnet_mask.addr };
    SET_BITS(subnet_end.addr, 0, subnet_lsb - 1);

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-91s │", ip_to_str(ip, nw_addr.lsb, 0));
    printf("\n│ MASK         │  %-91s │", ip_to_str(mask, nw_addr.lsb, 0));
    printf("\n│ CLASS        │  %-54s │", nw_addr.class_name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-91s │", ip_to_str(subnet_mask, nw_addr.lsb, subnet_bitcnt));
    printf("\n│ SUBNET BITS  │  %-54d │", subnet_bitcnt);
    printf("\n│ RANGE        │  %-54d │", range);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    for (int i = 1; i <= subnet_cnt; i++) {
        printf("\n│              │  %-54s │", "");
        printf("\n│ SUBNET NO.   │  %-54d │", i);
        printf("\n│ SUBNET START │  %-91s │", ip_to_str(subnet_strt, nw_addr.lsb, subnet_bitcnt));
        printf("\n│ SUBNET END   │  %-91s │", ip_to_str(subnet_end, nw_addr.lsb, subnet_bitcnt));
        subnet_strt.addr += range;
        subnet_end.addr += range;
    }
    printf("\n│              │  %-54s │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
