#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    IP ip                 = { 0 };
    IP mask               = { 0 };
    IP sub_mask           = { 0 };
    IP subnet_strt        = { 0 };
    IP subnet_end         = { 0 };
    IP_Class class        = { 0 };
    unsigned char sub_cnt = 0;
    unsigned char bit_cnt = 0;
    int cnt               = 0;

    //===========================================================================================
    printf("Enter IP: ");
    cnt   = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    class = lookup(ip);
    SET_BITS(mask.as_int, class.nw_addr_lsb, 31);

    if (cnt < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octet(s) to 0.\n", cnt);
    }

    //===========================================================================================
    printf("Enter Subnet Count: ");
    cnt     = scanf("%hhu", &sub_cnt);
    bit_cnt = get_bit_cnt(sub_cnt);
    SET_BITS(sub_mask.as_int, class.nw_addr_lsb - bit_cnt, 31);

    if (cnt < 1) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    //===========================================================================================
    subnet_strt.as_int = ip.as_int & sub_mask.as_int;
    subnet_end.as_int  = ip.as_int & sub_mask.as_int;
    SET_BITS(subnet_end.as_int, 0, class.nw_addr_lsb - bit_cnt - 1);
    cnt = subnet_end.as_int - subnet_strt.as_int + 1;

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-344s │", ip_to_str(ip, class.nw_addr_lsb, 0));
    printf("\n│ MASK         │  %-344s │", ip_to_str(mask, class.nw_addr_lsb, 0));
    printf("\n│ CLASS        │  %-53s  │", class.class_name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-344s │", ip_to_str(sub_mask, class.nw_addr_lsb, bit_cnt));
    printf("\n│ SUBNET BITS  │  %-53d  │", bit_cnt);
    printf("\n│ RANGE        │  %-53d  │", cnt);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    for (int i = 1; i <= sub_cnt; i++) {
        printf("\n│              │  %-53s  │", "");
        printf("\n│ SUBNET NO.   │  %-53d  │", i);
        printf("\n│ SUBNET START │  %-344s │", ip_to_str(subnet_strt, class.nw_addr_lsb, bit_cnt));
        printf("\n│ SUBNET END   │  %-344s │", ip_to_str(subnet_end, class.nw_addr_lsb, bit_cnt));
        subnet_strt.as_int += cnt;
        subnet_end.as_int += cnt;
    }
    printf("\n│              │  %-53s  │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
