/* Example 03 :
This example show how to read a specified instance.
The answer from the request is displayed on the standard output.

    /!\ : This example only work with libcurl 7.75 or above
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

#define AK_SIZE 20
#define SK_SIZE 40

void main(){
  // Getting the access key / secret key from environement
  const char *ak = getenv("OSC_ACCESS_KEY");
  const char *sk = getenv("OSC_SECRET_KEY");

  char ak_sk[AK_SIZE + SK_SIZE + 2];

  if (strlen(ak) != AK_SIZE || strlen(sk) != SK_SIZE) {
    abort();
  }

  stpcpy(stpcpy(stpcpy(ak_sk, ak), ":"), sk);

  // Data to post. See : https://docs.outscale.com/api, for more information
  const char *data = "{ \"Filters\": {\"VmIds\": [\"i-42s6t6n6\"] } }";
  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();

  // Setting the url
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadVms");

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
