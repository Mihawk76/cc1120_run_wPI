#include <stdio.h>
#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>

//char*id_getway[100];
int total_pairing;
int pairing_id[100];

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
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
 sprintf(select,"select id_paring from  %s where id_getway=%d",nm_table,kk); 
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

}

