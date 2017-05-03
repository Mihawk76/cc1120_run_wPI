#ifndef __PARING_H__
#define __PARING_H__

#include <my_global.h>
#include <mysql.h>

typedef struct{
	uint16_t th_id;
}TH_CONFIG;

typedef struct{
	uint16_t io_id;
	uint8_t input[8];
	uint16_t input_timeCount[8];
	uint8_t output[8];
	time_t ts;
}IO_CONFIG;

typedef struct{
	uint16_t kwh_id;
}KWH_CONFIG;

typedef struct{
	int suhu;
	int no_byte;
	int value_byte;
}IR_COMMAND;

typedef struct{
	uint16_t ir_id;
	int default_temp;
	time_t ts;
}IR_CONFIG;

typedef struct{
	uint16_t io_id;
	char* brand;
	struct tm start_operation;
	struct tm end_operation;
	int channel;
	int type;
}LAMP_CONFIG;

typedef struct{
	char* brand;
	uint16_t kwh_id;
	struct tm start_operation;
	struct tm end_operation;
}AC_CONFIG;

int total_pairing;
int pairing_id[1000];
int mysql_id;
struct tm start_operation;
struct tm end_operation;

extern TH_CONFIG th_config[20];
extern IO_CONFIG io_config[10];
extern IR_COMMAND ir_command[1000];
extern IR_CONFIG ir_config[20];
extern LAMP_CONFIG lamp_config[16];
extern AC_CONFIG ac_config[20];
extern KWH_CONFIG kwh_config[10];

void finish_with_error(MYSQL *con);
void get_th_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_io_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_kwh_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_lamp_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ac_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ir_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ir_command(char* server, char* user ,char* password ,char* dbname,char* nm_table,char* tipe_ac, int suhu);
void get_id_pairing(char* server, char* user ,char* password ,char* dbname,char* nm_table,int kk);
void get_id(char* server, char* user ,char* password ,char* dbname,char* nm_table);

#endif // __PARING_H__