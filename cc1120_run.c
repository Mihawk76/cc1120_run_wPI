	/*
	* SPI testing utility (using spidev driver)
	*
	* Copyright (c) 2007  MontaVista Software, Inc.
	* Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
	*
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 2 of the License.
	*
	* Cross-compile with cross-gcc -I/path/to/cross-kernel/include
	*/
	
#include <wiringPi.h>  
#include <wiringPiSPI.h>  
#include <stdio.h>    
#include <stdlib.h>    
#include <stdint.h>  
#include <string.h>  
#include <time.h>
	
#include "cc112x_easy_link_reg_config.h"
#include "mac_address.c"
	
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
	
	#define ISR_ACTION_REQUIRED 1
	#define ISR_IDLE            0
	#define RX_FIFO_ERROR       0x11
	
	#define CC1120_RST  31
	#define CC1120_SSEL 5
	#define CC1120_MOSI 12
	#define CC1120_MISO 13
	#define CC1120_SCLK 14
	

	
	#define VCDAC_START_OFFSET 2
	#define FS_VCO2_INDEX 0
	#define FS_VCO4_INDEX 1
	#define FS_CHP_INDEX 2

uint8_t txBuffer[136];
uint8_t rxBuffer[136];
uint16_t packetCounter = 0;
uint8_t scan_key = 0x00;
uint8_t add_type = 0x02; //add type as new node
uint8_t index_node = 0x00; //temp index node
uint8_t wakeup_hold = 0x05; //wake up hold in 100ms
uint16_t cc1120_TH_ID;
uint16_t cc1120_TH_ID_Selected[10] = { 0x0000, 0x0926};
int cc1120_TH_Listed = 2;
uint8_t cc1120_TH_Node;
uint16_t gateway_ID;

	
/*******************************************************************************
 * @fn          trxReadWriteBurstSingle
 *
 * @brief       When the address byte is sent to the SPI slave, the next byte
 *              communicated is the data to be written or read. The address
 *              byte that holds information about read/write -and single/
 *              burst-access is provided to this function.
 *
 *              Depending on these two bits this function will write len bytes to
 *              the radio in burst mode or read len bytes from the radio in burst
 *              mode if the burst bit is set. If the burst bit is not set, only
 *              one data byte is communicated.
 *
 *              NOTE: This function is used in the following way:
 *
 *              TRXEM_SPI_BEGIN();
 *              while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
 *              ...[Depending on type of register access]
 *              trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
 *              TRXEM_SPI_END();
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      int
 */

static int trxReadWriteBurstSingle(uint8_t addr, uint8_t *pData, uint16_t len)
{
  int ret;
  uint8_t temp[len];

  /* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
  if(addr&RADIO_READ_ACCESS)
  {
    memset(temp,0x00,len);
	wiringPiSPIDataRW(0, pData, len);
	ret = pData[len-1];
  }
  else
  {
	memcpy(temp,pData,len);
	wiringPiSPIDataRW(0, temp, len);
	ret = temp[len-1];
  }
  
  
  return ret;
}
	
	
uint8_t trxSpiCmdStrobe(uint8_t cmd)
{
  
  /* Pull CS_N low */
  digitalWrite(CC1120_SSEL,  LOW) ; 
  
  uint8_t tx[] = {
	cmd, 0,
  };
     
  wiringPiSPIDataRW(0, tx, ARRAY_SIZE(tx)); 
  
 
  /* Pull CS_N high */
  digitalWrite(CC1120_SSEL,  HIGH) ; 

  /* return the status byte value */
  return(tx[1]);
}


/*******************************************************************************
 * @fn          trx8BitRegAccess
 *
 * @brief       This function performs a read or write from/to a 8bit register
 *              address space. The function handles burst and single read/write
 *              as specfied in addrByte. Function assumes that chip is ready.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       addrByte - address byte of register.
 * @param       pData    - data array
 * @param       len      - Length of array to be read(TX)/written(RX)
 *
 * output parameters
 *
 * @return      chip status
 */
