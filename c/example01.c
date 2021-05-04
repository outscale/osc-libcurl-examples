/* Example 01 :
This example show how to put the result 
of a curl request inside a file
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

void main(){
  // Getting the access key / secret key from environement
  const char *ak_sk = getenv("AKSK");

  // Opening a file where the result will be stored
  FILE *file = fopen("file.txt", "wb");

  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();

  // Setting the url
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadImages");

  // Let's see what curl is doing
  curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);

  // Empty post field to indicate we want to send a post request
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, "");

  // Telling curl to write the data fetched in the file
  curl_easy_setopt(c, CURLOPT_WRITEDATA, file);

  // For authentification we specify the method and our acces key / secret key
  curl_easy_setopt(c, CURLOPT_AWS_SIGV4, "osc");
  curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk);

  res = curl_easy_perform(c);

  curl_easy_cleanup(c);
}
