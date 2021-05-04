/* Example 02 :
This example show how to create a security rule.
The answer from the request is displayed on the standard output.

   /!\ : This example only works with libcurl 7.75 or above
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

void main(){
  // Getting the access key / secret key from environement
  const char *ak_sk = getenv("AKSK");

  // Data to post. See : https://docs.outscale.com/api, for more information
  const char *data = "{ \"Description\": \"This is a security rule created by a c program \","
    "\"SecurityGroupName\":  \"sg_c_libcurl\" }";
  
  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();

  // Setting the url
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/CreateSecurityGroup");

  // Let's see what curl is doing
  curl_easy_setopt(c, CURLOPT_VERBOSE, 1L);

  // Empty post field to indicate we want to send a post request
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, data);

  // For authentification we specify the method and our acces key / secret key
  curl_easy_setopt(c, CURLOPT_AWS_SIGV4, "osc");
  curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk);

  res = curl_easy_perform(c);

  curl_easy_cleanup(c);
}