uint8_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len)
{
  uint8_t d = accessType|addrByte;
  uint8_t tx[] = {d};

  /* Pull CS_N low*/
  digitalWrite(CC1120_SSEL,  LOW) ;

  wiringPiSPIDataRW(0, tx, ARRAY_SIZE(tx));
  trxReadWriteBurstSingle( d, pData, len);
 
  /* Pull CS_N high */
  digitalWrite(CC1120_SSEL,  HIGH) ;
  
  /* return the status byte value */
  return pData[0];
}

/******************************************************************************
 * @fn          trx16BitRegAccess
 *
 * @brief       This function performs a read or write in the extended adress
 *              space of CC112X.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       extAddr - Extended register space address = 0x2F.
 * @param       regAddr - Register address in the extended address space.
 * @param       *pData  - Pointer to data array for communication
 * @param       len     - Length of bytes to be read/written from/to radio
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t trx16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len)
{
  /* Pull CS_N low*/
  digitalWrite(CC1120_SSEL,  LOW) ;
 
  uint8_t d = accessType|extAddr;
  uint8_t tx[] = {d, regAddr};

  wiringPiSPIDataRW(0, tx, ARRAY_SIZE(tx));
  trxReadWriteBurstSingle( d, pData, len);

 
  /* Pull CS_N high*/
  digitalWrite(CC1120_SSEL,  HIGH) ;

  /* return the status byte value */
  return pData[0];
}



/******************************************************************************
 * @fn          cc112xSpiReadReg
 *
 * @brief       Read value(s) from config/status/extended radio register(s).
 *              If len  = 1: Reads a single register
 *              if len != 1: Reads len register values in burst mode 
 *
 * input parameters
 *
 * @param       addr   - address of first register to read
 * @param       *pData - pointer to data array where read bytes are saved
 * @param       len   - number of bytes to read
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiReadReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
  uint8_t tempExt  = (uint8_t)(addr>>8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc=0;
  
  /* Checking if this is a FIFO access -> returns chip not ready  */
  if((CC112X_SINGLE_TXFIFO<=tempAddr)&&(tempExt==0)) return STATUS_CHIP_RDYn_BM;
  
  /* Decide what register space is accessed */
  if(!tempExt)
  {
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),tempAddr,pData,len);
  }
  else if (tempExt == 0x2F)
  {
    rc = trx16BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),tempExt,tempAddr,pData,len);
  }
  return (rc);
}

/******************************************************************************
 * @fn          cc112xSpiWriteReg
 *
 * @brief       Write value(s) to config/status/extended radio register(s).
 *              If len  = 1: Writes a single register
 *              if len  > 1: Writes len register values in burst mode 
 *
 * input parameters
 *
 * @param       addr   - address of first register to write
 * @param       *pData - pointer to data array that holds bytes to be written
 * @param       len    - number of bytes to write
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiWriteReg(uint16_t addr, uint8_t *pData, uint8_t len)
{
  uint8_t tempExt  = (uint8_t)(addr>>8);
  uint8_t tempAddr = (uint8_t)(addr & 0x00FF);
  uint8_t rc=0;
  
  /* Checking if this is a FIFO access - returns chip not ready */
  if((CC112X_SINGLE_TXFIFO<=tempAddr)&&(tempExt==0)) return STATUS_CHIP_RDYn_BM;
  	
  /* Decide what register space is accessed */  
  if(!tempExt)
  {
    rc = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),tempAddr,pData,len);
  }
  else if (tempExt == 0x2F)
  {
    rc = trx16BitRegAccess((RADIO_BURST_ACCESS|RADIO_WRITE_ACCESS),tempExt,tempAddr,pData,len);
  }
  return (rc);
}



/*******************************************************************************
*   @fn         cc112xregConfig
*
*   @brief      Write register settings as given by SmartRF Studio found in
*               cc112x_easy_link_reg_config.h
*
*   @param      freq - frequency channel (0-80)
*
*   @return     none
*/
#define CC112X_FREQ_BASE 0x00668000l   // frequency base is 410Mhz
#define CC112X_FREQ_STEP 0x4000        // frequency step is 1Mhz

