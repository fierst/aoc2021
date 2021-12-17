#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// we can't have a packet shorter than 16 bits!
#define MIN_PACKET_LENGTH 16

#define SUBPACKET_LENGTH_TYPE_0 15
#define SUBPACKET_LENGTH_TYPE_1 11

// Certainly dumber ways to do this...
const uint8_t ascii_to_bin[128] = 
{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0 ,
   0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  
typedef enum {
    VERSION,
    TYPE,
    LENGTH_TYPE,
    SUBPACKET_LENGTH,
    LITERAL,
    SUBPACKETS,
    DONE
} parse_state_t;

// A big ol' array for the bits
uint8_t bits[__UINT16_MAX__];
uint16_t packet_length = 0;

int version_sum = 0;

int next_packet_start_index(int start_index);

int main(int argc, char const *argv[])
{
    char * line = NULL;
    size_t len;
    ssize_t read;
    
    while(read = getline(&line, &len, stdin) != EOF) {
        version_sum = 0;

        int bi = 0;

        char* p = line;
        
        while(*p != '\n' && *p != '\0') {
            char c = *p;
            printf("%c", c);
            bits[bi++] = ascii_to_bin[c] & 0b1000 ? 1 : 0;
            bits[bi++] = ascii_to_bin[c] & 0b0100 ? 1 : 0;
            bits[bi++] = ascii_to_bin[c] & 0b0010 ? 1 : 0;
            bits[bi++] = ascii_to_bin[c] & 0b0001 ? 1 : 0;
            p++;
        }
        
        printf("\n");
        for(int i = 0; i < bi; i++) {
            printf("%d", bits[i]);
        }
        printf("\n");

        packet_length = bi;

        int npsi = 0;

        while(packet_length - npsi > MIN_PACKET_LENGTH) {
            npsi = next_packet_start_index(npsi);
        }

        printf("%d\n\n", version_sum);
    }
    
    free(line);
    return 0;
}

int next_packet_start_index(int start_index) {
    int i = start_index;

    parse_state_t state = VERSION;

    int version = 0;
    int type = 0;
    int literal = 0;
    int subpacket_length = 0;
    int length_type = 0;

    while(state != DONE) {
        switch(state) {
            case VERSION:
                version |= (bits[i++] << 2);
                version |= (bits[i++] << 1);
                version |= (bits[i++]);
                state = TYPE;
                break;
            case TYPE:
                type |= (bits[i++] << 2);
                type |= (bits[i++] << 1);
                type |= (bits[i++]);
                if(type == 4) {
                    state = LITERAL;
                } else {
                    state = LENGTH_TYPE;
                }
                break;
            case LITERAL: 
                {
                    int should_continue = bits[i++];
                    literal <<= 4;
                    literal |= bits[i++] << 3;
                    literal |= bits[i++] << 2;
                    literal |= bits[i++] << 1;
                    literal |= bits[i++];
                    if(!should_continue) {
                        state = DONE;
                    }
                }
                break;
            case LENGTH_TYPE:
                length_type = bits[i++];
                state = SUBPACKET_LENGTH;
                break;
            case SUBPACKET_LENGTH:
                if(length_type == 0) {
                    for(int b = 0; b < SUBPACKET_LENGTH_TYPE_0; b++) {
                        subpacket_length <<= 1;
                        subpacket_length |= bits[i++];                                           
                    }
                } else {
                    for(int b = 0; b < SUBPACKET_LENGTH_TYPE_1; b++) {
                        subpacket_length <<= 1;
                        subpacket_length |= bits[i++];       
                    }
                }
                state = SUBPACKETS;
                break;
            case SUBPACKETS:
                {
                    // Parse a fixed number of bits
                    if(length_type == 0) {
                        int npsi = next_packet_start_index(i);
                        i = npsi;
                        subpacket_length -= npsi;
                    } else { // Parse a fixed number of subpackets
                        while(subpacket_length != 0) {
                            i = next_packet_start_index(i);
                            subpacket_length--;
                        }
                    }
                   
                    state = DONE;
                }
                break;
        }
    }

    version_sum += version;

    return i;
}
