#include "CRC8.h"

// Function to calculate CRC8
unsigned char Calc_CRC8(unsigned char inData, unsigned char seed) {
    unsigned char bitsLeft;
    unsigned char temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--) {
        temp = (seed ^ inData) & 0x01;
        if (temp == 0) {
            seed >>= 1;
        } else {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        inData >>= 1;
    }
    return seed;
}