#define _GNU_SOURCE 1
#include <stdio.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include "app.h"
#include "paring.h"

int total_pairing;
int pairing_id[1000];
int mysql_id;
struct tm start_operation;
struct tm end_operation;

//IR_COMMAND ir_command[1000];

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}
void get_th_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];

 sprintf(select,"select id from %s where id_location=%d", nm_table, gateway_id); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	//int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	//int i;
	int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {	
			th_nodes[a].id = atoi(row[0]?row[0]:"NULL");	
			printf("th id %d\n", th_nodes[a].id);
			a++;
  		//printf("\n"); 
    }
	}
	mysql_close(con);
}

void get_io_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{      
  MYSQL *con = mysql_init(NULL);
  struct timespec ts;
	
  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select id,io_id from %s where Gateway_id=%d", nm_table, gateway_id); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	//int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	//int i;
	int a=0;
	clock_gettime(CLOCK_REALTIME, &ts);
	
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {	
			io_nodes[a].index = atoi(row[0]?row[0]:0);
      io_nodes[a].io_id = atoi(row[1]?row[1]:0);			
			printf("io id %d %02X\n", io_nodes[a].index ,io_nodes[a].io_id);
			
      io_nodes[a].inputNum = 0;
			memset(io_nodes[a].input,0,sizeof(io_nodes[a].input));
			memset((uint8_t*)&io_nodes[a].input_timeCount,0,sizeof(io_nodes[a].input_timeCount));
      io_nodes[a].outputNum = 0;
			memset(io_nodes[a].outputSet,0x02,sizeof(io_nodes[a].input));
			memset(io_nodes[a].outputGet,0x00,sizeof(io_nodes[a].input));
			memset((uint8_t*)&io_nodes[a].output_timeCount,0,sizeof(io_nodes[a].input_timeCount));
      io_nodes[a].ts = ts.tv_sec;
      io_nodes[a].updated = STATUS_CLEARED;
			
			a++;
  		//printf("\n"); 
    }
	}
	mysql_close(con);
}

void get_kwh_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select kwh_id from %s where id_location=%d", nm_table, gateway_id); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	//int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	//int i;
	int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {	
			kwh_ID = atoi(row[0]?row[0]:"NULL");	
			printf("kwh id %02X\n", kwh_ID);
			a++;
  		//printf("\n"); 
    }
	}
	mysql_close(con);
}


void get_ir_config(char* server, char* user ,char* password ,char* dbname,char* nm_table,uint16_t gateway_id)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select ir_name, set_default from  %s where id_location=%d", nm_table, gateway_id); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	//int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	//int i;
	int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {
     /*for (i = 0; i < num_fields; i++)
        {
      total_pairing = a+i;     
			pairing_id[total_pairing]= atoi(row[i]?row[i]:"NULL");
			printf("%d %d\n", pairing_id[total_pairing], total_pairing);
        //printf("data_library:%02X \n",pairing_id[total_pairing]);
        }
		a=a+i;*/
			ir_config[a].ir_id = atoi(row[0]?row[0]:"NULL");	
			ir_config[a].default_temp = atoi(row[1]?row[1]:"NULL");
			ir_config[a].set_temp = atoi(row[1]?row[1]:"NULL");
			printf("%02X %d\n", ir_config[a].ir_id, ir_config[a].default_temp);
			a++;
  		//printf("\n"); 
			// if (ir_command[a].suhu == 30){
				// break;
			// }
    }
	}
	mysql_close(con);
}

void get_ir_command(char* server, char* user ,char* password ,char* dbname,char* nm_table,char* tipe_ac, int suhu, uint8_t * bufout)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select suhu, no_byte, value_byte from  %s where tipe_ac='%s' && suhu=%d", nm_table, tipe_ac, suhu); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	//int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	//int i;
	//int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {
     /*for (i = 0; i < num_fields; i++)
        {
      total_pairing = a+i;     
			pairing_id[total_pairing]= atoi(row[i]?row[i]:"NULL");
			printf("%d %d\n", pairing_id[total_pairing], total_pairing);
        //printf("data_library:%02X \n",pairing_id[total_pairing]);
        }
		a=a+i;*/
    *bufout++  = atoi(row[2]?row[2]:"NULL")&0xff;
/* 		
			ir_command[a].suhu = atoi(row[0]?row[0]:"NULL");	
			ir_command[a].no_byte = atoi(row[1]?row[1]:"NULL");
			ir_command[a].value_byte = atoi(row[2]?row[2]:"NULL");
			printf("%d %d %02X\n", ir_command[a].suhu, ir_command[a].no_byte, ir_command[a].value_byte);
			a++;
  		//printf("\n"); 
			if (ir_command[a].suhu == 30){
				break;
			}
			 */
    }
	}
	mysql_close(con);
}

void get_id_pairing(char* server, char* user ,char* password ,char* dbname,char* nm_table,int kk)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select id_paring from  %s where tipe_ac=%d",nm_table,kk); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	int i;
	int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {
     for (i = 0; i < num_fields; i++)
        {
      total_pairing = a+i;     
			pairing_id[total_pairing]=atoi(row[i]);
        //printf("data_library:%02X \n",pairing_id[total_pairing]);
        }
		a=a+i;
  	//printf("\n"); 
    }
	}
	mysql_close(con);
}
void get_id(char* server, char* user ,char* password ,char* dbname,char* nm_table)
{      
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  if (mysql_real_connect(con, server, user, password, 
          dbname, 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }    
	char select[100];
 sprintf(select,"select value from %s where label='id'",nm_table); 
 if (mysql_query(con,select)) 
 { 
      finish_with_error(con);
 }


 MYSQL_RES *result = mysql_store_result(con);  
 if (result == NULL) 
 {
      finish_with_error(con);
 }

	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	int i;
	int a=0;
	{
   while((row = mysql_fetch_row(result)) != NULL)
    {
     for (i = 0; i < num_fields; i++)
        {
      total_pairing = a+i;     
			mysql_id=atoi(row[i]);
        //printf("data_library:%02X \n",pairing_id[total_pairing]);
        }
		a=a+i;
  	//printf("\n"); 
    }
	}
	mysql_close(con);
}