void cc112xregConfig(uint8_t freq) {

    uint8_t writeByte;
    uint16_t i;
	uint32_t freq_code;
	freq_code = CC112X_FREQ_BASE + ((uint16_t)freq * CC112X_FREQ_STEP);// original code

    // Reset radio
    trxSpiCmdStrobe(CC112X_SRES);
    
	delay(100);
	
	trxSpiCmdStrobe(CC112X_SRES);
	
    // Write registers to radio
	for(i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) {
		writeByte = preferredSettings[i].data;
		if (preferredSettings[i].addr == CC112X_FREQ2) writeByte = (freq_code >> 16) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ1) writeByte = (freq_code >> 8) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ0) writeByte = freq_code & 0xff;
		cc112xSpiWriteReg(preferredSettings[i].addr, &writeByte, 1);
    }
}

/*******************************************************************************
*   @fn         registerConfigChek
*
*   @brief      Write register settings as given by SmartRF Studio found in
*               cc112x_easy_link_reg_config.h
*
*   @param      none
*
*   @return     int (0: all registers config match, 1: one or more registers config didn't match)
*/
int cc112xConfigChek(uint8_t freq) {

    uint8_t readByte; 
	  uint8_t cmpByte;
	  //buf[20];
    uint16_t i;
   	uint32_t freq_code;
	
    freq_code = CC112X_FREQ_BASE + ((uint16_t)freq * CC112X_FREQ_STEP);


    // Read registers to radio
    for(i = 0; i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) {
        readByte = 0x00;
		cmpByte = preferredSettings[i].data;
        cc112xSpiReadReg(preferredSettings[i].addr, &readByte, 1);
		if (preferredSettings[i].addr == CC112X_FREQ2) cmpByte = (freq_code >> 16) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ1) cmpByte = (freq_code >> 8) & 0xff;
		else if (preferredSettings[i].addr == CC112X_FREQ0) cmpByte = freq_code & 0xff;
		if (readByte != cmpByte) return 1;
    }
	return 0;
	
}




/*******************************************************************************
*   @fn         manualCalibration
*
*   @brief      Calibrates radio according to CC112x errata
*
*   @param      none
*
*   @return     none
*/
#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2

static void manualCalibration(void) {

    uint8_t original_fs_cal2;
    uint8_t calResults_for_vcdac_start_high[3];
    uint8_t calResults_for_vcdac_start_mid[3];
    uint8_t marcstate;
    uint8_t writeByte;

    // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 2) Start with high VCDAC (original VCDAC_START + 2):
    cc112xSpiReadReg(CC112X_FS_CAL2, &original_fs_cal2, 1);
    writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 3) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
		printf("marcstate: %02x\r\n", marcstate);

    } while (marcstate != 0x41);


    // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with 
    //    high VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2,
                     &calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

    // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 6) Continue with mid VCDAC (original VCDAC_START):
    writeByte = original_fs_cal2;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 7) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
		printf("marcstate: %02x\r\n", marcstate);
    } while (marcstate != 0x41);

    // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained 
    //    with mid VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2, 
                     &calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

    // 9) Write back highest FS_VCO2 and corresponding FS_VCO
    //    and FS_CHP result
    if (calResults_for_vcdac_start_high[FS_VCO2_INDEX] >
        calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) {
        writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    } else {
        writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    }
}

