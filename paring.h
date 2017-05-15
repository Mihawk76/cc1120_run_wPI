#ifndef __PARING_H__
#define __PARING_H__

#include <my_global.h>
#include <mysql.h>

typedef struct{
	int suhu;
	int no_byte;
	int value_byte;
}IR_COMMAND;

int total_pairing;
int pairing_id[1000];
int mysql_id;
struct tm start_operation;
struct tm end_operation;

extern IR_COMMAND ir_command[1000];

void finish_with_error(MYSQL *con);
void get_th_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_io_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_kwh_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ac_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
void get_ir_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id);
//void get_ir_command(char* server, char* user ,char* password ,char* dbname,char* nm_table,char* tipe_ac, int suhu);
void get_ir_command(char* server, char* user ,char* password ,char* dbname,char* nm_table,char* tipe_ac, int suhu, uint8_t * bufout);
void get_id_pairing(char* server, char* user ,char* password ,char* dbname,char* nm_table,int kk);
void get_id(char* server, char* user ,char* password ,char* dbname,char* nm_table);

#endif // __PARING_H__