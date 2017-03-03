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
		"{\"Th_id\": %d,\"type\": %d,\"Gateway_id\": %d,\"temp1\": %d,\"temp2\": %d,\"temp3\": %d,\"humidity\": %d}",
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

int  res_val_kwh (char* location,  int id, int gateway, int kwh_index)

{
	switch(kwh_index){
		/*case 0: // RST: vrms, freq
		  //snprintf(scoreData, sizeof scoreData, "Th1=%d&Th2=%d&Th3=%d&Humidity=%d&device=%d&nilai=%d&gateway=%d",
		  snprintf(scoreData, sizeof scoreData, 
		  "{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RVRMS\": %03d.%01d,\"RFREQ\": %02d.%0d,\"SVRMS\": %03d.%01d,\"SFREQ\": %02d.%0d,\"TVRMS\": %03d.%01d,\"TFREQ\": %02d.%0d}",
		    gateway,
            id,	
			phase_R.vrms/10, phase_R.vrms%10, phase_R.freq/100, phase_R.freq%100,
			phase_S.vrms/10, phase_S.vrms%10, phase_S.freq/100, phase_S.freq%100,
			phase_T.vrms/10, phase_T.vrms%10, phase_T.freq/100, phase_T.freq%100
			);
		  printf("%s\n", scoreData);
		  break;*/
		
		case 1:	// RST: Ia, pfa, WATTa, VARa
		  snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.wa, phase_S.wa, phase_T.wa,
			phase_R.irmsa, 
			phase_S.irmsa,  
			phase_T.irmsa, (kwh_index-1));
		  printf("%s\n", scoreData);

			break;
		
		case 2: // RST: Ib, pfb, WATTb, VARb
		  snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.wb, phase_S.wb, phase_T.wb,
			phase_R.irmsb, 
			phase_S.irmsb,  
			phase_T.irmsb, (kwh_index-1));
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RIRMSB\": %03d.%03d,\"RPFB\": %d,\"RWATTB\": %d,\"RVARB\": %d,\"SIRMSB\": %03d.%03d,\"SPFB\": %d,\"SWATTB\": %d,\"SVARB\": %d,\"TIRMSB\": %03d.%03d,\"TPFB\": %d,\"TWATTB\": %d,\"TVARB\": %d}",
		    gateway, id,*/
		  printf("%s\n", scoreData);
			break;
		
		case 3:	// RST: Ic, pfc, WATTc, VARc
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.wc, phase_S.wc, phase_T.wc,
			phase_R.irmsc, 
			phase_S.irmsc,  
			phase_T.irmsc, (kwh_index-1));
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RIRMSC\": %03d.%03d,\"RPFC\": %d,\"RWATTC\": %d,\"RVARC\": %d,\"SIRMSC\": %03d.%03d,\"SPFC\": %d,\"SWATTC\": %d,\"SVARC\": %d,\"TIRMSC\": %03d.%03d,\"TPFC\": %d,\"TWATTC\": %d,\"TVARC\": %d}",
		    gateway, id,*/
		  printf("%s\n", scoreData);
			break;

		
		case 4: // RST: Id, pfd, WATTd, VARd
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.wd, phase_S.wd, phase_T.wd,
			phase_R.irmsd, 
			phase_S.irmsd,  
			phase_T.irmsd, (kwh_index-1));
		 /* "{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RIRMSD\": %03d.%03d,\"RPFD\": %d,\"RWATTD\": %d,\"RVARD\": %d,\"SIRMSD\": %03d.%03d,\"SPFD\": %d,\"SWATTD\": %d,\"SVARD\": %d,\"TIRMSD\": %03d.%03d,\"TPFD\": %d,\"TWATTD\": %d,\"TVARD\": %d}",
		    gateway, id,*/
		  printf("%s\n", scoreData);
			break;
		
		case 5:	// RST: Ie, pfe, WATTe, VARe
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.we, phase_S.we, phase_T.we,
			phase_R.irmse, 
			phase_S.irmse,  
			phase_T.irmse, (kwh_index-1));
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RIRMSE\": %03d.%03d,\"RPFE\": %d,\"RWATTE\": %d,\"RVARE\": %d,\"SIRMSE\": %03d.%03d,\"SPFE\": %d,\"SWATTE\": %d,\"SVARE\": %d,\"TIRMSE\": %03d.%03d,\"TPFE\": %d,\"TWATTE\": %d,\"TVARE\": %d}",
		    gateway, id,*/
		  printf("%s\n", scoreData);
			break;
		
		case 6: // RST: If, pff, WATTf, VARf
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Kwh_id\": %d,\"Gateway_id\": %d,\"WattPrdR\": %d,\"WattPrdS\": %d,\"WattPrdT\": %d,\"IR\": %d,\"IS\": %d,\"IT\": %d,\"Channel\": %d}",
		    id, gateway,
			phase_R.wf, phase_S.wf, phase_T.wf,
			phase_R.irmsf, 
			phase_S.irmsf,  
			phase_T.irmsf, (kwh_index-1));
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RIRMSF\": %03d.%03d,\"RPFF\": %d,\"RWATTF\": %d,\"RVARF\": %d,\"SIRMSF\": %03d.%03d,\"SPFF\": %d,\"SWATTF\": %d,\"SVARF\": %d,\"TIRMSF\": %03d.%03d,\"TPFF\": %d,\"TWATTF\": %d,\"TVARF\": %d}",
		    gateway, id,*/
		  printf("%s\n", scoreData);
			break;
			
		case 7:	// RST: whab varhab
         snprintf(scoreData, sizeof scoreData, 
		  "{\"data\":[{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d},{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d}]}",
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWHA\": %d,\"RVARHA\": %d,\"RWHB\": %d,\"RVARHB\": %d,\"SWHA\": %d,\"SVARHA\": %d,\"SWHB\": %d,\"SVARHB\": %d,\"TWHA\": %d,\"TVARHA\": %d,\"TWHB\": %d,\"TVARHB\": %d}",*/
		  id, gateway,
			phase_R.wha, phase_S.wha, phase_T.wha,
			phase_R.varha, phase_S.varha, phase_T.varha,
			((kwh_index-7)*2),
		  id, gateway,
			phase_R.whb, phase_S.whb, phase_T.whb,
			phase_R.varhb, phase_S.varhb, phase_T.varhb,
			(((kwh_index-7)*2)+1)
      /*phase_R.wha, phase_R.varha,
			phase_R.whb, phase_R.varhb,
			phase_S.wha, phase_S.varha,
			phase_S.whb, phase_S.varhb,
			phase_T.wha, phase_T.varha,
			phase_T.whb, phase_T.varhb*/
			);
		  printf("%s\n", scoreData);
			break;
		
		case 8: // RST: whcd varhcd
          snprintf(scoreData, sizeof scoreData, 
		  "{\"data\":[{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d},{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d}]}",
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWHC\": %d,\"RVARHC\": %d,\"RWHD\": %d,\"RVARHD\": %d,\"SWHC\": %d,\"SVARHC\": %d,\"SWHD\": %d,\"SVARHD\": %d,\"TWHC\": %d,\"TVARHC\": %d,\"TWHD\": %d,\"TVARHD\": %d}",*/
		  id, gateway,
			phase_R.whc, phase_S.whc, phase_T.whc,
			phase_R.varhc, phase_S.varhc, phase_T.varhc,
			((kwh_index-7)*2),
		  id, gateway,
			phase_R.whd, phase_S.whd, phase_T.whd,
			phase_R.varhd, phase_S.varhd, phase_T.varhd,
			(((kwh_index-7)*2)+1)
		  /*  gateway, id,
			phase_R.whc, phase_R.varhc,
			phase_R.whd, phase_R.varhd,
			phase_S.whc, phase_S.varhc,
			phase_S.whd, phase_S.varhd, 
			phase_T.whc, phase_T.varhc, 
			phase_T.whd, phase_T.varhd*/
			);
		  printf("%s\n", scoreData);
			break;
		
		case 9:	// RST: whef varhef
          snprintf(scoreData, sizeof scoreData, 
		  "{\"data\":[{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d},{\"Kwh_id\": %d,\"Gateway_id\": %d,\"Rwh\": %d,\"Swh\": %d,\"Twh\": %d,\"Rvarh\": %d,\"Svarh\": %d,\"Tvarh\": %d,\"Channel\":%d}]}",
		  /*"{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWHE\": %d,\"RVARHE\": %d,\"RWHF\": %d,\"RVARHF\": %d,\"SWHE\": %d,\"SVARHE\": %d,\"SWHF\": %d,\"SVARHF\": %d,\"TWHE\": %d,\"TVARHE\": %d,\"TWHF\": %d,\"TVARHF\": %d}",*/
		  id, gateway,
			phase_R.whe, phase_S.whe, phase_T.whe,
			phase_R.varhe, phase_S.varhe, phase_T.varhe,
			((kwh_index-7)*2),
		  id, gateway,
			phase_R.whf, phase_S.whf, phase_T.whf,
			phase_R.varhf, phase_S.varhf, phase_T.varhf,
			(((kwh_index-7)*2)+1)
		    /*gateway, id,
			phase_R.whe, phase_R.varhe,
			phase_R.whf, phase_R.varhf,
			phase_S.whe, phase_S.varhe,
			phase_S.whf, phase_S.varhf,
			phase_T.whe, phase_T.varhe,
			phase_T.whf, phase_T.varhf);
		  printf("%s\n", scoreData*/
			);
			break;
		
		/*case 10:	// RST: whper ab, varhper ab
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWPERA\": %d,\"RVARPERA\": %d,\"RWPERB\": %d,\"RVARPERB\": %d,\"SWPERA\": %d,\"SVARPERA\": %d,\"SWPERB\": %d,\"SVARPERB\": %d,\"TWPERA\": %d,\"TVARPERA\": %d,\"TWPERB\": %d,\"TVARPERB\": %d}",
		    gateway, id,
            phase_R.wha_per, phase_R.varha_per,
			phase_R.whb_per, phase_R.varhb_per,
			phase_S.wha_per, phase_S.varha_per,
			phase_S.whb_per, phase_S.varhb_per,
			phase_T.wha_per, phase_T.varha_per,
			phase_T.whb_per, phase_T.varhb_per);
		  printf("%s\n", scoreData);
			break;*/
		
		/*case 11: // RST: whper cd, varhper cd
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWPERC\": %d,\"RVARPERC\": %d,\"RWPERD\": %d,\"RVARPERD\": %d,\"SWPERC\": %d,\"SVARPERC\": %d,\"SWPERD\": %d,\"SVARPERD\": %d,\"TWPERC\": %d,\"TVARPERC\": %d,\"TWPERD\": %d,\"TVARPERD\": %d}",
		    gateway, id,
			phase_R.whc_per, phase_R.varhc_per,
			phase_R.whd_per, phase_R.varhd_per,
			phase_S.whc_per, phase_S.varhc_per,
			phase_S.whd_per, phase_S.varhd_per,
			phase_T.whc_per, phase_T.varhc_per,
			phase_T.whd_per, phase_T.varhd_per);
		  printf("%s\n", scoreData);
			break;*/
			
		/*case 12: // RST: whper ef, varhper ef
          snprintf(scoreData, sizeof scoreData, 
		  "{\"Gateway_id\": %d,\"Kwh_id\": %d,\"RWPERE\": %d,\"RVARPERE\": %d,\"RWPERF\": %d,\"RVARPERF\": %d,\"SWPERE\": %d,\"SVARPERE\": %d,\"SWPERF\": %d,\"SVARPERF\": %d,\"TWPERE\": %d,\"TVARPERE\": %d,\"TWPERF\": %d,\"TVARPERF\": %d}",
		    gateway, id,
			phase_R.whe_per, phase_R.varhe_per, 
			phase_R.whf_per, phase_R.varhf_per,
			phase_S.whe_per, phase_S.varhe_per,
			phase_S.whf_per, phase_S.varhf_per,
			phase_T.whe_per, phase_T.varhe_per,
			phase_T.whf_per, phase_T.varhf_per);
		  printf("%s\n", scoreData);
			break;*/
	}
		
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


	