/*******************************************************************************
 * @fn          cc112xSpiWriteTxFifo
 *
 * @brief       Write pData to radio transmit FIFO.
 *
 * input parameters
 *
 * @param       *pData - pointer to data array that is written to TX FIFO
 * @param       len    - Length of data array to be written
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiWriteTxFifo(uint8_t *pData, uint8_t len)
{
  uint8_t rc;
  rc = trx8BitRegAccess(0x00,CC112X_BURST_TXFIFO, pData, len);
  return (rc);
}

/*******************************************************************************
 * @fn          cc112xSpiReadRxFifo
 *
 * @brief       Reads RX FIFO values to pData array
 *
 * input parameters
 *
 * @param       *pData - pointer to data array where RX FIFO bytes are saved
 * @param       len    - number of bytes to read from the RX FIFO
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
uint8_t cc112xSpiReadRxFifo(uint8_t * pData, uint8_t len)
{
  uint8_t rc;
  rc = trx8BitRegAccess(0x00,CC112X_BURST_RXFIFO, pData, len);
  return (rc);
}

/******************************************************************************
 * @fn      cc112xGetTxStatus(void)
 *          
 * @brief   This function transmits a No Operation Strobe (SNOP) to get the 
 *          status of the radio and the number of free bytes in the TX FIFO.
 *          
 *          Status byte:
 *          
 *          ---------------------------------------------------------------------------
 *          |          |            |                                                 |
 *          | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (free bytes in the TX FIFO |
 *          |          |            |                                                 |
 *          ---------------------------------------------------------------------------
 *
 *
 * input parameters
 *
 * @param   none
 *
 * output parameters
 *         
 * @return  rfStatus_t 
 *
 */
uint8_t cc112xGetTxStatus(void)
{
    return(trxSpiCmdStrobe(CC112X_SNOP));
}

/******************************************************************************
 *
 *  @fn       cc112xGetRxStatus(void)
 *
 *  @brief   
 *            This function transmits a No Operation Strobe (SNOP) with the 
 *            read bit set to get the status of the radio and the number of 
 *            available bytes in the RXFIFO.
 *            
 *            Status byte:
 *            
 *            --------------------------------------------------------------------------------
 *            |          |            |                                                      |
 *            | CHIP_RDY | STATE[2:0] | FIFO_BYTES_AVAILABLE (available bytes in the RX FIFO |
 *            |          |            |                                                      |
 *            --------------------------------------------------------------------------------
 *
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 * @return    rfStatus_t 
 *
 */
uint8_t cc112xGetRxStatus(void)
{
    return(trxSpiCmdStrobe(CC112X_SNOP | RADIO_READ_ACCESS));
}

/******************************************************************************
 *
 *  @fn       cc112x_hw_rst(void)
 *
 *  @brief   
 *            Reset cc112x by reset pin.
 *            
 *
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 * @return    none
 *
 */
void cc112x_hw_rst(void) {
  digitalWrite(CC1120_RST,  LOW) ;
  delay(50);  
  digitalWrite(CC1120_RST,  HIGH) ;
}

/******************************************************************************
 *
 *  @fn       cc112xpwrConfig(uint8_t pwrTx)
 *
 *  @brief   
 *            Set Tx Power
 *            
 *
 *
 * input parameters
 *
 * @param     pwrTx - Tx power value
 *
 * output parameters
 *         
 * @return    none
 *
 */
void cc112xpwrConfig(uint8_t pwrTx) {
    uint8_t writeByte;
	writeByte = pwrTx;
    cc112xSpiWriteReg(CC112X_PA_CFG2, &writeByte, 1);
}

/******************************************************************************
 *
 *  @fn       uint8_t wait_exp_val(uint16_t addr, uint8_t exp_val)
 *
 *  @brief   
 *            Waiting until register value and expected value is match
 *            
 *
 * input parameters
 *
 * @param     addr    - register address
 * @param     exp_val - expected value
 *
 * output parameters
 *         
 *
 * @return   register value;
 *
 */
uint8_t wait_exp_val(uint16_t addr, uint8_t exp_val) {
	uint8_t temp_byte;
	do {
		cc112xSpiReadReg(addr, &temp_byte, 1);
	} while (temp_byte != exp_val);
	return temp_byte;
}


/******************************************************************************
 *
 *  @fn       cc112x_init(uint8_t freq, uint8_t pwr)
 *
 *  @brief   
 *            Initialize/configure CC112x with given freq channel & tx power
 *            
 *
 * input parameters
 *
 * @param     freq    - frequency channel
 * @param     pwr     - tx power
 *
 * output parameters
 *         
 *
 * @return   none
 *
 */
