#include <stdio.h>
#include <curl/curl.h>

int test (int kwh[])
{
	int channel;
	for (channel = 0; channel <=18; channel++)
	{
		printf("Nilai Kwh %d %d \n", channel , kwh[channel]);
	}
	return 0;
}
int res_kwh_array (char* location,uint16_t VoltR[18],uint16_t VoltS[18],uint16_t VoltT[18],uint16_t CurrentR[18],
								uint16_t CurrentS[18],uint16_t CurrentT[18], int nilai,int device)

{
  char scoreData[10000];
	char temp[500];	
	int Channel = 0;
	{
		snprintf(scoreData, sizeof scoreData, "data=");
		for(Channel=0;Channel<18;Channel++)
		{
			snprintf(temp, sizeof scoreData, "VoltR%d=%d VoltS%d=%d VoltT%d=%d CurrentR%d=%d CurrentS%d=%d CurrentT%d=%d\n",
			Channel, VoltR[Channel], Channel, VoltS[Channel], Channel, VoltT[Channel], Channel, CurrentR[Channel], 
			Channel, VoltR[Channel], Channel, VoltS[Channel], Channel, VoltT[Channel], Channel, CurrentR[Channel], 
			Channel,CurrentS[Channel], Channel, CurrentT[Channel]);
			strcat(scoreData, temp);
		}
		snprintf(temp, sizeof scoreData, "&device=%d&nilai=%d&Channel=%d", device, nilai, Channel);
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
   char scoreData[128];
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
   char scoreData[128];
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
int  res_th (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device)

{
   char scoreData[128];
   snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		snprintf(scoreData, sizeof scoreData, "Th1=%d&Th2=%d&Th3=%d&Humidity=%d&device=%d&nilai=%d",
		Th1, Th2, Th3, Humidity, device, nilai);
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


	
