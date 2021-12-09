/* Example 02 :
This example show how to create a security rule.
The answer from the request is displayed on the standard output.

   /!\ : This example only works with libcurl 7.75 or above
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

#define AK_SIZE 20
#define SK_SIZE 40

int main(void){
  // Getting the access key / secret key from environement
  const char *ak = getenv("OSC_ACCESS_KEY");
  const char *sk = getenv("OSC_SECRET_KEY");

  if (ak == NULL || sk == NULL) {
    printf("ak or sk not declared");
    exit(1);
  }
  char ak_sk[AK_SIZE + SK_SIZE + 2];

  if (strlen(ak) != AK_SIZE || strlen(sk) != SK_SIZE) {
    printf("wrong size of ak or sk");
    exit(1);
  }

  stpcpy(stpcpy(stpcpy(ak_sk, ak), ":"), sk);

  // Data to post. See : https://docs.outscale.com/api, for more information
  const char *data = "{ \"Description\": \"This is a security rule created by a c program \","
    "\"SecurityGroupName\":  \"sg_c_libcurl\" }";
  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();
  const char *op;

  // Creating HEADERS
  struct curl_slist *hs = NULL;
  hs = curl_slist_append(hs, "Content-Type: application/json");

  // Setting HEADERS
  curl_easy_setopt(c, CURLOPT_HTTPHEADER, hs);

 // Setting the url
  op = "https://api.eu-west-2.outscale.com/api/v1/CreateSecurityGroup";
  curl_easy_setopt(c, CURLOPT_URL, op);

  // Empty post field to indicate we want to send a post request
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, data);
  //curl_easy_setopt(c, CURLOPT_VERBOSE, 1);

  // For authentification we specify the method and our acces key / secret key
  curl_easy_setopt(c, CURLOPT_AWS_SIGV4, "osc");
  curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk);

  res = curl_easy_perform(c);

  op = "https://api.eu-west-2.outscale.com/api/v1/ReadSecurityGroups";
  curl_easy_setopt(c, CURLOPT_URL, op);
  curl_easy_setopt(c, CURLOPT_POSTFIELDS,
		   "{\"Filters\": { \"SecurityGroupNames\": [\"sg_c_libcurl\"]}}");
  res = curl_easy_perform(c);

  op = "https://api.eu-west-2.outscale.com/api/v1/DeleteSecurityGroup";
  curl_easy_setopt(c, CURLOPT_URL, op);
  curl_easy_setopt(c, CURLOPT_POSTFIELDS,
		   "{\"SecurityGroupName\": \"sg_c_libcurl\" }");
  res = curl_easy_perform(c);

  curl_slist_free_all(hs);

  curl_easy_cleanup(c);
}
