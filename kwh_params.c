#include <stdint.h>
#include <string.h>

typedef struct{
	int32_t vrms;
	int32_t irmsa;
	int32_t irmsb;
	int32_t irmsc;
	int32_t irmsd;
	int32_t irmse;
	int32_t irmsf;
	int32_t freq;
	int32_t angle0;
	int32_t angle1;
	int32_t angle2;
	int32_t pf0;
	int32_t pf1;
	int32_t pf2;
	int32_t pfa;
	int32_t pfb;
	int32_t pfc;
	int32_t pfd;
	int32_t pfe;
	int32_t pff;
	int32_t anglea;
	int32_t angleb;
	int32_t anglec;
	int32_t angled;
	int32_t anglee;
	int32_t anglef;
	int16_t sinpa;
	int16_t sinpb;
	int16_t sinpc;
	int16_t sinpd;
	int16_t sinpe;
	int16_t sinpf;
	int32_t wa;
	int32_t wb;
	int32_t wc;
	int32_t wd;
	int32_t we;
	int32_t wf;
	int32_t wha;
	int32_t whb;
	int32_t whc;
	int32_t whd;
	int32_t whe;
	int32_t whf;
	int32_t wha_per;
	int32_t whb_per;
	int32_t whc_per;
	int32_t whd_per;
	int32_t whe_per;
	int32_t whf_per;
	int32_t vara;
	int32_t varb;
	int32_t varc;
	int32_t vard;
	int32_t vare;
	int32_t varf;
	int32_t varha;
	int32_t varhb;
	int32_t varhc;
	int32_t varhd;
	int32_t varhe;
	int32_t varhf;
	int32_t varha_per;
	int32_t varhb_per;
	int32_t varhc_per;
	int32_t varhd_per;
	int32_t varhe_per;
	int32_t varhf_per;
	int32_t voltUpperLimit;
	int32_t voltLowerLimit;
	int32_t iUpperLimit;
	int32_t iLowerLimit;
	int16_t ct_ratio_a;
	int16_t ct_ratio_b;
	int16_t ct_ratio_c;
	int16_t ct_ratio_d;
	int16_t ct_ratio_e;
	int16_t ct_ratio_f;
	int16_t v_threshold;
	int16_t ia_threshold;
	int16_t ib_threshold;
	int16_t ic_threshold;
	int16_t id_threshold;
	int16_t ie_threshold;
	int16_t if_threshold;
	uint16_t AlarmFlaga;
	uint16_t AlarmFlagb;
	uint16_t AlarmFlagc;
	uint16_t AlarmFlagd;
	uint16_t AlarmFlage;
	uint16_t AlarmFlagf;
	uint8_t  status;
}ADE7816_STRUCT;

ADE7816_STRUCT phases[3];
#define phase_R phases[0]
#define phase_S phases[1]
#define phase_T phases[2]
time_t  phase_ts;
uint8_t phase_flags[14];

