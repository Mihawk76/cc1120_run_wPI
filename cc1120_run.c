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
#include <syslog.h>
	
#include "cc112x_easy_link_reg_config.h"
#include "mac_address.c"
#include "kwh_params.c"
#include "res_sensor.c"
//#include "read_int.c"
	
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
	
#define TH_DATA_LENGTH 0x0c
#define TH_DATA_CODE   0xc1

#define TH_NODES_MAX 20;

typedef struct{
  uint8_t nodeId;
  uint8_t nodeType;
  uint8_t rssiRx;
  uint8_t rssiTx;
  uint8_t trap;
  uint8_t batt;
  uint8_t state;
  uint16_t humidity;
  uint16_t temp1;
  uint16_t temp2;
  uint16_t temp3;
  uint8_t din1;
  uint8_t din2;
  uint32_t counterRx;
  uint32_t counterTx;
  uint32_t counterTotal;
  uint32_t timestamp;
}WITH_STRUCT;

WITH_STRUCT wiTH_nodes[TH_NODES_MAX];

uint8_t txBuffer[256];
uint8_t rxBuffer[256];
uint16_t packetCounter = 0;
uint8_t scan_key = 0x00;
uint8_t add_type = 0x02; //add type as new node
uint8_t index_node = 0x00; //temp index node
uint8_t wakeup_hold = 0x05; //wake up hold in 100ms
uint16_t cc1120_TH_ID;
uint16_t cc1120_TH_ID_Selected[12] = { 1, 2, 3, 4, 5, 6 };
uint32_t cc1120_KWH_ID;
int cc1120_TH_Listed = 6;
uint8_t cc1120_TH_Node;
uint16_t gateway_ID;
uint16_t mac_address_gateway;
int freq_main;
uint8_t freq_th;
int kwh_loop = 0;
uint16_t humidity;
uint16_t temp1;
uint16_t temp2;
uint16_t temp3;
uint16_t dIn1;
uint16_t dIn2;
int16_t rssi = 0;
uint16_t past_temp[3];
int loop_temp=0;
//char* location = "http://35.160.141.229/post.php";
//char* location = "http://52.43.48.93/dcms/rest/alfa";
//char* location = "http://192.168.88.19:1616/dcms/rest/alfa";
//char* location = "http://10.42.0.1/post.php";
char* location = "http://35.160.141.229:3000/api/ths";
char* location_wattR = "http://35.160.141.229:3000/api/RWatts";
char* location_wattS = "http://35.160.141.229:3000/api/SWatts";
char* location_wattT = "http://35.160.141.229:3000/api/TWatts";
char* gateway_trap_id = "EM24010101";
FILE *f;
	
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
		//FILE *writeFile = fopen("/home/data.log", "a");
		//if (writeFile == NULL)
		//{
    	//printf("Error opening file!\n");
    	//exit(1);
		//}

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
		//fprintf(writeFile, "marcstate: %02x\r\n", marcstate);

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
		//fprintf(writeFile, "marcstate: %02x\r\n", marcstate);
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
		//fclose(writeFile);
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
	if (temp_byte & 0x04) {
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
// middle filter function
uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
 uint16_t middle;

 if ((a <= b) && (a <= c))
 {
   middle = (b <= c) ? b : c; 
 }
 else if ((b <= a) && (b <= c))
 {
   middle = (a <= c) ? a : c; 
 }
 else 
 {
   middle = (a <= b) ? a : b; 
 }
 return middle;
}

