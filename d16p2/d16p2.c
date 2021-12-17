#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "packet.h"

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

int main(int argc, char const *argv[])
{
    char * line = NULL;
    size_t len;
    ssize_t read;
    
    while(read = getline(&line, &len, stdin) != EOF) {

        int bi = 0;

        char* p = line;
        
        while(*p != '\n' && *p != '\0') {
            char c = *p;
            packet_bits[bi++] = ascii_to_bin[c] & 0b1000 ? 1 : 0;
            packet_bits[bi++] = ascii_to_bin[c] & 0b0100 ? 1 : 0;
            packet_bits[bi++] = ascii_to_bin[c] & 0b0010 ? 1 : 0;
            packet_bits[bi++] = ascii_to_bin[c] & 0b0001 ? 1 : 0;
            p++;
        }
        
        packet_length = bi;

        int bit = 0;

        packet_t * packet = parse_packet(&bit);
        printf("%ld\n", packet->literal_value);
    }

    free(line);

    return 0;
}


