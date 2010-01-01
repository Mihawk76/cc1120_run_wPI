#include <stdio.h>
#include <curl/curl.h>

char scoreData[10000];
char temp[1000];	


int test (int kwh[])
{
	int channel;
	for (channel = 0; channel <=18; channel++)
	{
		printf("Nilai Kwh %d %d \n", channel , kwh[channel]);
	}
	return 0;
}
int res_R_I (char* location, int32_t CurrentR[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "jsonR={\"Data\":[{");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "\"CurrentR\":%d", 
		CurrentR[Channel]);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "}]}&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_S_I (char* location, int32_t CurrentS[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "jsonS={\"Data\":[{");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "\"CurrentS\":%d", 
		CurrentS[Channel]);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "}]}&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_T_I (char* location, int32_t CurrentT[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "jsonT={\"Data\":[{");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "\"CurrentT\":%d", 
		CurrentT[Channel]);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "}]}&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_R_I_V_PF (char* location,	int32_t VoltR[6], int32_t CurrentR[6], int32_t PowerFactorR[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VoltR%d=%d CurrentR%d=%d PowerFactorR%d=%d\n", 
		Channel, VoltR[Channel], Channel, CurrentR[Channel], Channel, PowerFactorR[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_S_I_V_PF (char* location,	int32_t VoltS[6], int32_t CurrentS[6], int32_t PowerFactorS[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VoltS%d=%d CurrentS%d=%d PowerFactorS%d=%d\n", 
		Channel, VoltS[Channel], Channel, CurrentS[Channel], Channel, PowerFactorS[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_T_I_V_PF (char* location,	int32_t VoltT[6], int32_t CurrentT[6], int32_t PowerFactorT[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VoltT%d=%d CurrentT%d=%d PowerFactorT%d=%d\n", 
		Channel, VoltT[Channel], Channel, CurrentT[Channel], Channel, PowerFactorT[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_R_Watt_Var (char* location,	int32_t WattR[6], int32_t VarR[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "WattR%d=%d VarR%d=%d\n", 
		Channel, WattR[Channel], Channel, VarR[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_S_Watt_Var (char* location,	int32_t WattS[6], int32_t VarS[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "WattS%d=%d VarS%d=%d\n", 
		Channel, WattS[Channel], Channel, VarS[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_T_Watt_Var (char* location,	int32_t WattT[6], int32_t VarT[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "WattT%d=%d VarT%d=%d\n", 
		Channel, WattT[Channel], Channel, VarT[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_WattPrd_I (char* location,	int32_t WattPrdR[6],	int32_t WattPrdS[6],	int32_t WattPrdT[6],	
														int32_t IR[6],	int32_t IS[6],	int32_t IT[6], int nilai ,int device, int gateway, int Channel)
{
	int i = 0;
	snprintf(scoreData, sizeof scoreData, "[");
	for(i=0;i<2;i++)
	{
		snprintf(temp, sizeof scoreData, "{\"Kwh_id\":%d,\"Gateway_id\":%d,\"WattPrdR\":%d,\"WattPrdS\":%d,\"WattPrdT\":%d,\"Channel\":%d,\"id\":0,", 
		device, gateway, WattPrdR[Channel+i], WattPrdS[Channel+i], WattPrdT[Channel+i], Channel+i);
		strcat(scoreData, temp);
		snprintf(temp, sizeof scoreData, "\"IR\":%d, \"IS\":%d, \"IT\":%d}", 
		WattPrdR[Channel+i], WattPrdS[Channel+i], WattPrdT[Channel+i]);
		strcat(scoreData, temp);
		if(i<1){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "]");
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	/*CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	  curl_easy_setopt(curl, CURLOPT_URL, location);
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
	  res = curl_easy_perform(curl);
	  if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	*/return 0;
}
int res_R_WattPrd_WattHour (char* location,	int32_t WattPrdR[6], int32_t WattHourR[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "[");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "{\"Kwh_id\":%d,\"Gateway_id\":%d,\"WattPrdR\":%d,\"WattHourR\":%d,\"Channel\":%d,\"id\":0}", 
		device, gateway, WattPrdR[Channel], WattHourR[Channel], Channel);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "]");
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	  curl_easy_setopt(curl, CURLOPT_URL, location);
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
	  res = curl_easy_perform(curl);
	  if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_S_WattPrd_WattHour (char* location,	int32_t WattPrdS[6], int32_t WattHourS[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "[");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "{\"Kwh_id\":%d,\"Gateway_id\":%d,\"WattPrdS\":%d,\"WattHourS\":%d,\"Channel\":%d,\"id\":0}", 
		device, gateway, WattPrdS[Channel], WattHourS[Channel], Channel);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "]");
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	  curl_easy_setopt(curl, CURLOPT_URL, location);
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
	  res = curl_easy_perform(curl);
	  if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_T_WattPrd_WattHour (char* location,	int32_t WattPrdT[6], int32_t WattHourT[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "[");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "{\"Kwh_id\":%d,\"Gateway_id\":%d,\"WattPrdT\":%d,\"WattHourT\":%d,\"Channel\":%d,\"id\":0}", 
		device, gateway, WattPrdT[Channel], WattHourT[Channel], Channel);
		strcat(scoreData, temp);
		if(Channel<5){
		snprintf(temp, sizeof scoreData, ",");
		strcat(scoreData, temp);
		}
	}
	snprintf(temp, sizeof scoreData, "]");
	strcat(scoreData, temp);
	printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	  curl_easy_setopt(curl, CURLOPT_URL, location);
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
	  res = curl_easy_perform(curl);
	  if(res != CURLE_OK)
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_R_VarPrd_VarHour (char* location,	int32_t VarPrdR[6], int32_t VarHourR[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VarPrdR%d=%d VarHourR%d=%d\n", 
		Channel, VarPrdR[Channel], Channel, VarHourR[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_S_VarPrd_VarHour (char* location,	int32_t VarPrdS[6], int32_t VarHourS[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VarPrdS%d=%d VarHourS%d=%d\n", 
		Channel, VarPrdS[Channel], Channel, VarHourS[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}
int res_T_VarPrd_VarHour (char* location,	int32_t VarPrdT[6], int32_t VarHourT[6], int nilai ,int device, int gateway)
{
	int Channel = 0;
	snprintf(scoreData, sizeof scoreData, "data=");
	for(Channel=0;Channel<6;Channel++)
	{
		snprintf(temp, sizeof scoreData, "VarPrdT%d=%d VarHourT%d=%d\n", 
		Channel, VarPrdT[Channel], Channel, VarHourT[Channel]);
		strcat(scoreData, temp);
	}
	snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
	strcat(scoreData, temp);
	//printf("%s\n", scoreData);
	//printf("location %s\n", location);	
	CURL *curl;
	CURLcode res; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
  if(curl) {     
	   curl_easy_setopt(curl, CURLOPT_URL, location);
	   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
	   res = curl_easy_perform(curl);
	   if(res != CURLE_OK)
	   fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	   curl_easy_cleanup(curl);
	}   
	curl_global_cleanup();
	return 0;
}

int res_kwh_array (char* location,int32_t KwhR[18],int32_t KwhS[18],int32_t KwhT[18],
								int32_t VoltR[18],int32_t VoltS[18],int32_t VoltT[18],int32_t CurrentR[18],
								int32_t CurrentS[18],int32_t CurrentT[18], int nilai ,int device, int gateway)

{
	int Channel = 0;
	{
		snprintf(scoreData, sizeof scoreData, "data=");
		for(Channel=0;Channel<18;Channel++)
		{
			snprintf(temp, sizeof scoreData, "KwhR%d=%d KwhS%d=%d KwhT%d=%d VoltR%d=%d VoltS%d=%d VoltT%d=%d CurrentR%d=%d CurrentS%d=%d CurrentT%d=%d\n",
			Channel, KwhR[Channel], Channel, KwhS[Channel], Channel, KwhT[Channel], 
			Channel, VoltR[Channel], Channel, VoltS[Channel], Channel, VoltT[Channel], 
			Channel, CurrentR[Channel], Channel,CurrentS[Channel], Channel, CurrentT[Channel]);
			strcat(scoreData, temp);
		}
		snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d&gateway=%d", device, nilai, Channel, gateway);
		strcat(scoreData, temp);
		//printf("%s\n", scoreData);
		//printf("location %s\n", location);	
		  CURL *curl;
		  CURLcode res; 
		  curl_global_init(CURL_GLOBAL_ALL);
		  curl = curl_easy_init();
		  if(curl) { 
		     
			    curl_easy_setopt(curl, CURLOPT_URL, location);
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
			    res = curl_easy_perform(curl);
			    if(res != CURLE_OK)
			    fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			    curl_easy_cleanup(curl);
		  	}   
		  curl_global_cleanup();
		
	}
	return 0;
}
int  res_kwh_2 (char* location,uint16_t* VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device, int Channel)

{
   //snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		snprintf(scoreData, sizeof scoreData, "data=1001&nilai=%d&Channel=%d",
		nilai, Channel);
		printf("location %s\n", location);
		printf("%s\n", scoreData);
		  CURL *curl;
		  CURLcode res; 
		  curl_global_init(CURL_GLOBAL_ALL);
		  curl = curl_easy_init();
		  if(curl) { 
		     
			    curl_easy_setopt(curl, CURLOPT_URL, location);
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
			    res = curl_easy_perform(curl);
			    if(res != CURLE_OK)
			    fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			    curl_easy_cleanup(curl);
		  	}   
		  curl_global_cleanup();
		
	}
	return 0;
}
int  res_kwh (char* location,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device, int Channel)

{
   //snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		snprintf(scoreData, sizeof scoreData, "VoltR=%d&VoltS=%d&VoltT=%d&CurrentR=%d&CurrentS=%d&CurrentT=%d&device=%d&nilai=%d&Channel=%d",
		VoltR, VoltS, VoltT, CurrentR, CurrentS, CurrentT, device, nilai, Channel);
		printf("%s\n", scoreData);
		  CURL *curl;
		  CURLcode res; 
		  curl_global_init(CURL_GLOBAL_ALL);
		  curl = curl_easy_init();
		  if(curl) { 
		     
			    curl_easy_setopt(curl, CURLOPT_URL, location);
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
			    res = curl_easy_perform(curl);
			    if(res != CURLE_OK)
			    fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			    curl_easy_cleanup(curl);
		  	}   
		  curl_global_cleanup();
		
	}
	return 0;
}

int  res_th (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device, int gateway)

{
		//snprintf(scoreData, sizeof scoreData, "Th1=%d&Th2=%d&Th3=%d&Humidity=%d&device=%d&nilai=%d&gateway=%d",
		snprintf(scoreData, sizeof scoreData, 
		"{\"Th_id\": %d,\"type\": %d,\"Gateway_id\": %d,\"temp1\": %d,\"temp2\": %d,\"temp3\": %d,\"humidity\": %d,\"id\": 0}",
		device, nilai, gateway, Th1, Th2, Th3, Humidity);
		//printf("%d\n", sizeof scoreData);
		printf("%s\n", scoreData);
		  CURL *curl;
		  CURLcode res; 
		  curl_global_init(CURL_GLOBAL_ALL);
		  curl = curl_easy_init();
		  if(curl) { 
		     	curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
					struct curl_slist *headers = NULL;
					headers = curl_slist_append(headers, "Accept: application/json");
					headers = curl_slist_append(headers, "Content-Type: application/json");
					headers = curl_slist_append(headers, "charsets: utf-8");
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
		     	//curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
			    curl_easy_setopt(curl, CURLOPT_URL, location);
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
			    res = curl_easy_perform(curl);
			    if(res != CURLE_OK)
			    fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			    curl_easy_cleanup(curl);
		  	}   
		  curl_global_cleanup();
	return 0;
}

int trap_th (char* location, int uID, char* gateway_ID, uint16_t th_ID, uint16_t dIn1, uint16_t dIn2
            , uint16_t hummidity, uint16_t th1, uint16_t th2, uint16_t th3, int16_t rssi
            )
{
  // uID th keberapa yang konek ke si gateway
  // thID dan Gateway ID sudah jelas
  // sensorUsed id 4 bit, 1111 (kelembapan, suhu, suhu, suhu) 1 nyala 0 mati
  // btw trap_th_connect sama dengan trap_th_discovery
  printf("rssi %d\n", rssi); 
  snprintf(scoreData, sizeof scoreData, "18.0.3.0:%s;18.%d.1.0:%d;18.%d.10.0:%d;18.%d.12.0:%d",
          gateway_ID, uID, th_ID, uID, dIn1, uID, dIn2 ); 
  snprintf(temp, sizeof scoreData,  ";18.%d.40.0:%d;18.%d.41.0:%d;18.%d.42.0:%d;18.%d.43.0:%d"
          , uID, th1, uID, th2, uID, th3, uID, rssi);
  strcat(scoreData, temp); 
  printf("scoreData %s\n", scoreData);
  CURL *curl;
  CURLcode res; 
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) { 
    curl_easy_setopt(curl, CURLOPT_URL, location);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  return 0;
}

int  res_th_2 (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device)

{
   snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		snprintf(scoreData, sizeof scoreData, "Th1=%d&Th2=%d&Th3=%d&Humidity=%d&device=%d&nilai=%d",
		Th1, Th2, Th3, Humidity, device, nilai);
		printf("%s\n", scoreData);
		  CURL *curl;
		  CURLcode res; 
		  curl_global_init(CURL_GLOBAL_ALL);
		  curl = curl_easy_init();
		  if(curl) { 
		     
			    curl_easy_setopt(curl, CURLOPT_URL, location);
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, scoreData);
			    res = curl_easy_perform(curl);
			    if(res != CURLE_OK)
			    fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			    curl_easy_cleanup(curl);
		  	}   
		  curl_global_cleanup();
		
	}
	return 0;
}


	