int processPacket(uint8_t *bufferin, uint8_t *bufferout, uint8_t len) {
    uint8_t temp_8;
	  uint8_t i,ii,z;
	  uint16_t temp_16, temp2_16;
	  uint32_t temp_32;
		uint8_t pktCmd;
		uint8_t srcAddr;
	
		uint32_t interval_between_node;
		uint32_t current_stamp;
    uint32_t next_stamp;
	
		int ret = 0;
	
	  pktCmd = *bufferin;
	  i = 0;
	  for (temp_8 = 0; temp_8 < (len-1); temp_8++) {
			i += bufferin[temp_8];
		}
	  if (i !=  bufferin[temp_8]) pktCmd = 0;
		if (len < 5) pktCmd = 0;
	  replyDly = 2;
	

		switch (pktCmd) {

			case	COMM_TH_TO_GW:
				//Tx.. Data
				//frame: CMD, NodeID, humidity, temp1, temp2, temp3, carry, rssirx, dinBat count,  cs
				//         1       1         1      1      1      1      1     1       0,5   2.5    1
				if(len!=12) break;
				
	      srcAddr = bufferin[1]; // get destination address	
	      				
			  i = srcAddr-1;
			  if (i>=TH_NODES_MAX) break;
			
			  wiTH_nodes[i].counterRx++;
			  wiTH_nodes[i].counterRx &= 0x000fffff;
			  
			  temp_32 = bufferin[8] & 0x0f;
			  temp_32 <<= 16;
			  memcpy((uint8_t*) &temp_32, (uint8_t*) &bufferin[9], 2);
	      wiTH_nodes[i].counterTotal = temp_32+1;
			  wiTH_nodes[i].counterTx &=0x000fffff;
			  if(wiTH_nodes[i].counterTotal<wiTH_nodes[i].counterTx) 
			    wiTH_nodes[i].counterTx = temp_32;
			
				// srcAddr found in the nodes table parse to collect data
				temp_8 = 0;
				temp_16 = bufferin[6] & 0xc0;
				temp_16 <<= 2;
				temp_16 += bufferin[2];
				temp_16 *= 10;
				
				wiTH_nodes[i].nodeType = temp_16&0xffff; //store Node Type
				
				if (temp_16<SENSOR_TONLY) temp_8 |= 0x02;			 // get humidity enable flag
				if (temp_16>=SENSOR_TONLY) temp_16=0x2a2a;
				temp2_16 = (wiTH_nodes[i].humidity&0x3fff);
				wiTH_nodes[i].humidity = temp_16&0x3fff ;      //store humidity value
				if (abs(temp2_16 - temp_16) >= 200) wiTH_nodes[i].trap |= 0x0002; 
				
				temp_16 = bufferin[6] & 0x30;
				temp_16 <<= 4;
				temp_16 += bufferin[3];
				temp_16 *= 10;
				if (temp_16<VALUE_INVALID) temp_8 |= 0x01;			 // get temp1 enable flag
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				temp2_16 = (wiTH_nodes[i].temp1&0x3fff);
				wiTH_nodes[i].temp1 = temp_16&0x3fff;            //store temp1 value
				if (abs(temp2_16 - temp_16) >= 200) wiTH_nodes[i].trap |= 0x0001; 
				
				temp_16 = bufferin[6] & 0x0C;
				temp_16 <<= 6;
				temp_16 += bufferin[4];
				temp_16 *= 10;
				if (temp_16<VALUE_INVALID) temp_8 |= 0x04;			 // get temp2 enable flag
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				temp2_16 = wiTH_nodes[i].temp2&0x3fff;
				wiTH_nodes[i].temp2 = temp_16&0x3fff;            //store temp2 value
				if (abs(temp2_16 - temp_16) >= 200) wiTH_nodes[i].trap |= 0x0004; 

				radio_node[i].rssi    = bufferin[len];
				radio_node[i].rssi_tx = bufferin[7];

				temp_16 = bufferin[6] & 0x03;
				temp_16 <<= 8;
				temp_16 += bufferin[5];
				temp_16 *= 10;
				if (temp_16<VALUE_INVALID) temp_8 |= 0x08;			 // get temp3 enable flag
				if (temp_16>=VALUE_INVALID) temp_16=0x2a2a;
				temp2_16 = wiTH_nodes[i].temp3&0x3fff;
				wiTH_nodes[i].temp3 = temp_16&0x3fff; //store temp3 value
				if (abs(temp2_16 - temp_16) >= 200) wiTH_nodes[i].trap |= 0x0008; 

				ii = *(uint8_t *)(bufferin+8);		// Get Batt Status
				ii >>= 4; 
				ii &= 0x03;
				wiTH_nodes[i].batt = ii; //Batt Status value

				ii = *(uint8_t *)(bufferin+8); // Get Din1 Status
				ii >>= 6;
				ii &= 0x01;
				if (wiTH_nodes[i].din1!=ii) wiTH_nodes[i].trap |= 0x0010;
				wiTH_nodes[i].din1=ii;

				ii = *(uint8_t *)(bufferin+8); // Get Din2 Status
				ii >>= 7;
				ii &= 0x01;
				if (wiTH_nodes[i].din2!=ii) wiTH_nodes[i].trap |= 0x0020;
				wiTH_nodes[i].din2=ii;
				wiTH_nodes[i].timestamp = os_time_get() / 100;

				if(wiTH_nodes[i].state == 'r') {
//					wiTH_nodes[i].trap |= 0x40;
				  wiTH_nodes[i].state = 'R';
				}
				

//				current_stamp = os_time_get();
//				switch(WiTH[i].th3 &0x3fff ) {
//					case SENSOR_TONLY:
//						bufferout[4] = wakeup_interval;
//					  interval_between_node = (wakeup_interval*6000) / NODE_NUM;
//					  next_stamp = ((current_stamp / (wakeup_interval*6000))+1) * (wakeup_interval*6000);
//					  break;
//					case SENSOR_BUSDUCT:
//						bufferout[4] = ds_wakeup_interval;
//					  interval_between_node = (ds_wakeup_interval*6000) / NODE_NUM;
//					  next_stamp = ((current_stamp / (ds_wakeup_interval*6000))+1) * (ds_wakeup_interval*6000);
//					  break;
//					default:
//						bufferout[4] = th_wakeup_interval;
//					  interval_between_node = (th_wakeup_interval*6000) / NODE_NUM;
//					  next_stamp = ((current_stamp / (th_wakeup_interval*6000))+1) * (th_wakeup_interval*6000);
//						break;
//				}
//        
//				next_stamp = next_stamp + (interval_between_node*i);
//				next_stamp = next_stamp - current_stamp;
				
//				if (next_stamp>=((interval_between_node*NODE_NUM)+((interval_between_node*NODE_NUM)/2))) 
//					next_stamp -= (interval_between_node*NODE_NUM); 
//				else if (next_stamp <= interval_between_node) 
//					next_stamp += (interval_between_node*NODE_NUM); 

				next_stamp = 600;
				current_stamp = 1234;

			  memset(bufferout+5,0,2);
			  memcpy(bufferout+5, (uint8_t*) &next_stamp, 2);

			  memset(bufferout+7,0,3);
			  memcpy(bufferout+7, (uint8_t*) &current_stamp, 3);
				printf("ID: %d Stamp :%d, NextWakeup: %d\r\n",i,current_stamp, next_stamp);
//				create_packet(bufferout, srcAddr, COMM_GW_TO_TH);

				break;
			
		}
		
		return ret;
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
	uint8_t cs_byte;
	int Oid = 0;
	//fprintf(f,"The start of the loop\n");
	time_t t;
	time (&t);
	struct tm * timeinfo = localtime (&t);
	openlog("cc1120Log", LOG_PID|LOG_CONS, LOG_USER);
	//scanning kwh and then adding them
	/*if ( kwh_loop <= 10){
		printf("Sending KWH data\n");
		syslog(LOG_INFO, "Sending KWH data\n");
		txBuffer[0] = 15; //length packet data
		txBuffer[1] = 0x02; //command code 
		*(uint16_t*)&txBuffer[2] =  gateway_ID; //(2 byte)
		*(uint16_t*)&txBuffer[4] = 0x0000; //(2 byte) id harusnya cuman 4 
		txBuffer[6] = 0xFF; 
		txBuffer[7] = 0xFF; //rssi
		txBuffer[8] = 0xFF; //sensor number
		txBuffer[9] = 0xFF; //radio channel
		txBuffer[10] = 0x14; //node type = ADE
		txBuffer[11] = 0x07;//in sec wakeup (2 byte)
		txBuffer[12] = 0x61;//in sec next wakeup (2 byte)
		txBuffer[13] = 0x28;
		txBuffer[14] = 0x1B;
		txBuffer[15] = 0x01;
		printf ("txbuffer ");
		syslog(LOG_INFO, "txbuffer ");
		for (i=0;i<=15;i++) {
			printf("%02X ", txBuffer[i]);
			//syslog(LOG_INFO, "%02X ", txBuffer[i]);
		}
		kwh_loop++;
		sleep(1);
	}*/
		// Infinite loop
	
	//fprintf(f,"Before spi read reg\n");
	/*	fprintf(f, "Before spi read reg %04d-%02d-%02d %02d:%02d:%02d\n", 
		timeinfo->tm_year+1900,
		timeinfo->tm_mon+1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec
		);*/
	cc112xSpiReadReg(CC112X_MARC_STATUS1, &temp_byte, 1);
	if (( temp_byte == 0x07)||( temp_byte == 0x08)){
		// Flush TX FIFO
		trxSpiCmdStrobe(CC112X_SFTX);
		
		printf("trxspicmdstrobe is excecuted %04d-%02d-%02d %02d:%02d:%02d", 
		timeinfo->tm_year+1900,
		timeinfo->tm_mon+1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec
		);
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
					//cc112xSpiReadReg(CC112X_MARCSTATE, &temp_byte, 1);
					printf("After there is data on rx buffer %04d-%02d-%02d %02d:%02d:%02d\n", 
					timeinfo->tm_year+1900,
					timeinfo->tm_mon+1,
					timeinfo->tm_mday,
					timeinfo->tm_hour,
					timeinfo->tm_min,
					timeinfo->tm_sec
					);
					// Update packet counter
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
				  
					processPacket((uint8_t*)&rxBuffer[1], txBuffer, rx_byte-3);
					
				}
			}
		}
		//fprintf(f,"After the whole data is processed\n");
	}
	else if( temp_byte == 0) {
					//os_dly_wait (1);
//		if (replyDly>0) return;
		send_packet(txBuffer);
		//fprintf(f,"After send packet buffer\n");
		return;
	}
	
	trxSpiCmdStrobe(CC112X_SIDLE);
	
	wait_exp_val(CC112X_MARCSTATE, 0x41);
		
			
    // Set radio back in RX
    trxSpiCmdStrobe(CC112X_SRX);
		closelog();
}



/*******************************************************************************
********************************************************************************/

int main(int argc, char *argv[]) {
  uint8_t DUMMY_BUF[]={1,2,3,4,5,6,7,8,9,0};
  int ret = 0;
	freq_main = 23;
	freq_th = 23;
	//freq_main = 0;
  gateway_ID = 0x1001;
	//mac_address_gateway = read_ints();
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
  //cc112x_init(23,0);// freq 410 Mhz + (1 Mhz * freq_main)
  cc112x_init(freq_main,0);// freq 410 Mhz + (1 Mhz * freq_main)
	memcpy(&txBuffer[1],DUMMY_BUF,10);
	txBuffer[0]=10;
	int datalog = 0;
	while (1)
	{
 		/*f = fopen("/home/data.log", "a");
		if (f == NULL)
		{
    	printf("Error opening file!\n");
    	exit(1);
		}*/
		/* print some text */
		/*const char *text = "Why the result are diffrent";
		fprintf(f, "Some text: %s %d \n", text, datalog);*/
		/* communication handler */
		cc112x_run();
		datalog++;
		//fclose(f);
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
