/* Example 01 :
This example show how to put the result of a curl request inside a variable.
This example also shows how to read available images and all their informations.

    /!\ : This example only works with libcurl 7.75 or above
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

// Function that will write the data inside a variable
size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;

  char **response_ptr = (char**)userp;
  *response_ptr = strndup(data, realsize);
  
  return realsize; 
}

void main(){
  // Getting the access key / secret key from environement
  const char *ak_sk = getenv("AKSK");

  // Variable that will store the data
  struct memory chunk;

  
  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();

  // Setting the url
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadImages");

  // Let's see what curl is doing
  curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);

  // Empty post field to indicate we want to send a post request
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, "");

  // Telling curl to use the write_data function to write over response
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(c, CURLOPT_WRITEDATA, (void *)&chunk);

  // For authentification we specify the method and our acces key / secret key
  curl_easy_setopt(c, CURLOPT_AWS_SIGV4, "osc");
  curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk);

  res = curl_easy_perform(c);

  curl_easy_cleanup(c);

  printf("\n\n Output : %s", chunk.response);
}
