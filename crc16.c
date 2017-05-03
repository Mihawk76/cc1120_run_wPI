#include <inttypes.h>
#include "crc16.h"

uint32_t crc16(uint8_t *buffer, uint16_t tcrc_init, uint16_t buffer_length)
{
    uint8_t crc_hi;
    uint8_t crc_lo;
    uint8_t i; /* will index into CRC lookup */
	
	
	  crc_hi = tcrc_init & 0xFF; /* high CRC byte initialized */
    crc_lo = (tcrc_init >> 8)& 0xFF; /* low CRC byte initialized */


    /* pass through message buffer */
    while ((buffer_length--)) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi | crc_lo << 8);
}