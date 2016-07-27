#include <stdint.h>

#include <string.h>



#define	MAX_CHANNEL	6



  uint8_t	Cc1120ConnectionStatChannels[MAX_CHANNEL];

	uint16_t AlarmFlag[MAX_CHANNEL];

		

	int16_t  PhaseRVoltUpperLimit[MAX_CHANNEL];

	int16_t  PhaseRVoltLowerLimit[MAX_CHANNEL];

	int16_t  PhaseSVoltUpperLimit[MAX_CHANNEL];

	int16_t  PhaseSVoltLowerLimit[MAX_CHANNEL];

	int16_t  PhaseTVoltUpperLimit[MAX_CHANNEL];

	int16_t  PhaseTVoltLowerLimit[MAX_CHANNEL];

	int32_t  PhaseRCurrentUpper[MAX_CHANNEL];

	int32_t  PhaseRCurrentLower[MAX_CHANNEL];

	int32_t  PhaseSCurrentUpper[MAX_CHANNEL];

	int32_t  PhaseSCurrentLower[MAX_CHANNEL];

	int32_t  PhaseTCurrentUpper[MAX_CHANNEL];

	int32_t  PhaseTCurrentLower[MAX_CHANNEL];

	uint16_t 	EventCodeChannels[MAX_CHANNEL];

	

	int32_t PhaseRVoltChannels[MAX_CHANNEL];					/*0x0300 multiple 10*/ /*0x1848 0x1849*/

	int32_t PhaseRCurrentChannels[MAX_CHANNEL];				/*0x0308 multiple 1000*/ /*0x1858 0x1859*/

	int32_t PhaseRFrequencyChannels[MAX_CHANNEL];			/*0x031C multiple ??*/

	int32_t PhaseRPowerFactorChannels[MAX_CHANNEL];

	int32_t PhaseRwattChannels[MAX_CHANNEL];					/*0x0310 multiple 1000*/ /*0x186C 0x186D 0x186E*/

	int32_t PhaseRvarChannels[MAX_CHANNEL];						/*0x0314 multiple 1000*/ /*0x1878 0x1879 0x187A*/

	int32_t PhaseRkwh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseRkvarh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseRkwh_totChannels[MAX_CHANNEL];

	int32_t PhaseRkvarh_totChannels[MAX_CHANNEL];

	int32_t PhaseRTHD_voltChannels[MAX_CHANNEL];

	int32_t PhaseRTHD_CurrentChannels[MAX_CHANNEL];





	int32_t PhaseSVoltChannels[MAX_CHANNEL];					/*0x0301 multiple 10*/ /*0x184A 0x184B*/

	int32_t PhaseSCurrentChannels[MAX_CHANNEL];				/*0x0309 multiple 1000*/ /*0x185A 0x185B*/

	int32_t PhaseSFrequencyChannels[MAX_CHANNEL];			/*0x031C multiple ??*/

	int32_t PhaseSPowerFactorChannels[MAX_CHANNEL];

	int32_t PhaseSwattChannels[MAX_CHANNEL];					/*0x0311 multiple 1000*/ /*0x186f 0x1870 0x1871*/

	int32_t PhaseSvarChannels[MAX_CHANNEL];						/*0x0315 multiple 1000*/ /*0x187B 0x187C 0x187D*/

	int32_t PhaseSkwh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseSkvarh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseSkwh_totChannels[MAX_CHANNEL];

	int32_t PhaseSkvarh_totChannels[MAX_CHANNEL];

	int32_t PhaseSTHD_voltChannels[MAX_CHANNEL];

	int32_t PhaseSTHD_CurrentChannels[MAX_CHANNEL];





	int32_t PhaseTVoltChannels[MAX_CHANNEL];					/*0x0302 multiple 10*/ /*0x184C 0x184D*/

	int32_t PhaseTCurrentChannels[MAX_CHANNEL];				/*0x030A multiple 1000*/ /*0x185C 0x185D*/

	int32_t PhaseTFrequencyChannels[MAX_CHANNEL];			/*0x031C multiple ??*/

	int32_t PhaseTPowerFactorChannels[MAX_CHANNEL];

	int32_t PhaseTwattChannels[MAX_CHANNEL];					/*0x0312 multiple 1000*/ /*0x1872 0x1873 0x1874*/

	int32_t PhaseTvarChannels[MAX_CHANNEL];						/*0x0316 multiple 1000*/ /*0x187E 0x187F 0x1880*/

	int32_t PhaseTkwh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseTkvarh_tot_prdChannels[MAX_CHANNEL];

	int32_t PhaseTkwh_totChannels[MAX_CHANNEL];

	int32_t PhaseTkvarh_totChannels[MAX_CHANNEL];

	int32_t PhaseTTHD_voltChannels[MAX_CHANNEL];

	int32_t PhaseTTHD_CurrentChannels[MAX_CHANNEL];



	uint64_t PhaseRtotWattChannels[MAX_CHANNEL];

	uint64_t PhaseStotWattChannels[MAX_CHANNEL];

	uint64_t PhaseTtotWattChannels[MAX_CHANNEL];

	uint64_t Phase3totWattChannels[MAX_CHANNEL];

	uint64_t PhaseRtotVARChannels[MAX_CHANNEL];

	uint64_t PhaseStotVARChannels[MAX_CHANNEL];

	uint64_t PhaseTtotVARChannels[MAX_CHANNEL];

	uint64_t Phase3totVARChannels[MAX_CHANNEL];

	

	int32_t Phase3wattChannels[MAX_CHANNEL];

	int32_t Phase3varChannels[MAX_CHANNEL];



	uint32_t  Phase3kwh_tot_prdChannels[MAX_CHANNEL];

	uint32_t  Phase3kvarh_tot_prdChannels[MAX_CHANNEL];



	uint32_t kwh_totChannels[MAX_CHANNEL];

	uint32_t kwhdmd_totChannels[MAX_CHANNEL];

	uint32_t kvarh_totChannels[MAX_CHANNEL];

	uint32_t kvarhdmd_totChannels[MAX_CHANNEL];





