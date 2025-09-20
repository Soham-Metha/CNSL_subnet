#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    IP ip                 = { 0 };
    IP mask               = { 0 };
    IP sub_mask           = { 0 };
    IP first_subnet_strt  = { 0 };
    IP first_subnet_end   = { 0 };
    IP_Class class        = { 0 };
    unsigned char sub_cnt = 0;
    int cnt               = 0;

    //===========================================================================================
    printf("Enter IP:");
    cnt   = scanf("%hhu.%hhu.%hhu.%hhu", &ip.octet[3], &ip.octet[2], &ip.octet[1], &ip.octet[0]);
    class = lookup(ip);
    SET_BITS(mask.as_int, class.mask_start_at, 31);

    if (cnt < 4) {
        printf("\nWARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }

    //===========================================================================================
    printf("Enter Subnet Count:");
    cnt     = scanf("%hhu", &sub_cnt);
    sub_cnt = get_bit_cnt(sub_cnt);
    SET_BITS(sub_mask.as_int, class.mask_start_at - sub_cnt, 31);

    if (cnt < 1) {
        printf("\nWARN: Subnet count not entered, defaulted to 0.\n");
    }

    //===========================================================================================
    first_subnet_strt.as_int = ip.as_int & sub_mask.as_int;
    first_subnet_end.as_int  = ip.as_int & sub_mask.as_int;
    SET_BITS(first_subnet_end.as_int, 0, class.mask_start_at - sub_cnt);
    cnt = first_subnet_end.as_int - first_subnet_strt.as_int + 1;

    //===========================================================================================
    printf("\n┌──────────────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP                 │  %s │", ip_to_str(ip));
    printf("\n│ MASK               │  %s │", ip_to_str(mask));
    printf("\n│ CLASS              │  %s │", class.class_name);
    printf("\n├─────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK        │  %s │", ip_to_str(sub_mask));
    printf("\n│ FIRST SUBNET START │  %s │", ip_to_str(first_subnet_strt));
    printf("\n│ FIRST SUBNET END   │  %s │", ip_to_str(first_subnet_end));
    printf("\n│ RANGE              │  %d │", cnt);
    printf("\n└─────────────────────────────────────────────┘\n\n");

    return 0;
}
