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


	