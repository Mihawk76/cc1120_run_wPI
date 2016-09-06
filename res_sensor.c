#include <stdio.h>
#include <curl/curl.h>
int  res_kwh (char* location,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device)

{
   char scoreData[128];
   snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		snprintf(scoreData, sizeof scoreData, "VoltR=%d&VoltS=%d&VoltT=%d&CurrentR=%d&CurrentS=%d&CurrentT=%d&device=%d&nilai=%d",
		VoltR, VoltS, VoltT, CurrentR, CurrentS, CurrentT, device, nilai);
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
int trap_kwh (char* location, int channel, uint16_t id,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT
						,uint16_t CurrentR,uint16_t CurrentS,uint16_t CurrentT,uint16_t FrequencyR,uint16_t FrequencyS,uint16_t FrequencyT
						,uint16_t PowerFactorR,uint16_t PowerFactorS,uint16_t PowerFactorT,uint16_t WattR,uint16_t WattS,uint16_t WattT
						,uint16_t KwhPrdR,uint16_t KwhPrdS,uint16_t KwhPrdT,uint16_t TotalKwhPrd)

{
   char scoreData[128];
   snprintf(scoreData, sizeof scoreData, "location=%s",location);
	{
		int channelTrap = channel + 1;
		snprintf(scoreData, sizeof scoreData, "trapVariable=21,0,3,0:%d;21,%d,5,0:%d;21,%d,6,0:%d;21,%d,7,0:%d
						;21,%d,8,0:%d;21,%d,9,0:%d;21,%d,10,0:%d;21,%d,11,0:%d;21,%d,12,0:%d
						;21,%d,13,0:%d;21,%d,14,0:%d;21,%d,15,0:%d;21,%d,16,0:%d;21,%d,17,0:%d
						;21,%d,18,0:%d;21,%d,19,0:%d;21,%d,27,0:%d;21,%d,28,0:%d;21,%d,29,0:%d;21,%d,30,0:%d",
						id, channel, VoltR, channel, VoltT, channel, VoltS, channel
						, channel, CurrentR, channel, CurrentT, channel, CurrentS, channel
						, channel, FrequencyR, channel, FrequencyT, channel, FrequencyS, channel
						, channel, PowerFactorR, channel, PowerFactorT, channel, PowerFactorS, channel
						, channel, WattR, channel, WattT, channel, WattS, channel
						, channel, KwhPrdR, channel, KwhPrdT, channel, KwhPrdS, channel
						, channel, TotalKwhPrd
						);
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


	