int get_params_value(uint8_t* recbuff, uint8_t subcmd, int len){
	
	int32_t s32;
	uint32_t u32;
	
	struct timespec tspec;
					
	clock_gettime(CLOCK_REALTIME, &tspec);
	phase_ts  = tspec.tv_sec;
	//printf("\r\nDetik: %d\r\n", phase_ts);
	
	switch(subcmd){
		case 0: // RST: vrms, freq
          if(len != 36) break;
		  phase_R.status = recbuff[0];
		  memcpy((uint8_t*)&u32, &recbuff[1], 4); phase_R.vrms = u32/1000;
		  memcpy((uint8_t*)&u32, &recbuff[5], 4); phase_R.freq = u32;
			
		  phase_S.status = recbuff[9];
		  memcpy((uint8_t*)&u32, &recbuff[10], 4); phase_S.vrms = u32/1000;
		  memcpy((uint8_t*)&u32, &recbuff[14], 4); phase_S.freq = u32;
			
			phase_T.status = recbuff[18];
		  memcpy((uint8_t*)&u32, &recbuff[19], 4); phase_T.vrms = u32/1000;
		  memcpy((uint8_t*)&u32, &recbuff[23], 4); phase_T.freq = u32;
		  phase_flags[0] = STATUS_UPDATED;
		  
		  printf("VoltR: %03d.%01d freq: %02d.%0d\r\n", phase_R.vrms/10, phase_R.vrms%10, phase_R.freq/100, phase_R.freq%100);
		  printf("VoltS: %03d.%01d freq: %02d.%0d\r\n", phase_S.vrms/10, phase_S.vrms%10, phase_S.freq/100, phase_S.freq%100);
		  printf("VoltT: %03d.%01d freq: %02d.%0d\r\n", phase_T.vrms/10, phase_T.vrms%10, phase_T.freq/100, phase_T.freq%100);
			break;
		
		case 1:	// RST: Ia, pfa, WATTa, VARa

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmsa = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pfa   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.wa    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.vara  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmsa = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pfa   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.wa    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.vara  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmsa = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pfa   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.wa    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.vara  = s32;
			phase_flags[1] = STATUS_UPDATED;
			
          printf("RIa: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmsa/1000, phase_R.irmsa%1000, phase_R.pfa, phase_R.wa, phase_R.vara);
		  printf("SIa: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmsa/1000, phase_S.irmsa%1000, phase_S.pfa, phase_S.wa, phase_S.vara);
		  printf("TIa: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmsa/1000, phase_T.irmsa%1000, phase_T.pfa, phase_T.wa, phase_T.vara);

		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlaga |= (uint16_t )0x0002;
		  else phase_R.AlarmFlaga &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlaga |= (uint16_t )0x0004;
		  else phase_R.AlarmFlaga &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlaga |= (uint16_t )0x0002;
		  else phase_S.AlarmFlaga &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlaga |= (uint16_t )0x0004;
		  else phase_S.AlarmFlaga &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlaga |= (uint16_t )0x0002;
		  else phase_T.AlarmFlaga &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlaga |= (uint16_t )0x0004;
		  else phase_T.AlarmFlaga &= (uint16_t )~0x0004;

		  if (phase_R.irmsa > phase_R.iUpperLimit) phase_R.AlarmFlaga |= (uint16_t )0x0080;
		  else phase_R.AlarmFlaga &= (uint16_t )~0x0080; 
		  if (phase_R.irmsa < phase_R.iLowerLimit) phase_R.AlarmFlaga |= (uint16_t )0x0100;
		  else phase_R.AlarmFlaga &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmsa > phase_S.iUpperLimit) phase_S.AlarmFlaga |= (uint16_t )0x0080;
		  else phase_S.AlarmFlaga &= (uint16_t )~0x0080; 
		  if (phase_S.irmsa < phase_S.iLowerLimit) phase_S.AlarmFlaga |= (uint16_t )0x0100;
		  else phase_S.AlarmFlaga &= (uint16_t )~0x0100;

		  if (phase_T.irmsa > phase_T.iUpperLimit) phase_T.AlarmFlaga |= (uint16_t )0x0080;
		  else phase_T.AlarmFlaga &= (uint16_t )~0x0080;
		  if (phase_T.irmsa < phase_T.iLowerLimit) phase_T.AlarmFlaga |= (uint16_t )0x0100;
		  else phase_T.AlarmFlaga &= (uint16_t )~0x0100;

			break;
		
		case 2: // RST: Ib, pfb, WATTb, VARb

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmsb = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pfb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.wb    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varb  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmsb = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pfb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.wb    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varb  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmsb = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pfb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.wb    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varb  = s32;
			
			phase_flags[2] = STATUS_UPDATED;
			
          printf("RIb: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmsb/1000, phase_R.irmsb%1000, phase_R.pfb, phase_R.wb, phase_R.varb);
		  printf("SIb: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmsb/1000, phase_S.irmsb%1000, phase_S.pfb, phase_S.wb, phase_S.varb);
		  printf("TIb: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmsb/1000, phase_T.irmsb%1000, phase_T.pfb, phase_T.wb, phase_T.varb);

		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlagb |= (uint16_t )0x0002;
		  else phase_R.AlarmFlagb &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlagb |= (uint16_t )0x0004;
		  else phase_R.AlarmFlagb &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlagb |= (uint16_t )0x0002;
		  else phase_S.AlarmFlagb &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlagb |= (uint16_t )0x0004;
		  else phase_S.AlarmFlagb &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlagb |= (uint16_t )0x0002;
		  else phase_T.AlarmFlagb &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlagb |= (uint16_t )0x0004;
		  else phase_T.AlarmFlagb &= (uint16_t )~0x0004;

		  if (phase_R.irmsb > phase_R.iUpperLimit) phase_R.AlarmFlagb |= (uint16_t )0x0080;
		  else phase_R.AlarmFlagb &= (uint16_t )~0x0080; 
		  if (phase_R.irmsb < phase_R.iLowerLimit) phase_R.AlarmFlagb |= (uint16_t )0x0100;
		  else phase_R.AlarmFlagb &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmsb > phase_S.iUpperLimit) phase_S.AlarmFlagb |= (uint16_t )0x0080;
		  else phase_S.AlarmFlagb &= (uint16_t )~0x0080; 
		  if (phase_S.irmsb < phase_S.iLowerLimit) phase_S.AlarmFlagb |= (uint16_t )0x0100;
		  else phase_S.AlarmFlagb &= (uint16_t )~0x0100;

		  if (phase_T.irmsb > phase_T.iUpperLimit) phase_T.AlarmFlagb |= (uint16_t )0x0080;
		  else phase_T.AlarmFlagb &= (uint16_t )~0x0080;
		  if (phase_T.irmsb < phase_T.iLowerLimit) phase_T.AlarmFlagb |= (uint16_t )0x0100;
		  else phase_T.AlarmFlagb &= (uint16_t )~0x0100;
			break;
		
		case 3:	// RST: Ic, pfc, WATTc, VARc
            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmsc = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pfc   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.wc    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varc  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmsc = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pfc   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.wc    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varc  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmsc = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pfc   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.wc    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varc  = s32;
			phase_flags[3] = STATUS_UPDATED;
			
          printf("RIc: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmsc/1000, phase_R.irmsc%1000, phase_R.pfc, phase_R.wc, phase_R.varc);
		  printf("SIc: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmsc/1000, phase_S.irmsc%1000, phase_S.pfc, phase_S.wc, phase_S.varc);
		  printf("TIc: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmsc/1000, phase_T.irmsc%1000, phase_T.pfc, phase_T.wc, phase_T.varc);

		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlagc |= (uint16_t )0x0002;
		  else phase_R.AlarmFlagc &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlagc |= (uint16_t )0x0004;
		  else phase_R.AlarmFlagc &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlagc |= (uint16_t )0x0002;
		  else phase_S.AlarmFlagc &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlagc |= (uint16_t )0x0004;
		  else phase_S.AlarmFlagc &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlagc |= (uint16_t )0x0002;
		  else phase_T.AlarmFlagc &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlagc |= (uint16_t )0x0004;
		  else phase_T.AlarmFlagc &= (uint16_t )~0x0004;

		  if (phase_R.irmsc > phase_R.iUpperLimit) phase_R.AlarmFlagc |= (uint16_t )0x0080;
		  else phase_R.AlarmFlagc &= (uint16_t )~0x0080; 
		  if (phase_R.irmsc < phase_R.iLowerLimit) phase_R.AlarmFlagc |= (uint16_t )0x0100;
		  else phase_R.AlarmFlagc &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmsc > phase_S.iUpperLimit) phase_S.AlarmFlagc |= (uint16_t )0x0080;
		  else phase_S.AlarmFlagc &= (uint16_t )~0x0080; 
		  if (phase_S.irmsc < phase_S.iLowerLimit) phase_S.AlarmFlagc |= (uint16_t )0x0100;
		  else phase_S.AlarmFlagc &= (uint16_t )~0x0100;

		  if (phase_T.irmsc > phase_T.iUpperLimit) phase_T.AlarmFlagc |= (uint16_t )0x0080;
		  else phase_T.AlarmFlagc &= (uint16_t )~0x0080;
		  if (phase_T.irmsc < phase_T.iLowerLimit) phase_T.AlarmFlagc |= (uint16_t )0x0100;
		  else phase_T.AlarmFlagc &= (uint16_t )~0x0100;
			break;
		
		case 4: // RST: Id, pfd, WATTd, VARd
            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmsd = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pfd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.wd    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.vard  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmsd = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pfd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.wd    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.vard  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmsd = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pfd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.wd    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.vard  = s32;
			phase_flags[4] = STATUS_UPDATED;

          printf("RId: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmsd/1000, phase_R.irmsd%1000, phase_R.pfd, phase_R.wd, phase_R.vard);
		  printf("SId: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmsd/1000, phase_S.irmsd%1000, phase_S.pfd, phase_S.wd, phase_S.vard);
		  printf("TId: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmsd/1000, phase_T.irmsd%1000, phase_T.pfd, phase_T.wd, phase_T.vard);
			
		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlagd |= (uint16_t )0x0002;
		  else phase_R.AlarmFlagd &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlagd |= (uint16_t )0x0004;
		  else phase_R.AlarmFlagd &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlagd |= (uint16_t )0x0002;
		  else phase_S.AlarmFlagd &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlagd |= (uint16_t )0x0004;
		  else phase_S.AlarmFlagd &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlagd |= (uint16_t )0x0002;
		  else phase_T.AlarmFlagd &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlagd |= (uint16_t )0x0004;
		  else phase_T.AlarmFlagd &= (uint16_t )~0x0004;

		  if (phase_R.irmsd > phase_R.iUpperLimit) phase_R.AlarmFlagd |= (uint16_t )0x0080;
		  else phase_R.AlarmFlagd &= (uint16_t )~0x0080; 
		  if (phase_R.irmsd < phase_R.iLowerLimit) phase_R.AlarmFlagd |= (uint16_t )0x0100;
		  else phase_R.AlarmFlagd &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmsd > phase_S.iUpperLimit) phase_S.AlarmFlagd |= (uint16_t )0x0080;
		  else phase_S.AlarmFlagd &= (uint16_t )~0x0080; 
		  if (phase_S.irmsd < phase_S.iLowerLimit) phase_S.AlarmFlagd |= (uint16_t )0x0100;
		  else phase_S.AlarmFlagd &= (uint16_t )~0x0100;

		  if (phase_T.irmsd > phase_T.iUpperLimit) phase_T.AlarmFlagd |= (uint16_t )0x0080;
		  else phase_T.AlarmFlagd &= (uint16_t )~0x0080;
		  if (phase_T.irmsd < phase_T.iLowerLimit) phase_T.AlarmFlagd |= (uint16_t )0x0100;
		  else phase_T.AlarmFlagd &= (uint16_t )~0x0100;
			break;
		
		case 5:	// RST: Ie, pfe, WATTe, VARe

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmse = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pfe   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.we    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.vare  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmse = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pfe   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.we    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.vare  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmse = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pfe   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.we    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.vare  = s32;
			phase_flags[5] = STATUS_UPDATED;

          printf("RIe: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmse/1000, phase_R.irmse%1000, phase_R.pfe, phase_R.we, phase_R.vare);
		  printf("SIe: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmse/1000, phase_S.irmse%1000, phase_S.pfe, phase_S.we, phase_S.vare);
		  printf("TIe: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmse/1000, phase_T.irmse%1000, phase_T.pfe, phase_T.we, phase_T.vare);
			
		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlage |= (uint16_t )0x0002;
		  else phase_R.AlarmFlage &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlage |= (uint16_t )0x0004;
		  else phase_R.AlarmFlage &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlage |= (uint16_t )0x0002;
		  else phase_S.AlarmFlage &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlage |= (uint16_t )0x0004;
		  else phase_S.AlarmFlage &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlage |= (uint16_t )0x0002;
		  else phase_T.AlarmFlage &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlage |= (uint16_t )0x0004;
		  else phase_T.AlarmFlage &= (uint16_t )~0x0004;

		  if (phase_R.irmse > phase_R.iUpperLimit) phase_R.AlarmFlage |= (uint16_t )0x0080;
		  else phase_R.AlarmFlage &= (uint16_t )~0x0080; 
		  if (phase_R.irmse < phase_R.iLowerLimit) phase_R.AlarmFlage |= (uint16_t )0x0100;
		  else phase_R.AlarmFlage &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmse > phase_S.iUpperLimit) phase_S.AlarmFlage |= (uint16_t )0x0080;
		  else phase_S.AlarmFlage &= (uint16_t )~0x0080; 
		  if (phase_S.irmse < phase_S.iLowerLimit) phase_S.AlarmFlage |= (uint16_t )0x0100;
		  else phase_S.AlarmFlage &= (uint16_t )~0x0100;

		  if (phase_T.irmse > phase_T.iUpperLimit) phase_T.AlarmFlage |= (uint16_t )0x0080;
		  else phase_T.AlarmFlage &= (uint16_t )~0x0080;
		  if (phase_T.irmse < phase_T.iLowerLimit) phase_T.AlarmFlage |= (uint16_t )0x0100;
		  else phase_T.AlarmFlage &= (uint16_t )~0x0100;
			break;
		
		case 6: // RST: If, pff, WATTf, VARf
            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.irmsf = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.pff   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.wf    = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varf  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.irmsf = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.pff   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.wf    = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varf  = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.irmsf = s32/100;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.pff   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.wf    = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varf  = s32;
			phase_flags[6] = STATUS_UPDATED;
			
          printf("RIf: %03d.%03d pf: %02d %d %d\r\n", phase_R.irmsf/1000, phase_R.irmsf%1000, phase_R.pff, phase_R.wf, phase_R.varf);
		  printf("SIf: %03d.%03d pf: %02d %d %d\r\n", phase_S.irmsf/1000, phase_S.irmsf%1000, phase_S.pff, phase_S.wf, phase_S.varf);
		  printf("TIf: %03d.%03d pf: %02d %d %d\r\n", phase_T.irmsf/1000, phase_T.irmsf%1000, phase_T.pff, phase_T.wf, phase_T.varf);

		  if (phase_R.vrms > phase_R.voltUpperLimit) phase_R.AlarmFlagf |= (uint16_t )0x0002;
		  else phase_R.AlarmFlagf &= (uint16_t )~0x0002; 
		  if (phase_R.vrms < phase_R.voltLowerLimit) phase_R.AlarmFlagf |= (uint16_t )0x0004;
		  else phase_R.AlarmFlagf &= (uint16_t )~0x0004; 
			
		  if (phase_S.vrms > phase_S.voltUpperLimit) phase_S.AlarmFlagf |= (uint16_t )0x0002;
		  else phase_S.AlarmFlagf &= (uint16_t )~0x0002; 
		  if (phase_S.vrms < phase_S.voltLowerLimit) phase_S.AlarmFlagf |= (uint16_t )0x0004;
		  else phase_S.AlarmFlagf &= (uint16_t )~0x0004;

		  if (phase_T.vrms > phase_T.voltUpperLimit) phase_T.AlarmFlagf |= (uint16_t )0x0002;
		  else phase_T.AlarmFlagf &= (uint16_t )~0x0002;
		  if (phase_T.vrms < phase_T.voltLowerLimit) phase_T.AlarmFlagf |= (uint16_t )0x0004;
		  else phase_T.AlarmFlagf &= (uint16_t )~0x0004;

		  if (phase_R.irmsf > phase_R.iUpperLimit) phase_R.AlarmFlagf |= (uint16_t )0x0080;
		  else phase_R.AlarmFlagf &= (uint16_t )~0x0080; 
		  if (phase_R.irmsf < phase_R.iLowerLimit) phase_R.AlarmFlagf |= (uint16_t )0x0100;
		  else phase_R.AlarmFlagf &= (uint16_t )~0x0100; 
			
		  if (phase_S.irmsf > phase_S.iUpperLimit) phase_S.AlarmFlagf |= (uint16_t )0x0080;
		  else phase_S.AlarmFlagf &= (uint16_t )~0x0080; 
		  if (phase_S.irmsf < phase_S.iLowerLimit) phase_S.AlarmFlagf |= (uint16_t )0x0100;
		  else phase_S.AlarmFlagf &= (uint16_t )~0x0100;

		  if (phase_T.irmsf > phase_T.iUpperLimit) phase_T.AlarmFlagf |= (uint16_t )0x0080;
		  else phase_T.AlarmFlagf &= (uint16_t )~0x0080;
		  if (phase_T.irmsf < phase_T.iLowerLimit) phase_T.AlarmFlagf |= (uint16_t )0x0100;
		  else phase_T.AlarmFlagf &= (uint16_t )~0x0100;
			break;
			
		case 7:	// RST: whab varhab

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.wha   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varha = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhb = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.wha   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varha = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhb = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.wha   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whb   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varha = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhb = s32;
			phase_flags[7] = STATUS_UPDATED;
			
          printf("Rwha: %03d.%03d varh: %03d.%03d\r\n", phase_R.wha/1000, abs(phase_R.wha%1000), phase_R.varha/1000, abs(phase_R.varha%1000));
		  printf("Rwhb: %03d.%03d varh: %03d.%03d\r\n", phase_R.whb/1000, abs(phase_R.whb%1000), phase_R.varhb/1000, abs(phase_R.varhb%1000));
		  printf("Swha: %03d.%03d varh: %03d.%03d\r\n", phase_S.wha/1000, abs(phase_S.wha%1000), phase_S.varha/1000, abs(phase_S.varha%1000));
		  printf("Swhb: %03d.%03d varh: %03d.%03d\r\n", phase_S.whb/1000, abs(phase_S.whb%1000), phase_S.varhb/1000, abs(phase_S.varhb%1000));
		  printf("Twha: %03d.%03d varh: %03d.%03d\r\n", phase_T.wha/1000, abs(phase_T.wha%1000), phase_T.varha/1000, abs(phase_T.varha%1000));
		  printf("Twhb: %03d.%03d varh: %03d.%03d\r\n", phase_T.whb/1000, abs(phase_T.whb%1000), phase_T.varhb/1000, abs(phase_T.varhb%1000));
		  
			break;
		
		case 8: // RST: whcd varhcd

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.whc   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varhc = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhd = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.whc   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varhc = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhd = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.whc   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whd   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varhc = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhd = s32;
			phase_flags[8] = STATUS_UPDATED;
			
          printf("Rwhc: %03d.%03d varh: %03d.%03d\r\n", phase_R.whc/1000, abs(phase_R.whc%1000), phase_R.varhc/1000, abs(phase_R.varhc%1000));
		  printf("Rwhd: %03d.%03d varh: %03d.%03d\r\n", phase_R.whd/1000, abs(phase_R.whd%1000), phase_R.varhd/1000, abs(phase_R.varhd%1000));
		  printf("Swhc: %03d.%03d varh: %03d.%03d\r\n", phase_S.whc/1000, abs(phase_S.whc%1000), phase_S.varhc/1000, abs(phase_S.varhc%1000));
		  printf("Swhd: %03d.%03d varh: %03d.%03d\r\n", phase_S.whd/1000, abs(phase_S.whd%1000), phase_S.varhd/1000, abs(phase_S.varhd%1000));
		  printf("Twhc: %03d.%03d varh: %03d.%03d\r\n", phase_T.whc/1000, abs(phase_T.whc%1000), phase_T.varhc/1000, abs(phase_T.varhc%1000));
		  printf("Twhd: %03d.%03d varh: %03d.%03d\r\n", phase_T.whd/1000, abs(phase_T.whd%1000), phase_T.varhd/1000, abs(phase_T.varhd%1000));
			
			break;
		
		case 9:	// RST: whef varhef

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.whe   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whf   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varhe = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhf = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.whe   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whf   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varhe = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhf = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.whe   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whf   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varhe = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhf = s32;
			phase_flags[9] = STATUS_UPDATED;
			
          printf("Rwhe: %03d.%03d varh: %03d.%03d\r\n", phase_R.whe/1000, abs(phase_R.whe%1000), phase_R.varhe/1000, abs(phase_R.varhe%1000));
		  printf("Rwhf: %03d.%03d varh: %03d.%03d\r\n", phase_R.whf/1000, abs(phase_R.whf%1000), phase_R.varhf/1000, abs(phase_R.varhf%1000));
		  printf("Swhe: %03d.%03d varh: %03d.%03d\r\n", phase_S.whe/1000, abs(phase_S.whe%1000), phase_S.varhe/1000, abs(phase_S.varhe%1000));
		  printf("Swhf: %03d.%03d varh: %03d.%03d\r\n", phase_S.whf/1000, abs(phase_S.whf%1000), phase_S.varhf/1000, abs(phase_S.varhf%1000));
		  printf("Twhe: %03d.%03d varh: %03d.%03d\r\n", phase_T.whe/1000, abs(phase_T.whe%1000), phase_T.varhe/1000, abs(phase_T.varhe%1000));
		  printf("Twhf: %03d.%03d varh: %03d.%03d\r\n", phase_T.whf/1000, abs(phase_T.whf%1000), phase_T.varhf/1000, abs(phase_T.varhf%1000));
			
			break;
		
		case 10:	// RST: whper ab, varhper ab

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.wha_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whb_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varha_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhb_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.wha_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whb_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varha_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhb_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.wha_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whb_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varha_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhb_per = s32;
			phase_flags[10] = STATUS_UPDATED;
			
          printf("Rwhpa: %03d.%03d varh: %03d.%03d\r\n", phase_R.wha_per/1000, abs(phase_R.wha_per%1000), phase_R.varha_per/1000, abs(phase_R.varha_per%1000));
		  printf("Rwhpb: %03d.%03d varh: %03d.%03d\r\n", phase_R.whb_per/1000, abs(phase_R.whb_per%1000), phase_R.varhb_per/1000, abs(phase_R.varhb_per%1000));
		  printf("Swhpa: %03d.%03d varh: %03d.%03d\r\n", phase_S.wha_per/1000, abs(phase_R.wha_per%1000), phase_S.varha_per/1000, abs(phase_R.varha_per%1000));
		  printf("Swhpb: %03d.%03d varh: %03d.%03d\r\n", phase_S.whb_per/1000, abs(phase_R.whb_per%1000), phase_S.varhb_per/1000, abs(phase_R.varhb_per%1000));
		  printf("Twhpa: %03d.%03d varh: %03d.%03d\r\n", phase_T.wha_per/1000, abs(phase_R.wha_per%1000), phase_T.varha_per/1000, abs(phase_R.varha_per%1000));
		  printf("Twhpb: %03d.%03d varh: %03d.%03d\r\n", phase_T.whb_per/1000, abs(phase_R.whb_per%1000), phase_T.varhb_per/1000, abs(phase_R.varhb_per%1000));
			
			break;
		
		case 11: // RST: whper cd, varhper cd

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.whc_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whd_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varhc_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhd_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.whc_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whd_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varhc_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhd_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.whc_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whd_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varhc_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhd_per = s32;
			phase_flags[11] = STATUS_UPDATED;
			
          printf("Rwhpc: %03d.%03d varh: %03d.%03d\r\n", phase_R.whc_per/1000, abs(phase_R.whc_per%1000), phase_R.varhc_per/1000, abs(phase_R.varhc_per%1000));
		  printf("Rwhpd: %03d.%03d varh: %03d.%03d\r\n", phase_R.whd_per/1000, abs(phase_R.whd_per%1000), phase_R.varhd_per/1000, abs(phase_R.varhd_per%1000));
		  printf("Swhpc: %03d.%03d varh: %03d.%03d\r\n", phase_S.whc_per/1000, abs(phase_R.whc_per%1000), phase_S.varhc_per/1000, abs(phase_R.varhc_per%1000));
		  printf("Swhpd: %03d.%03d varh: %03d.%03d\r\n", phase_S.whd_per/1000, abs(phase_R.whd_per%1000), phase_S.varhd_per/1000, abs(phase_R.varhd_per%1000));
		  printf("Twhpc: %03d.%03d varh: %03d.%03d\r\n", phase_T.whc_per/1000, abs(phase_R.whc_per%1000), phase_T.varhc_per/1000, abs(phase_R.varhc_per%1000));
		  printf("Twhpd: %03d.%03d varh: %03d.%03d\r\n", phase_T.whd_per/1000, abs(phase_R.whd_per%1000), phase_T.varhd_per/1000, abs(phase_R.varhd_per%1000));
			
			break;
			
		case 12: // RST: whper ef, varhper ef

            if(len != 57) break;
			memcpy((uint8_t*)&s32, &recbuff[0], 4); 	phase_R.whe_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[4], 4); 	phase_R.whf_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[8], 4); 	phase_R.varhe_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[12], 4); 	phase_R.varhf_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[16], 4); 	phase_S.whe_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[20], 4); 	phase_S.whf_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[24], 4); 	phase_S.varhe_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[28], 4); 	phase_S.varhf_per = s32;
			
			memcpy((uint8_t*)&s32, &recbuff[32], 4); 	phase_T.whe_per   = s32;
		  memcpy((uint8_t*)&s32, &recbuff[36], 4); 	phase_T.whf_per   = s32;
			memcpy((uint8_t*)&s32, &recbuff[40], 4); 	phase_T.varhe_per = s32;
			memcpy((uint8_t*)&s32, &recbuff[44], 4); 	phase_T.varhf_per = s32;
			phase_flags[12] = STATUS_UPDATED;
			
          printf("Rwhpe: %03d.%03d varh: %03d.%03d\r\n", phase_R.whe_per/1000, abs(phase_R.whe_per%1000), phase_R.varhe_per/1000, abs(phase_R.varhe_per%1000));
		  printf("Rwhpf: %03d.%03d varh: %03d.%03d\r\n", phase_R.whf_per/1000, abs(phase_R.whf_per%1000), phase_R.varhf_per/1000, abs(phase_R.varhf_per%1000));
		  printf("Swhpe: %03d.%03d varh: %03d.%03d\r\n", phase_S.whe_per/1000, abs(phase_R.whe_per%1000), phase_S.varhe_per/1000, abs(phase_R.varhe_per%1000));
		  printf("Swhpf: %03d.%03d varh: %03d.%03d\r\n", phase_S.whf_per/1000, abs(phase_R.whf_per%1000), phase_S.varhf_per/1000, abs(phase_R.varhf_per%1000));
		  printf("Twhpe: %03d.%03d varh: %03d.%03d\r\n", phase_T.whe_per/1000, abs(phase_R.whe_per%1000), phase_T.varhe_per/1000, abs(phase_R.varhe_per%1000));
		  printf("Twhpf: %03d.%03d varh: %03d.%03d\r\n", phase_T.whf_per/1000, abs(phase_R.whf_per%1000), phase_T.varhf_per/1000, abs(phase_R.varhf_per%1000));
			
			break;

	}
    return 1;
}

