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
    SET_BITS(mask.as_int, class.mask_start_at, 31);

    if (cnt < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octets to 0.\n", cnt);
    }

    //===========================================================================================
    printf("Enter Subnet Count: ");
    cnt     = scanf("%hhu", &sub_cnt);
    bit_cnt = get_bit_cnt(sub_cnt);
    SET_BITS(sub_mask.as_int, class.mask_start_at - bit_cnt + 1, 31);

    if (cnt < 1) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    //===========================================================================================
    subnet_strt.as_int = ip.as_int & sub_mask.as_int;
    subnet_end.as_int  = ip.as_int & sub_mask.as_int;
    SET_BITS(subnet_end.as_int, 0, class.mask_start_at - bit_cnt);
    cnt = subnet_end.as_int - subnet_strt.as_int + 1;

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-54s │", ip_to_str(ip));
    printf("\n│ MASK         │  %-54s │", ip_to_str(mask));
    printf("\n│ CLASS        │  %-54s │", class.class_name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-54s │", ip_to_str(sub_mask));
    printf("\n│ RANGE        │  %-54d │", cnt);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    while (sub_cnt) {
        printf("\n│ SUBNET START │  %-54s │", ip_to_str(subnet_strt));
        printf("\n│ SUBNET END   │  %-54s │", ip_to_str(subnet_end));
        printf("\n│              │  %-54s │", "");
        subnet_strt.as_int += cnt;
        subnet_end.as_int += cnt;
        sub_cnt -= 1;
    }
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