//temp_8 = *(uint8_t *)(bufferin+10);

//temp_32 = get_params_value(bufferin+11, temp_8, len-11);



int get_params_value(uint8_t* recbuff, uint8_t subcmd, int len){

	int chan;

	int32_t s32;

	int ret = 0;

	switch(subcmd){

		case 1:	// R: V, I, PF

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff,4);

					PhaseRVoltChannels[chan] = s32/1000;

				

					memcpy((uint8_t*)&s32,recbuff+4+(4*chan),4);

					PhaseRCurrentChannels[chan] = s32/100;

			

					memcpy((uint8_t*)&PhaseRPowerFactorChannels[chan],recbuff+28+(4*chan),4);

				

					if (PhaseRVoltChannels[chan] > PhaseRVoltUpperLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0002;   

					else AlarmFlag[chan] &= (uint16_t )~0x0002; 

					if (PhaseRVoltChannels[chan] < PhaseRVoltLowerLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0004; 

					else AlarmFlag[chan] &= (uint16_t )~0x0004;

				

					if (PhaseRCurrentChannels[chan] > PhaseRCurrentUpper[chan]) AlarmFlag[chan] |= (uint16_t )0x0080; 

					else AlarmFlag[chan] &= (uint16_t )~0x0080; 

					if (PhaseRCurrentChannels[chan] < PhaseRCurrentLower[chan]) AlarmFlag[chan] |= (uint16_t )0x0100; 

					else AlarmFlag[chan] &= (uint16_t )~0x0100; 

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			

			ret |= 0x01;

			break;

		

		case 2: // R: Watt, Var

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff+(4*chan),4);

					PhaseRwattChannels[chan] = s32*1000;



					memcpy((uint8_t*)&s32,recbuff+24+(4*chan),4);

					PhaseRvarChannels[chan] = s32*1000;

				

					memcpy((uint8_t*)&s32,recbuff+48,4);

					PhaseRFrequencyChannels[chan] = s32;

				

					AlarmFlag[chan] &= (uint16_t )~0x0001;

				  Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x02;

			break;



		case 3:	// S: V, I, PF

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff,4);

					PhaseSVoltChannels[chan] = s32/1000;

				

					memcpy((uint8_t*)&s32,recbuff+4+(4*chan),4);

					PhaseSCurrentChannels[chan] = s32 / 100;

			

					memcpy((uint8_t*)&PhaseSPowerFactorChannels[chan],recbuff+28+(4*chan),4);

				

					if (PhaseSVoltChannels[chan] > PhaseSVoltUpperLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0008; 

					else AlarmFlag[chan] &= (uint16_t )~0x0008; 

					if (PhaseSVoltChannels[chan] < PhaseSVoltLowerLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0010; 

					else AlarmFlag[chan] &= (uint16_t )~0x0010; 

				

					if (PhaseSCurrentChannels[chan] > PhaseSCurrentUpper[chan]) AlarmFlag[chan] |= (uint16_t )0x0200; 

					else AlarmFlag[chan] &= (uint16_t )~0x0200; 

					if (PhaseSCurrentChannels[chan] < PhaseSCurrentLower[chan]) AlarmFlag[chan] |= (uint16_t )0x0400; 

					else AlarmFlag[chan] &= (uint16_t )~0x0400;

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x04;

			break;

		

		case 4: // S: Watt, Var

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff+(4*chan),4);

					PhaseSwattChannels[chan] = s32 * 1000;



					memcpy((uint8_t*)&s32,recbuff+24+(4*chan),4);

					PhaseSvarChannels[chan] = s32 * 1000;

				

					memcpy((uint8_t*)&s32,recbuff+48,4);

					PhaseSFrequencyChannels[chan] = s32;

				

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x08;

			break;

			

		case 5:	// T: V, I, PF

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff,4);

					PhaseTVoltChannels[chan] = s32/1000;

				

					memcpy((uint8_t*)&s32,recbuff+4+(4*chan),4);

					PhaseTCurrentChannels[chan] = s32 / 100;

			

					memcpy((uint8_t*)&PhaseTPowerFactorChannels[chan],recbuff+28+(4*chan),4);

				

					if (PhaseTVoltChannels[chan] > PhaseTVoltUpperLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0020; 

					else AlarmFlag[chan] &= (uint16_t )~0x0020; 

					if (PhaseTVoltChannels[chan] < PhaseTVoltLowerLimit[chan]) AlarmFlag[chan] |= (uint16_t )0x0040; 

					else AlarmFlag[chan] &= (uint16_t )~0x0040; 

				

					if (PhaseTCurrentChannels[chan] > PhaseTCurrentUpper[chan]) AlarmFlag[chan] |= (uint16_t )0x0800; 

					else AlarmFlag[chan] &= (uint16_t )~0x0800; 

					if (PhaseTCurrentChannels[chan] < PhaseTCurrentLower[chan]) AlarmFlag[chan] |= (uint16_t )0x1000; 

					else AlarmFlag[chan] &= (uint16_t )~0x1000; 

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x10;

			break;

		

		case 6: // T: Watt, Var

			if (len!=52) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&s32,recbuff+(4*chan),4);

					PhaseTwattChannels[chan] = s32*1000;



					memcpy((uint8_t*)&s32,recbuff+24+(4*chan),4);

					PhaseTvarChannels[chan] = s32*1000;

				

					memcpy((uint8_t*)&s32,recbuff+48,4);

					PhaseTFrequencyChannels[chan] = s32;

				

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x20;

			break;

			

		case 0x11: //R: watt prd, watt hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseRkwh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseRkwh_totChannels[chan],recbuff+24+(4*chan),4);

					Phase3kwh_tot_prdChannels[chan] = PhaseRkwh_tot_prdChannels[chan] + 

																				    PhaseSkwh_tot_prdChannels[chan] + 

																				    PhaseTkwh_tot_prdChannels[chan];

		

					kwhdmd_totChannels[chan] 			 = PhaseRkwh_totChannels[chan] +

																				   PhaseSkwh_totChannels[chan] +

																				   PhaseTkwh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x40;

			break;

		

		case 0x12: //R: var prd, var hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseRkvarh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseRkvarh_totChannels[chan],recbuff+24+(4*chan),4);



					Phase3kvarh_tot_prdChannels[chan] = PhaseRkvarh_tot_prdChannels[chan] + 

																							PhaseSkvarh_tot_prdChannels[chan] + 

																							PhaseTkvarh_tot_prdChannels[chan];

		

					kvarhdmd_totChannels[chan] 			 = PhaseRkvarh_totChannels[chan] +

																					   PhaseSkvarh_totChannels[chan] +

																						 PhaseTkvarh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x80;

			break;

		

		case 0x13: //S: watt prd, watt hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseSkwh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseSkwh_totChannels[chan],recbuff+24+(4*chan),4);

					Phase3kwh_tot_prdChannels[chan] = PhaseRkwh_tot_prdChannels[chan] + 

																				    PhaseSkwh_tot_prdChannels[chan] + 

																				    PhaseTkwh_tot_prdChannels[chan];

		

					kwhdmd_totChannels[chan] 			 = PhaseRkwh_totChannels[chan] +

																				   PhaseSkwh_totChannels[chan] +

																				   PhaseTkwh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x0100;

			break;

		

		case 0x14: //S: var prd, var hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseSkvarh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseSkvarh_totChannels[chan],recbuff+24+(4*chan),4);



					Phase3kvarh_tot_prdChannels[chan] = PhaseRkvarh_tot_prdChannels[chan] + 

																							PhaseSkvarh_tot_prdChannels[chan] + 

																							PhaseTkvarh_tot_prdChannels[chan];

		

					kvarhdmd_totChannels[chan] 			 = PhaseRkvarh_totChannels[chan] +

																					   PhaseSkvarh_totChannels[chan] +

																						 PhaseTkvarh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x0200;

			break;

			

		case 0x15: //T: watt prd, watt hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseTkwh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseTkwh_totChannels[chan],recbuff+24+(4*chan),4);

					Phase3kwh_tot_prdChannels[chan] = PhaseRkwh_tot_prdChannels[chan] + 

																				    PhaseSkwh_tot_prdChannels[chan] + 

																				    PhaseTkwh_tot_prdChannels[chan];

		

					kwhdmd_totChannels[chan] 			 = PhaseRkwh_totChannels[chan] +

																				   PhaseSkwh_totChannels[chan] +

																				   PhaseTkwh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x0400;

			break;

		

		case 0x16: //T: var prd, var hour

			if (len!=48) break;

			for (chan=0;chan<MAX_CHANNEL;chan++) {

					memcpy((uint8_t*)&PhaseTkvarh_tot_prdChannels[chan],recbuff+(4*chan),4);

					memcpy((uint8_t*)&PhaseTkvarh_totChannels[chan],recbuff+24+(4*chan),4);



					Phase3kvarh_tot_prdChannels[chan] = PhaseRkvarh_tot_prdChannels[chan] + 

																							PhaseSkvarh_tot_prdChannels[chan] + 

																							PhaseTkvarh_tot_prdChannels[chan];

		

					kvarhdmd_totChannels[chan] 			 = PhaseRkvarh_totChannels[chan] +

																					   PhaseSkvarh_totChannels[chan] +

																						 PhaseTkvarh_totChannels[chan];

					AlarmFlag[chan] &= (uint16_t )~0x0001;

					Cc1120ConnectionStatChannels[chan] = 1;

			}

			ret |= 0x0800;

			break;

		

	}

		

	return ret;

}
