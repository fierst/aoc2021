#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdint.h>

// we can't have a packet shorter than 16 bits!
#define MIN_PACKET_LENGTH 16

#define BITS_SUBPACKET_LENGTH_TYPE_0 15
#define BITS_SUBPACKET_LENGTH_TYPE_1 11

typedef enum {
    /**
     * @brief Packets with type ID 0 are sum packets - their value is the sum of the 
     *        values of their sub-packets. If they only have a single sub-packet, 
     *        their value is the value of the sub-packet.
     * 
     */
    PT_SUM = 0,
    
    /**
     * @brief Packets with type ID 1 are product packets - their value is the result 
     *        of multiplying together the values of their sub-packets. If they only 
     *        have a single sub-packet, their value is the value of the sub-packet.
     * 
     */
    PT_PRODUCT = 1,

    /**
     * @brief Packets with type ID 2 are minimum packets - their value is the 
     *        minimum of the values of their sub-packets.
     * 
     */
    PT_MINIMUM = 2,

    /**
     * @brief Packets with type ID 3 are maximum packets - their value is the 
     *        maximum of the values of their sub-packets.
     * 
     */
    PT_MAXIMUM = 3,

    /**
     * @brief Packets with type ID 4 are literal values
     * 
     */
    PT_LITERAL = 4,

    /**
     * @brief Packets with type ID 5 are greater than packets - their value is 1 if 
     *        the value of the first sub-packet is greater than the value of the 
     *        second sub-packet; otherwise, their value is 0. These packets always 
     *        have exactly two sub-packets.
     * 
     */
    PT_GREATER_THAN = 5,

    /**
     * @brief Packets with type ID 6 are less than packets - their value is 1 if 
     *        the value of the first sub-packet is less than the value of the second 
     *        sub-packet; otherwise, their value is 0. These packets always have 
     *        exactly two sub-packets.
     * 
     */
    PT_LESS_THAN = 6,

    /**
     * @brief Packets with type ID 7 are equal to packets - their value is 1 if the 
     *        value of the first sub-packet is equal to the value of the second sub-
     *        packet; otherwise, their value is 0. These packets always have exactly 
     *        two sub-packets.
     * 
     */
    PT_EQUAL_TO = 7
} packet_type_t;

typedef enum {
    PS_VERSION,
    PS_TYPE,
    PS_LENGTH_TYPE,
    PS_SUBPACKET_LENGTH,
    PS_LITERAL,
    PS_SUBPACKETS,
    PS_DONE
} parse_state_t;
  
typedef struct packet_t{
    
    /**
     * @brief The packet version
     * 
     */
    uint8_t version;

    /**
     * @brief The packet type
     * 
     */
    packet_type_t type;

    /**
     * @brief The length type of the packet, either 0 or 1
     * 
     */
    uint8_t length_type;
    
    /**
     * @brief THe literal packet value (if applicable)
     * 
     */
    uint64_t literal_value;

    /**
     * @brief The subpacket length from the parent packet, either bits or number of packets
     * 
     */

    int subpacket_length;
    /**
     * @brief How many subpackets this packet has
     * 
     */
    int subpacket_count;

    /**
     * @brief An array of subpacket pointers, arbitrarily large (lol)
     * 
     */
    struct packet_t* subpackets[128];
} packet_t;

packet_t * parse_packet(int *bit_index);

uint64_t process_packet(packet_t * packet);

extern uint8_t packet_bits[__UINT16_MAX__];
extern uint16_t packet_length;


#endif //_PACKET_H_