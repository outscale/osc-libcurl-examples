/* Example 03 :
This example show how to make a call to the API whit data to POST.

    /!\ : This example only work with libcurl 7.75 or above
 */

#include "curl/curl.h"

int main(void)
{
  // Data to post. See : https://docs.outscale.com/api, for more information
  CURLcode res;

  // Creating the handler
  CURL *c = curl_easy_init();

  // Creating HEADERS
  struct curl_slist *hs = NULL;
  hs = curl_slist_append(hs, "Content-Type: application/json");

  // Setting HEADERS
  curl_easy_setopt(c, CURLOPT_HTTPHEADER, hs);

  // Setting the url
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadNetAccessPointServices");
  // Empty post field to indicate we want to send a post request
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, "");

  res = curl_easy_perform(c);

  curl_slist_free_all(hs);

  curl_easy_cleanup(c);
}
