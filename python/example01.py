import pycurl
import os
import sys

AK_SIZE = 20
SK_SIZE = 40

class Tag:
    def __init__(self, resources_id, key, value):
        self.resources_id = resources_id
        self.key = key
        self.value = value

def setup_curl(url, json_data, ak_sk):
    curl = pycurl.Curl()
    headers = ["Content-Type: application/json"]
    curl.setopt(pycurl.HTTPHEADER, headers)
    curl.setopt(pycurl.URL, url)
    curl.setopt(pycurl.POSTFIELDS, json_data)
    curl.setopt(pycurl.AWS_SIGV4, 'osc')
    curl.setopt(pycurl.USERPWD, ak_sk)
    
    try:
        curl.perform()
    except pycurl.error as e:
        print("Pycurl error:", e)
        return False
    
    curl.close()
    return True

def create_tag(tag, ak_sk):
    json_data = f'{{"ResourceIds": ["{tag.resources_id}"], "Tags": [{{"Key": "{tag.key}", "Value": "{tag.value}"}}]}}'
    # Setting the URL for create #
    url = "https://api.eu-west-2.outscale.com/api/v1/CreateTags"
    
    if setup_curl(url, json_data, ak_sk):
        print(f"\nTag created successfully: \nResources ID: {tag.resources_id}\n Key: {tag.key}\nValue: {tag.value})")
        return True
    else:
        return False

def delete_tag(tag, ak_sk):
    json_data = f'{{"ResourceIds": ["{tag.resources_id}"], "Tags": [{{"Key": "{tag.key}", "Value": "{tag.value}"}}]}}'
    # Setting the URL for delete #
    url = "https://api.eu-west-2.outscale.com/api/v1/DeleteTags"
    
    if setup_curl(url, json_data, ak_sk):
        print(f"\nTag deleted successfully: \nResources ID: {tag.resources_id}\n Key: {tag.key}\nValue: {tag.value})")
        return True
    else:
        return False

def main():
    # Getting the access key / secret key from the environment #
    ak = os.getenv("OSC_ACCESS_KEY")
    sk = os.getenv("OSC_SECRET_KEY")
    
    if ak is None or sk is None:
        print("OSC_ACCESS_KEY or OSC_SECRET_KEY environment variable not set")
        return 1

    if len(ak) != AK_SIZE or len(sk) != SK_SIZE:
        print("Wrong size OSC_ACCESS_KEY or OSC_SECRET_KEY")
        return 1

    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <ResourceId> <Key> <Value>")
        return 1

    ak_sk = f"{ak}:{sk}"
    new_tag = Tag(sys.argv[1], sys.argv[2], sys.argv[3])

    if not create_tag(new_tag, ak_sk):
        print("Failed to create tag.\n")
        return 1

    if not delete_tag(new_tag, ak_sk):
        print("Failed to delete tag.\n")
        return 1

    return 0

if __name__ == "__main__":
    sys.exit(main())
