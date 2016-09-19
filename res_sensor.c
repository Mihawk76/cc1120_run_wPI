#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
char temp[128];
char scoreData[300];
int  res_kwh (char* location,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT,uint16_t CurrentR,
								uint16_t CurrentS,uint16_t CurrentT, int nilai,int device)

{
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

int trap_th_discovery (char* location, int uID, uint16_t gateway_ID, uint16_t th_ID, int sensorUsed)
{
	// uID th keberapa yang konek ke si gateway
	// thID dan Gateway ID sudah jelas
	// sensorUsed id 4 bit, 1111 (kelembapan, suhu, suhu, suhu) 1 nyala 0 mati
	// btw trap_th_connect sama dengan trap_th_discovery
	snprintf(scoreData, sizeof scoreData, "trapVariable=18.0.3.0:%d;18.0.54.0:%d;18.%d.39.0:%d",
					gateway_ID, th_ID, uID, sensorUsed); 
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

int trap_th_disconnect (char* location, int uID, uint16_t gateway_ID, uint16_t th_ID)
{
	// uID th keberapa yang konek ke si gateway
	// thID dan Gateway ID sudah jelas
	// sensorUsed id 4 bit, 1111 (kelembapan, suhu, suhu, suhu) 1 nyala 0 mati
	// btw trap_th_connect sama dengan trap_th_discovery
	snprintf(scoreData, sizeof scoreData, "trapVariable=18,0,3,0:%d;18,0,54,0:%d",
					gateway_ID, th_ID); 
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

int trap_th (char* location, int uID, uint16_t gateway_ID, uint16_t th_ID, uint16_t dIn1, uint16_t dIn2
						, uint16_t hummidity, uint16_t th1, uint16_t th2, uint16_t th3, int16_t rssi
						)
{
	// uID th keberapa yang konek ke si gateway
	// thID dan Gateway ID sudah jelas
	// sensorUsed id 4 bit, 1111 (kelembapan, suhu, suhu, suhu) 1 nyala 0 mati
	// btw trap_th_connect sama dengan trap_th_discovery
	printf("rssi %d\n", rssi);
	snprintf(scoreData, sizeof scoreData, "18.0.3.0:%d;18.%d.1.0:%d;18.%d.10.0:%d;18.%d.12.0:%d",
					gateway_ID, uID, th_ID, uID, dIn1, uID, dIn2 ); 
	snprintf(temp, sizeof scoreData,  ";18.%d.40.0:%d%d;18.%d.41.0:%d;18.%d.42.0:%d;18.%d.43.0:%d"
					, uID, hummidity, th1, uID, th2, uID, th3, uID, rssi);
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
int trap_kwh (char* location, int channel, uint16_t id,uint16_t VoltR,uint16_t VoltS,uint16_t VoltT
						,uint16_t CurrentR,uint16_t CurrentS,uint16_t CurrentT,uint16_t FrequencyR,uint16_t FrequencyS,uint16_t FrequencyT
						,uint16_t PowerFactorR,uint16_t PowerFactorS,uint16_t PowerFactorT,uint16_t WattR,uint16_t WattS,uint16_t WattT
						,uint16_t KwhPrdR,uint16_t KwhPrdS,uint16_t KwhPrdT,uint16_t TotalKwhPrd)

{
		int channelTrap = channel + 1;
		/*snprintf(scoreData, sizeof scoreData, "",
						, channel
						, channel
						, channel
						,  channel
						, 
						);*/
		snprintf(scoreData, sizeof scoreData, "trapVariable=21.0.3.0:%d;21.%d.5.0:%d;21.%d.6.0:%d;21.%d.7.0:%d",
					id, channelTrap, VoltR, channelTrap, VoltS, channelTrap, VoltT); 
		snprintf(temp, sizeof scoreData,  ";21.%d.8.0:%d;21.%d.9.0:%d;21.%d.10.0:%d"
						, channelTrap, CurrentR, channelTrap, CurrentS, channelTrap, CurrentT);
		strcat(scoreData, temp);
		printf("scoreData %s\n", scoreData);	
		snprintf(temp, sizeof scoreData,  ";21.%d.11.0:%d;21.%d.12.0:%d;21.%d.13.0:%d"
						, channelTrap, FrequencyR, channelTrap, FrequencyS, channelTrap, FrequencyT);
		strcat(scoreData, temp);
		printf("scoreData %s\n", scoreData);	
		snprintf(temp, sizeof scoreData,  ";21.%d.14.0:%d;21.%d.15.0:%d;21.%d.16.0:%d"
						, channelTrap, PowerFactorR, channelTrap, PowerFactorS, channelTrap, PowerFactorT);
		strcat(scoreData, temp);
		printf("scoreData %s\n", scoreData);	
		snprintf(temp, sizeof scoreData,  ";21.%d.17.0:%d;21.%d.18.0:%d;21.%d.19.0:%d"
						, channelTrap, WattR, channelTrap, WattS, channelTrap, WattT);
		strcat(scoreData, temp);
		printf("scoreData %s\n", scoreData);	
		snprintf(temp, sizeof scoreData,  ";21.%d.27.0:%d;21.%d.28.0:%d;21.%d.29.0:%d;21.%d.30.0:%d"
						,channelTrap, KwhPrdR, channelTrap, KwhPrdS, channelTrap, KwhPrdT, channelTrap, TotalKwhPrd);
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
int  res_th (char* location,uint16_t Th1,uint16_t Th2,uint16_t Th3,uint16_t Humidity, int nilai,int device)

{
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


	
