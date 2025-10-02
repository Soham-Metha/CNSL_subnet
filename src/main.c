#include <Utils/cmd_line.h>
#include <subnet_ip.h>

int main(int argc, char** argv)
{
    cli_flag_push("--ip", "IP whose subnet is to be checked");
    cli_flag_push("--subnet-cnt", "Number of subnets in current network");
    cli_flag_push("--to-ping", "IP to be pinged");
    cli_parse(&argc, &argv);

    IP ip                     = { 0 };
    IP_addr target            = { 0 };
    Subnet_Range range        = { 0 };

    String_View subnet_cnt_sv = cli_flag_get_val(STR("--subnet-cnt"));
    ip.subnet.cnt             = sv_to_uint(&subnet_cnt_sv);

    String_View ip_sv         = cli_flag_get_val(STR("--ip"));
    uint8_t octet_cnt         = IP_ADDRESS_SIZE / 8;
    while (ip_sv.len) {
        String_View octet_sv       = sv_split_by_delim(&ip_sv, '.');
        ip.addr.octet[--octet_cnt] = sv_to_uint(&octet_sv);
    }

    String_View to_ip_sv     = cli_flag_get_val(STR("--to-ping"));
    uint8_t target_octet_cnt = IP_ADDRESS_SIZE / 8;
    while (to_ip_sv.len) {
        String_View octet_sv             = sv_split_by_delim(&to_ip_sv, '.');
        target.octet[--target_octet_cnt] = sv_to_uint(&octet_sv);
    }

    //===========================================================================================
    {     // determine IP Class & Mask
        ip.nw = lookup(ip.addr);
        SET_BITS(ip.mask.as_u32, ip.nw.lsb_pos, IP_ADDRESS_SIZE - 1);
    }

    {     // determine the no. of bits to borrow, position of the bits
        ip.subnet.bit_cnt = get_bit_cnt(ip.subnet.cnt);
        ip.subnet.lsb_pos = ip.nw.lsb_pos - ip.subnet.bit_cnt;
        SET_BITS(ip.subnet.mask.as_u32, ip.subnet.lsb_pos, IP_ADDRESS_SIZE - 1);
    }

    {     // Determine the subnet range, & the first,last addresses
        range = (Subnet_Range) {
            .size        = 1 << ip.subnet.lsb_pos,
            .strt.as_u32 = ip.addr.as_u32 & ip.subnet.mask.as_u32,
            .end.as_u32  = ip.addr.as_u32 & ip.subnet.mask.as_u32,
        };
        SET_BITS(range.end.as_u32, 0, ip.subnet.lsb_pos - 1);
    }
    //===========================================================================================

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

    printf("\n┌────────────────────────────────────────────────────────────────────────┐");
    printf("\n│              │  %-54s │", "");
    printf("\n│ IP           │  %-54s │", ip_to_str(ip.addr, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ TARGET       │  %-54s │", ip_to_str(target, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ SUBNET START │  %-54s │", ip_to_str(range.strt, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│ SUBNET END   │  %-54s │", ip_to_str(range.end, ip.nw.lsb_pos, ip.subnet.lsb_pos));
    printf("\n│              │  %-54s │", "");
    printf("\n└────────────────────────────────────────────────────────────────────────┘\n");

    if (target.as_u32 == range.end.as_u32) {
        printf("\nTarget address is the broadcast address!!\n\n\n");
        return 1;
    } else if (target.as_u32 == range.strt.as_u32) {
        printf("\nTarget address is the network address!!\n\n\n");
        return 1;
    } else if (target.as_u32 < range.strt.as_u32 || target.as_u32 > range.end.as_u32) {
        printf("\nTarget address from a different network/subnet!!\n\n\n");
        return 1;
    }
    printf("\nTarget address from the same subnet, pinging!!\n\n\n");
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