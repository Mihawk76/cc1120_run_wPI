#ifndef __KWH_PARAMS_H__
#define __KWH_PARAMS_H__

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

extern ADE7816_STRUCT phases[3];
#define phase_R phases[0]
#define phase_S phases[1]
#define phase_T phases[2]
extern time_t  phase_ts[14];
extern uint8_t phase_flags[14];

int get_params_value(uint8_t* recbuff, uint8_t subcmd, int len);

#endif //#ifndef __KWH_PARAMS_H__