void  cc112x_init(uint8_t freq, uint8_t pwr)
{	
	uint16_t ii;
	ii = 0;
	do {
		cc112xregConfig(freq);
		if (( ii++ % 5) == 0) {
			/*** CC112X HW Reset ***/
			cc112x_hw_rst();
			printf("CC1120 HW Reset.\r\n");
			
		}
		if ( ii > 15 ) {
		  printf("CC1120 can not init.\r\n");
		  exit(1);
		}
		
	} while (cc112xConfigChek(freq));
	
	if (pwr>0x3e) pwr = 0x3e;
	cc112xpwrConfig(0x7f-pwr);
	
	manualCalibration();

	memset(txBuffer,0,sizeof(txBuffer));
	memset(rxBuffer,0,sizeof(rxBuffer));
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	
	// Flush RX FIFO
	trxSpiCmdStrobe(CC112X_SFRX);
	
	// Flush TX FIFO
	trxSpiCmdStrobe(CC112X_SFTX);

	wait_exp_val(CC112X_MARCSTATE, 0x41);
	
	// Set radio in RX
	trxSpiCmdStrobe(CC112X_SRX);
			
}

/******************************************************************************
 *
 *  @fn       uint8_t send_packet(uint8_t * sendBuffer)
 *
 *  @brief   
 *            Send packet data from given buffer (sendBuffer)
 *			  the first byte of sendBuffer is data length
 *            data length must > 0
 *
 * input parameters
 *
 * @param     sendBuffer   - Buffer of data send
 *
 * output parameters
 *         
 *
 * @return   send result status, 0 = success, another value = fail
 *
 */
uint8_t send_packet(uint8_t * sendBuffer) {
	uint8_t temp_byte;
	uint8_t len;
	len = *sendBuffer;
	if (len == 0) return 0;
	
	cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);
	if (temp_byte != 0x6d) 
		return 0;
	
	cc112xSpiReadReg(CC112X_RSSI0, &temp_byte, 1);
	if (temp_byte & 0x40) {
		len =  0; // if RSSI present from another 
		return 0;
	}
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	wait_exp_val(CC112X_MARCSTATE, 0x41);

	trxSpiCmdStrobe(CC112X_SFTX);					/*  SFTX    - Flush the TX FIFO buffer. */	
	trxSpiCmdStrobe(CC112X_SFRX);					/*  SFRX    - Flush the RX FIFO buffer. */

	temp_byte = 0x06;
	cc112xSpiWriteReg(CC112X_IOCFG2, &temp_byte, 1);
	
	// Write packet to TX FIFO
	cc112xSpiWriteTxFifo(sendBuffer, len+1);

	// Strobe TX to send packet
	trxSpiCmdStrobe(CC112X_STX);
	do {
		//os_dly_wait (1);
		cc112xSpiReadReg(CC112X_MARC_STATUS1, &temp_byte, 1);
		//printf("marcstate: %02x\r\n", marcState);
	} while (( temp_byte) == 0);
	
	if ( temp_byte & 0x40) *sendBuffer = 0;
	return temp_byte;
}


/******************************************************************************
 *
 *  @fn       cc112x_run(void)
 *
 *  @brief   
 *            Runs as service, this function will handle data Tx & Rx
 *			  received data will store to rxBuffer
 *            and txBuffer for send data
 *
 * input parameters
 *
 * @param     none
 *
 * output parameters
 *         
 *
 * @return   none
 *
 */
