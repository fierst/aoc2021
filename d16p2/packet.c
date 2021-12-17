#include "packet.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
    
uint8_t packet_bits[__UINT16_MAX__];
uint16_t packet_length = 0;

uint64_t process_packet(packet_t * packet) {
    uint64_t value = 0;

    switch(packet->type) {
        case PT_SUM: 
        {
            uint64_t sum = 0;
            for(int i = 0; i < packet->subpacket_count; i++) {
                sum += packet->subpackets[i]->literal_value;
            }
            value = sum;
        }
        break;
        case PT_PRODUCT: {
            uint64_t prod = 1;
            for(int i = 0; i < packet->subpacket_count; i++) {
                prod *= packet->subpackets[i]->literal_value;
            }
            value = prod;            
        }
        break;
        case PT_MINIMUM: {
            uint64_t min = __INT_MAX__;
            for(int i = 0; i < packet->subpacket_count; i++) {
                if(packet->subpackets[i]->literal_value < min) {
                    min = packet->subpackets[i]->literal_value;
                }
            }
            value = min;
        }
        break;
        case PT_MAXIMUM: {
            uint64_t max = 0;
            for(int i = 0; i < packet->subpacket_count; i++) {
                if(packet->subpackets[i]->literal_value > max) {
                    max = packet->subpackets[i]->literal_value;
                }
            }
            value = max;
        }
        break;
        case PT_GREATER_THAN: {
            value = (packet->subpackets[0]->literal_value > packet->subpackets[1]->literal_value) ? 1 : 0;
        }
        break;
        case PT_LESS_THAN: {
            value = (packet->subpackets[0]->literal_value < packet->subpackets[1]->literal_value) ? 1 : 0;
        }
        break;
        case PT_EQUAL_TO: {
            value = (packet->subpackets[0]->literal_value == packet->subpackets[1]->literal_value) ? 1 : 0;
        }
        break;
    }

    return value;
}

bool all_subpackets_are_literals(packet_t * packet) {
    for(int i = 0; i < packet->subpacket_count; i++) {
        if(packet->subpackets[i]->type != PT_LITERAL) return false;
    }
    return true;
}

static void print_packet(packet_t * packet, int starting_bit) {
    printf("packet @ %d -- %d [%ld]\n", starting_bit, packet->type, packet->literal_value);
}

packet_t * parse_packet(int *bit_index) {
    
    packet_t * new_packet = malloc(sizeof(packet_t));

    int starting_bit = *bit_index;

    parse_state_t state = PS_VERSION;

    while(state != PS_DONE) {
        switch(state) {
            case PS_VERSION:
                new_packet->version |= (packet_bits[(*bit_index)++] << 2);
                new_packet->version |= (packet_bits[(*bit_index)++] << 1);
                new_packet->version |= (packet_bits[(*bit_index)++]);
                state = PS_TYPE;
                break;
            case PS_TYPE:
                new_packet->type |= (packet_bits[(*bit_index)++] << 2);
                new_packet->type |= (packet_bits[(*bit_index)++] << 1);
                new_packet->type |= (packet_bits[(*bit_index)++]);
                if(new_packet->type == PT_LITERAL) {
                    state = PS_LITERAL;
                } else {
                    state = PS_LENGTH_TYPE;
                }
                break;
            case PS_LITERAL: 
                {
                    int should_continue = packet_bits[(*bit_index)++];
                    new_packet->literal_value <<= 4;
                    new_packet->literal_value |= packet_bits[(*bit_index)++] << 3;
                    new_packet->literal_value |= packet_bits[(*bit_index)++] << 2;
                    new_packet->literal_value |= packet_bits[(*bit_index)++] << 1;
                    new_packet->literal_value |= packet_bits[(*bit_index)++];
                    if(!should_continue) {
                        print_packet(new_packet, starting_bit);
                        state = PS_DONE;
                    }
                }
                break;
            case PS_LENGTH_TYPE:
                new_packet->length_type = packet_bits[(*bit_index)++];
                state = PS_SUBPACKET_LENGTH;
                break;
            case PS_SUBPACKET_LENGTH:
                if(new_packet->length_type == 0) {
                    for(int b = 0; b < BITS_SUBPACKET_LENGTH_TYPE_0; b++) {
                        new_packet->subpacket_length <<= 1;
                        new_packet->subpacket_length |= packet_bits[(*bit_index)++];                                           
                    }
                } else {
                    for(int b = 0; b < BITS_SUBPACKET_LENGTH_TYPE_1; b++) {
                        new_packet->subpacket_length <<= 1;
                        new_packet->subpacket_length |= packet_bits[(*bit_index)++];       
                    }
                }
                state = PS_SUBPACKETS;
                break;
            case PS_SUBPACKETS:
                {
                    if(new_packet->length_type == 0) {  // Parse a fixed number of bits
                        int subpacket_bits = new_packet->subpacket_length;
                        while(subpacket_bits != 0) {
                            int old_index = *bit_index;
                            new_packet->subpackets[new_packet->subpacket_count] = parse_packet(bit_index);
                            subpacket_bits -= (*bit_index - old_index);
                            new_packet->subpacket_count++;
                        }
                    } else { // Parse a fixed number of subpackets
                        int num_packets = new_packet->subpacket_length;
                        new_packet->subpacket_count = 0;
                        while(num_packets != 0) {
                            new_packet->subpackets[new_packet->subpacket_count] = parse_packet(bit_index);

                            num_packets--;
                            new_packet->subpacket_count++;
                        }
                    }

                    if(all_subpackets_are_literals(new_packet)) {
                        new_packet->literal_value = process_packet(new_packet);
                        new_packet->type = PT_LITERAL;
                    }

                    state = PS_DONE;
                }
                break;
        }
    }

    return new_packet;
}