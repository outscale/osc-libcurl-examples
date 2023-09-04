/* Example 04 :
This example shows how to create and delete a tag for a specified resource.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#define AK_SIZE 20
#define SK_SIZE 40

#define auto_tag  __attribute__ ((__cleanup__(delete_tag_cleanup)))
#define auto_headers __attribute__((__cleanup__(headers_cleanup)))
#define auto_curl __attribute__((__cleanup__(curl_cleanup)))

#define TRY(condition, value, ...) if (condition) { \
  fprintf(stderr, __VA_ARGS__); \
  return value; \
}

struct Tag {
    char *resource_id;
    char *key;
    char *value;
    CURL *curl;
};


/* Function to create a tag */
struct Tag *create_tag(struct Tag *tag)
{
    CURLcode res;
    char json_data[BUFSIZ];
    snprintf(json_data, sizeof(json_data), "{\"ResourceIds\": [\"%s\"], \"Tags\": [{\"Key\": \"%s\", \"Value\": \"%s\"}]}",
    tag->resource_id, tag->key, tag->value);

    curl_easy_setopt(tag->curl, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/CreateTags");
    /* Empty post field to indicate we want to send a POST request */
    curl_easy_setopt(tag->curl, CURLOPT_POSTFIELDS, json_data);

    /* Perform the POST request */
    res = curl_easy_perform(tag->curl);

    TRY(res != CURLE_OK, NULL, "Failed to create tag: %s\n", curl_easy_strerror(res));

    printf("\nTag created successfully:\nResource ID: %s\nKey: %s\nValue: %s\n",
    tag->resource_id, tag->key, tag->value);

    return tag;
}

/* Function to delete a tag */
int delete_tag(struct Tag *tag)
{
    CURLcode res;
    char json_data[BUFSIZ];
    snprintf(json_data, sizeof(json_data), "{\"ResourceIds\": [\"%s\"], \"Tags\": [{\"Key\": \"%s\", \"Value\": \"%s\"}]}",
    tag->resource_id, tag->key, tag->value);

    /* Setting the URL for delete */
    curl_easy_setopt(tag->curl, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/DeleteTags");

    /* Empty post field to indicate we want to send a POST request */
    curl_easy_setopt(tag->curl, CURLOPT_POSTFIELDS, json_data);

    /* Perform the POST request */
    res = curl_easy_perform(tag->curl);

    TRY(res != CURLE_OK, -1, "Failed to delete tag: %s\n", curl_easy_strerror(res));

    printf("\nTag deleted successfully:\nResource ID: %s\nKey: %s\nValue: %s\n",
    tag->resource_id, tag->key, tag->value);

    return 0;
}

void delete_tag_cleanup(struct Tag **newTag)
{
    if (*newTag)
        delete_tag(*newTag);
}

void headers_cleanup(struct curl_slist **headers)
{
    if (*headers)
        curl_slist_free_all(*headers);
}

void curl_cleanup(CURL **curl_ptr)
{
    if (*curl_ptr)
        curl_easy_cleanup(*curl_ptr);
}

int main(int argc, char *argv[])
{
    /* Getting the access key / secret key from the environment */
    const char *ak = getenv("OSC_ACCESS_KEY");
    const char *sk = getenv("OSC_SECRET_KEY");

    TRY(ak == NULL || sk == NULL, 1, "OSC_ACCESS_KEY or OSC_SECRET_KEY environment variable not set\n");

    TRY(strlen(ak) != AK_SIZE || strlen(sk) != SK_SIZE, 1, "Wrong size OSC_ACCESS_KEY or OSC_SECRET_KEY\n");

    TRY(argc != 4, 1, "Usage: ./example04 <ResourceID> <Key> <Value>\n");

    /* Creating the handler */
    auto_curl CURL *curl = curl_easy_init();
    TRY(!curl, -1, "Failed to initialize libcurl.\n");

    /* Setting the AWS SV4 and AK/SK */
    curl_easy_setopt(curl, CURLOPT_AWS_SIGV4, "osc");
    curl_easy_setopt(curl, CURLOPT_USERNAME, ak);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, sk);

    /* Creating HEADERS */
    auto_headers struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    /* Setting HEADERS */
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    auto_tag struct Tag *newTag = create_tag(&(struct Tag){
        .resource_id = argv[1],
        .key = argv[2],
        .value = argv[3],
        .curl = curl
    });

}