void cc112x_run(void)
{
	uint8_t temp_byte;
	int i;
	uint8_t rx_byte = 0;
		// Infinite loop
	
	cc112xSpiReadReg(CC112X_MARC_STATUS1, &temp_byte, 1);
	if (( temp_byte == 0x07)||( temp_byte == 0x08)){
		// Flush TX FIFO
		trxSpiCmdStrobe(CC112X_SFTX);
	}
	else if (( temp_byte == 0x09)||( temp_byte == 0x0a)){
		// Flush RX FIFO
		trxSpiCmdStrobe(CC112X_SFRX);
	}
	else if( temp_byte&0x80 ) {
		// Read number of bytes in RX FIFO
		cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rx_byte, 1);

		// Check that we have bytes in FIFO
		if(rx_byte != 0) {

			// Read MARCSTATE to check for RX FIFO error
			cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);

			// Mask out MARCSTATE bits and check if we have a RX FIFO error
			if((temp_byte & 0x1F) == RX_FIFO_ERROR) {
				// Flush RX FIFO
				trxSpiCmdStrobe(CC112X_SFRX);
			} 
			else {
				rx_byte &= 0x7F;
				// Read n bytes from RX FIFO
				cc112xSpiReadRxFifo(rxBuffer, rx_byte);

				// Check CRC ok (CRC_OK: bit7 in second status byte)
				// This assumes status bytes are appended in RX_FIFO
				// (PKT_CFG1.APPEND_STATUS = 1)
				// If CRC is disabled the CRC_OK field will read 1
				temp_byte = 0;
				if(rxBuffer[rx_byte - 1] & 0x80) {
					// Update packet counter
					time_t t;
					time (&t);
					struct tm * timeinfo = localtime (&t);
					//struct tm * timeinfo = gmtime (&t);
					packetCounter++;
					printf("%05d: %04d-%02d-%02d %02d:%02d:%02d - Received %d %s RSSI=%d.\r\n", 
					packetCounter,
					timeinfo->tm_year+1900,
					timeinfo->tm_mon+1,
					timeinfo->tm_mday,
					timeinfo->tm_hour,
					timeinfo->tm_min,
					timeinfo->tm_sec,
					//timeinfo->tm_ms,
					rx_byte, (rx_byte>1) ? "bytes" : "byte",rxBuffer[rx_byte - 2]-102);
				  int counter = 0;
					while ( counter < cc1120_TH_Listed )
					{
						if ( 1/*cc1120_TH_ID_Selected[counter] == (*(uint16_t*)&rxBuffer[2])*/)
						{	
							printf( "counter:%d TH_ID_Selected:%04X\n", counter, cc1120_TH_ID_Selected[counter]);
							if ( rxBuffer[1] == 0x81 )
							{
							printf ("Joint detected\n");
								cc1120_TH_ID = *(uint16_t*)&rxBuffer[2];
								printf("cc1120_TH_ID is %04X\n", cc1120_TH_ID);
								cc1120_TH_Node = rxBuffer[6];
							if ( rxBuffer[7] != scan_key ){
								printf(" Scan key is diffrent \n old : new , %02x : %02x \nCommencing scan command\n", rxBuffer[7], scan_key);
								//cc1120_TH_ID = *(uint16_t*)&rxBuffer[2];
								//cc1120_TH_Node = rxBuffer[6];
								printf("TH id is %04X \n Node is %02X \n",cc1120_TH_ID, cc1120_TH_Node);
								txBuffer[0] = 0x0A; txBuffer[1] = 0x01; *(uint16_t*)&txBuffer[2] =  gateway_ID; *(uint16_t*)&txBuffer[4] = cc1120_TH_ID;
								txBuffer[6] = cc1120_TH_Node; txBuffer[7] = scan_key; txBuffer[8] = 0x00; txBuffer[9] = 0x00; txBuffer[10] = 0x00;  
								}
								if ( rxBuffer[7] == scan_key ){
									printf("Scan key is the same %02X:%02X\n", rxBuffer[7], scan_key);
									printf("Commencing add command\n");
									txBuffer[0] = 0x0A; txBuffer[1] = 0x06; *(uint16_t*)&txBuffer[2] =  gateway_ID; 
									*(uint16_t*)&txBuffer[4] = cc1120_TH_ID; txBuffer[6] = cc1120_TH_Node; 
									txBuffer[7] = add_type; txBuffer[8] = index_node; txBuffer[9] = scan_key; txBuffer[10] = wakeup_hold;  
								} 
							}
							if ( rxBuffer[1] == 0x92 )
							{
								printf("Th data detected\n");
								cc1120_TH_ID = *(uint16_t*)&rxBuffer[2];
								cc1120_TH_Node = rxBuffer[6];
								txBuffer[0] = 0x07; txBuffer[1] = 0x11; *(uint16_t*)&txBuffer[2] =  gateway_ID; 
								*(uint16_t*)&txBuffer[4] = cc1120_TH_ID; txBuffer[6] = cc1120_TH_Node; 
								txBuffer[7] = 0x00;  
								printf("Hummidity : %d Temp 1 : %d Temp2 : %d Temp 3 : %d\n", 
								*(uint16_t*)&rxBuffer[7], *(uint16_t*)&rxBuffer[9], *(uint16_t*)&rxBuffer[11], *(uint16_t*)&rxBuffer[13]); 
							}
					for (i=0;i<rx_byte;i++) {
						printf("%02X ", rxBuffer[i]);
					}
					printf ("txbuffer ");
					for (i=0;i<=11;i++) {
						printf("%02X ", txBuffer[i]);
					}
					printf("\r\n");
						break;		
					}
						if ( cc1120_TH_ID_Selected[counter] != (*(uint16_t*)&rxBuffer[2])){
							printf( "counter:%d TH_ID_Selected:%04X\n", counter, cc1120_TH_ID_Selected[counter]);
							printf("TH_ID_selected:TH_Incoming_Id %04X:%04X different \n",cc1120_TH_ID_Selected[counter] ,(*(uint16_t*)&rxBuffer[2]));
							}	
						counter++;
					}
					//processPacket((uint8_t*)&rxBuffer[1], txBuffer, rx_byte-3);
					
				}
			}
		}
	}
	else if( temp_byte == 0) {
					//os_dly_wait (1);
//		if (replyDly>0) return;
		send_packet(txBuffer);
		return;
	}
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	
	wait_exp_val(CC112X_MARCSTATE, 0x41);
		
			
    // Set radio back in RX
    trxSpiCmdStrobe(CC112X_SRX);

}



