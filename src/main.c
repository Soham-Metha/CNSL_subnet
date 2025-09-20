#include <stdbool.h>
#include <stdio.h>
#include <subnet/ip.h>

int main()
{
    //===========================================================================================
    printf("Enter IP: ");
    IP ip           = { 0 };
    int octets_read = scanf("%hhu.%hhu.%hhu.%hhu", &ip.as.octet[3], &ip.as.octet[2], &ip.as.octet[1], &ip.as.octet[0]);
    if (octets_read < 4) {
        printf("WARN: Only read %d octet(s), defaulted remining octet(s) to 0.\n", octets_read);
    }

    ip.nw = lookup(ip.as);
    SET_BITS(ip.mask.addr, ip.nw.lsb_pos, IP_ADDRESS_SIZE - 1);

    //===========================================================================================
    printf("Enter Subnet Count: ");
    if (scanf("%hhu", &ip.subnet.cnt) != true) {
        printf("WARN: Subnet count not entered, defaulted to 0.\n");
    }

    ip.subnet.bit_cnt = get_bit_cnt(ip.subnet.cnt);
    ip.subnet.lsb_pos = ip.nw.lsb_pos - ip.subnet.bit_cnt;
    SET_BITS(ip.subnet.mask.addr, ip.subnet.lsb_pos, IP_ADDRESS_SIZE - 1);

    //===========================================================================================
    Subnet_Range range = {
        .size = 1 << ip.subnet.lsb_pos,
        .strt = { .addr = ip.as.addr & ip.subnet.mask.addr },
        .end  = { .addr = ip.as.addr & ip.subnet.mask.addr },
    };

    SET_BITS(range.end.addr, 0, ip.subnet.lsb_pos - 1);

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│ IP           │  %-91s │", ip_to_str(ip.as, ip.nw.lsb_pos, 0));
    printf("\n│ MASK         │  %-91s │", ip_to_str(ip.mask, ip.nw.lsb_pos, 0));
    printf("\n│ CLASS        │  %-54s │", ip.nw.class_name);
    printf("\n├────────────────────────────────────────────────────────────────────────┤");
    printf("\n│ SUBNET MASK  │  %-91s │", ip_to_str(ip.subnet.mask, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ SUBNET BITS  │  %-54d │", ip.subnet.bit_cnt);
    printf("\n│ RANGE        │  %-54d │", range.size);
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================
    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    for (int i = 1; i <= ip.subnet.cnt; i++) {
        printf("\n│              │  %-54s │", "");
        printf("\n│ SUBNET NO.   │  %-54d │", i);
        printf("\n│ SUBNET START │  %-91s │", ip_to_str(range.strt, ip.nw.lsb_pos, ip.subnet.lsb_pos));
        printf("\n│ SUBNET END   │  %-91s │", ip_to_str(range.end, ip.nw.lsb_pos, ip.subnet.lsb_pos));
        range.strt.addr += range.size;
        range.end.addr += range.size;
    }
    printf("\n│              │  %-54s │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n\n");
    return 0;
}
