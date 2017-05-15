#define _GNU_SOURCE 1
#include <inttypes.h>
#include <math.h>
#include <wiringPi.h>  
#include <wiringPiSPI.h>  
#include <stdio.h>   
#include <stdlib.h>    
#include <stdint.h>  
#include <string.h>  
#include <time.h>

#include "cc1120.h"
#include "app.h"

CC120_BUFFER hi_priority_tx;
CC120_BUFFER lo_priority_tx;


void flush_buffer(CC120_BUFFER *buf) {
	buf->head = 0;
	buf->tail = 0;
	memset((uint8_t*)buf->rdata, 0, sizeof(RADIO_DATA));
}

/*
char udp_debug_buff[500];

int udp_debug_printf(const char *fmt, ...)
{
	int err;
	va_list ap;
	va_start(ap, fmt);
#ifdef UDP_DEBUG
	err = vsprintf(&udp_debug_buff[strlen(udp_debug_buff)], fmt, ap);
#endif
	va_end(ap);
	return err;
}
*/

uint8_t inc_ptr (uint8_t ptr, uint8_t buff_len) {
	uint8_t ptrx = ptr+1;
	if (ptrx >= buff_len) ptrx = 0;
	return ptrx;
}

// return 1 if free otherwise 0
uint8_t next_buffer_free(CC120_BUFFER *buf) {
	uint8_t h = inc_ptr(buf->head,RDATA_COUNT);
	if (h == buf->tail) return 0;
	return 1;
}

/*
   - if force flag = '1' (set), always write data to bufer even the buffer full
	   otherwise if the flag = '0' (not set)
	 - return : head_ptr with forced flag
	 
*/
uint8_t put_to_buffer (CC120_BUFFER *buf, uint8_t * buf_in, uint8_t len, uint8_t force) {
	
	uint8_t head;
	uint8_t ret;
	
	ret = next_buffer_free(buf) ? 1 : 0;
	
	if (ret || force) {
	  head = inc_ptr(buf->head,RDATA_COUNT);
	  memcpy(buf->rdata[buf->head].buf,buf_in, len);
	  buf->rdata[buf->head].len = len;
	  buf->head = head;
	  if (ret==0) buf->tail = inc_ptr(buf->tail,RDATA_COUNT);
	}
	
	return (buf->head|ret);
}

/*
	 - return : 0 = buffer empty, other wise length of buffer
*/
uint8_t get_from_buffer (CC120_BUFFER *buf, uint8_t * buf_out) {
	
	uint8_t tail;
	uint8_t ret = 0;
	
	tail = buf->tail;
	while (tail!=buf->head) {
		if (buf->rdata[buf->tail].len != 0) break;
	  tail = inc_ptr(tail,RDATA_COUNT);
	} 
  
	if (tail == buf->head) {
		buf->tail = tail;
		return ret;
	}
	
  memcpy(buf_out, buf->rdata[tail].buf, buf->rdata[tail].len);
	ret = buf->rdata[tail].len;
  buf->rdata[tail].len = 0;
  buf->tail = inc_ptr(tail,RDATA_COUNT);
	
	return ret;
}