/*******************************************************************************
********************************************************************************/

int main(int argc, char *argv[]) {
  uint8_t DUMMY_BUF[]={1,2,3,4,5,6,7,8,9,0};
  int ret = 0;
  gateway_ID = mac_address();
 
  //setup gpio pin to spi function
  wiringPiSetup();
  
  pinMode(CC1120_MOSI, SPI_PIN);
  pinMode(CC1120_MISO, SPI_PIN);
  pinMode(CC1120_SCLK, SPI_PIN);
  pinMode(CC1120_SSEL, OUTPUT) ;  
  pinMode(CC1120_RST, OUTPUT) ;
  
  //getchar();
  wiringPiSPISetup(0, 5000000); //32*1000*1000);  
  digitalWrite(CC1120_SSEL,  HIGH) ;

  cc112x_hw_rst();
  cc112x_init(0,0);// freq 410 Mhz + (1 Mhz * 0)
	memcpy(&txBuffer[1],DUMMY_BUF,10);
	txBuffer[0]=10;
	while (1)
	{
		/* communication handler */
		cc112x_run();
	}
  /*int repeat;
	for(repeat=0;repeat<=100;repeat++)
	{
  // init cc112x radio
  cc112x_init(repeat,0);// freq 410 Mhz + (1 Mhz * repeat)

  printf("Run CC1120..\r\n");
  //getchar();

  // settup base frequency
    
	memcpy(&txBuffer[1],DUMMY_BUF,10);
	txBuffer[0]=10;
	int comm = 0;
	while(comm<6) {
				
		cc112x_run();
		comm++;
		sleep (60);
	}
	int frekuensi = (410 + repeat);
	printf("No File found frek = %d\n", frekuensi);  	
	}*/
  return ret;
	
  }
