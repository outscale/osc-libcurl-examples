/* Example 01 :
This example show how to put the result of a curl request inside a variable.
This example also shows how to read available images and all their informations.

    /!\ : This example only works with libcurl 7.75 or above
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

#define AK_SIZE 20
#define SK_SIZE 40

struct resp {
	int len;
	char *buf;
};

/* Function that will write the data inside a variable */
size_t write_data(void *data, size_t size, size_t nmemb, void *userp){
  size_t bufsize = size * nmemb;
  struct resp *response = userp;
  int olen = response->len;

  response->len = response->len + bufsize;
  response->buf = realloc(response->buf, response->len);
  memcpy(response->buf + olen, data, bufsize);
  return bufsize;
}

int main(void){
  /* Getting the access key / secret key from environement */
  const char *ak = getenv("OSC_ACCESS_KEY");
  const char *sk = getenv("OSC_SECRET_KEY");

  if (ak == NULL || sk == NULL) {
    fprintf(stderr, "OSC_ACCESS_KEY or OSC_SECRET_KEY environement variable not set\n");
    return(1);
  }
  char ak_sk[AK_SIZE + SK_SIZE + 2];

  if (strlen(ak) != AK_SIZE || strlen(sk) != SK_SIZE) {
    fprintf(stderr, "Wrong size OSC_ACCESS_KEY or OSC_SECRET_KEY\n");
    return(1);
  }
  stpcpy(stpcpy(stpcpy(ak_sk, ak), ":"), sk);

  /* Variable that will store the data */
  struct resp response = {0};
  CURLcode res;

  /* Creating the handler */
  CURL *c = curl_easy_init();

  /* Creating HEADERS */
  struct curl_slist *hs = NULL;
  hs = curl_slist_append(hs, "Content-Type: application/json");

  /* Setting HEADERS */
  curl_easy_setopt(c, CURLOPT_HTTPHEADER, hs);

  /* Setting the url */
  curl_easy_setopt(c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadImages");

  /* Let's see what curl is doing */

  /* Empty post field to indicate we want to send a post request */
  curl_easy_setopt(c, CURLOPT_POSTFIELDS, "");

  /* Telling curl to use the write_data function to write over response */
  curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(c, CURLOPT_WRITEDATA, &response);

  /* For authentification we specify the method and our acces key / secret key */
  curl_easy_setopt(c, CURLOPT_AWS_SIGV4, "osc");
  curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk);

  res = curl_easy_perform(c);

  curl_slist_free_all(hs);

  curl_easy_cleanup(c);

  printf("%s", response.buf);
  free(response.buf);
}
