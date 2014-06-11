// torbot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CURL *curl;
	CURLcode res;
	static const char *bodyfilename = "body.out";
	FILE *bodyfile;
 
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://mobilelabs.herokuapp.com/get_torred/");
		/* example.com is redirected, so we tell libcurl to follow redirection */ 
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1");
		curl_easy_setopt(curl, CURLOPT_PROXYPORT, 9050);
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
		/* send all data to this function  */ 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		bodyfile = fopen(bodyfilename,"wb");
		if (bodyfile == NULL) {
			curl_easy_cleanup(curl);
			return -1;
		}
		/* we want the body be written to this file handle instead of stdout */ 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyfile);
 
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
 
		/* close the body file */ 
		fclose(bodyfile);

		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}


	return 0;
}

