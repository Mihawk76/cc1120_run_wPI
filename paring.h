#include<stdio.h>
#ifndef paring_h_
#define paring_h_

int mysql_id;
void get_ir_command(char* server, char* user ,char* password ,char* dbname,char* nm_table,char* tipe_ac);
void get_id(char* server, char* user ,char* password ,char* dbname,char* nm_table);
typedef struct{
  int suhu;
  int no_byte;
  int value_byte;
}IR_COMMAND;

#endif // paring_h_
