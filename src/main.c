#include <subnet_ip.h>

int main()
{
    //===========================================================================================
    IP ip              = { 0 };
    Subnet_Range range = { 0 };
    {     // Read IP Addr
        printf("Enter IP: ");
        int octets_read = scanf("%hhu.%hhu.%hhu.%hhu",
            &ip.addr.octet[3], &ip.addr.octet[2], &ip.addr.octet[1], &ip.addr.octet[0]);
        if (octets_read < 1)   ip.addr.octet[3] = 192;
        if (octets_read < 2)   ip.addr.octet[2] = 168;
        if (octets_read < 3)   ip.addr.octet[1] = 4;
        if (octets_read < 4) { ip.addr.octet[0] = 1;
            printf("WARN: Only read %d octet(s), defaulted remining octet(s) to <192>.<168>.<4>.<1>\n", octets_read);
        }
    }

    {     // determine IP Class & Mask
        ip.nw = lookup(ip.addr);
        SET_BITS(ip.mask.addr, ip.nw.lsb_pos, IP_ADDRESS_SIZE - 1);
    }

    {     // Read Subnet Count
        printf("Enter Subnet Count: ");
        if (scanf("%u", &ip.subnet.cnt) < 1) {
            printf("WARN: Subnet count not entered, defaulted to 0.\n");
        }
    }

    {     // determine the no. of bits to borrow, position of the bits
        ip.subnet.bit_cnt = get_bit_cnt(ip.subnet.cnt);
        ip.subnet.lsb_pos = ip.nw.lsb_pos - ip.subnet.bit_cnt;
        SET_BITS(ip.subnet.mask.addr, ip.subnet.lsb_pos, IP_ADDRESS_SIZE - 1);
    }

    {     // Determine the subnet range, & the first,last addresses
        range = (Subnet_Range) {
            .size = 1 << ip.subnet.lsb_pos,
            .strt = { .addr = ip.addr.addr & ip.subnet.mask.addr },
            .end  = { .addr = ip.addr.addr & ip.subnet.mask.addr },
        };
        SET_BITS(range.end.addr, 0, ip.subnet.lsb_pos - 1);
    }

    //===========================================================================================
    {     // print the details
        printf("\n┌────────────────────────────────────────────────────────────────────────┐");
        printf("\n│              │  %-54s │", "");
        printf("\n│ IP           │  %-54s │", ip_to_str(ip.addr, ip.nw.lsb_pos, ip.nw.lsb_pos));
        printf("\n│ MASK         │  %-54s │", ip_to_str(ip.mask, ip.nw.lsb_pos, ip.nw.lsb_pos));
        printf("\n│ CLASS        │  %-54s │", ip.nw.class_name);
        printf("\n│ SUBNETS REQ  │  %-54d │", ip.subnet.cnt);
        printf("\n│              │  %-54s │", "");
        printf("\n├────────────────────────────────────────────────────────────────────────┤");
        printf("\n│              │  %-54s │", "");
        printf("\n│ SUBNET MASK  │  %-54s │", ip_to_str(ip.subnet.mask, ip.nw.lsb_pos, ip.subnet.lsb_pos));
        printf("\n│ SUBNET BITS  │  %-54d │", ip.subnet.bit_cnt);
        printf("\n│ RANGE        │  %-54d │", range.size);
        printf("\n│ USABLE       │  %-54d │", (range.size > 2) ? range.size - 2 : 0);
        printf("\n│              │  %-54s │", "");
        printf("\n└────────────────────────────────────────────────────────────────────────┘\n");
    }

    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│              │  %-54s │", "");
    printf("\n│ IP           │  %-54s │", ip_to_str(ip.addr, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ SUBNET START │  %-54s │", ip_to_str(range.strt, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ SUBNET END   │  %-54s │", ip_to_str(range.end, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│              │  %-54s │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    //===========================================================================================

    // for (int i = 1; i <= ip.subnet.cnt; i++) {     // print all subnet ranges
    //     printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    //     printf("\n│              │  %-54s │", "");
    //     printf("\n│ SUBNET NO.   │  %-54d │", i);
    //     printf("\n│ SUBNET START │  %-54s │", ip_to_str(range.strt, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    //     printf("\n│ SUBNET END   │  %-54s │", ip_to_str(range.end, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    //     printf("\n│              │  %-54s │", "");
    //     printf("\n└────────────────────────────────────────────────────────────────────────┘\n");
    //     range.strt.addr += range.size;
    //     range.end.addr += range.size;
    //     // !BEFORE PRINTING, ensure that you apply the default mask instead of subnet mask!
    // }

    return 0;
}

/*
 * Assume IP Addr '192.168.4.1' & 7 subnets needed
 *     ip.addr.octet[3]     |    ip.addr.octet[2]     |    ip.addr.octet[1]     |    ip.addr.octet[0]
 *  31 30 29 28 27 26 25 24 | 23 22 21 20 19 18 17 16 | 15 14 13 12 11 10  9  8 |  7  6  5  4  3  2  1  0
 * [##_##_##_##_##_##_##_## | ##_##_##_##_##_##_##_## | ##_##_##_##_##_##_##_## | ##_##_##_##_##_##_##_## ]
 *   ^~~~~                                                                ~~~~^    ^     ^~~~
 *   IP_ADDRESS_SIZE-1                                            ip.nw.lsb_pos    ^     ip.subnet.lsb_pos
 *   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    +++++++
 * 							      ip.mask		         	                       ip.subnet.bit_cnt
 *   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  +++++++++++++
 *                                                 ip.subnet.mask                           host addr (not needed)
 *
 * Bit position ranges :
 * Host addr    = bit 0                   to (<ip.subnet.lsb_pos> - 1)
 * Subnet addr  = bit <ip.subnet.lsb_pos> to (<ip.nw.lsb_pos>    - 1)
 * Network addr = bit <ip.nw.lsb_pos>     to (<IP_ADDRESS_SIZE> - 1)
 *
 * Since octet 3 is in range 192-224, it's a CLASS C address.
 * For class C, mask bits 8 to 31 will be set.
 *
 * For 10 subnets needed, smallest power of 2 which is greater than 7 => 8 (2^3)
 * we have bit_cnt as 4, position of the subnet address LSB can be determined as
 *     NW ADDR LSB - BIT_CNT     =>    8 - 3    => 5
 * For subnet mask, bits 5 to 31 will be set.
 *
 * since bit 5 is the subnet addr LSB, the 4th bit will be the host addr MSB.
 * Bits 0 to 4 form the host address, if we set all these bits to 0, we get the starting address of the current subnet.
 * If we set these bits to 1, we get the ending address of the current subnet.
 *
 * Subnet is identified by the subnet bits,
 *     000 => SUBNET 1,  001 => SUBNET 2,  010 => SUBNET 3, ... 110 => SUBNET 7,  111 => SUBNET 8
 * Although the 8th subnet is created, it's not needed, hence wasted.
 * 
 * If we apply the subnet mask to any IP, we get it's nw & subnet address.
 */