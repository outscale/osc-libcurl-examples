import pycurl
import os
import sys
import json

AK_SIZE = 20
SK_SIZE = 40

class Tag:
    def __init__(self, key, value, ressource_id):
        self.key = key
        self.value = value
        self.resources_id = ressource_id

def get_account_id(ak_sk):
    # Setting the URL for read 
    url = "https://api.eu-west-2.outscale.com/api/v1/ReadAccounts"
    json_data = ""

    curl = setup_curl(url, json_data, ak_sk)
    response_data = send_request(curl)
    
    account_id = response_data['Accounts'][0]['AccountId']
    return account_id

def setup_curl(url, json_data, ak_sk):
    curl = pycurl.Curl()
    headers = ["Content-Type: application/json"]
    curl.setopt(pycurl.HTTPHEADER, headers)
    curl.setopt(pycurl.URL, url)
    curl.setopt(pycurl.POSTFIELDS, json_data)
    curl.setopt(pycurl.AWS_SIGV4, 'osc')
    curl.setopt(pycurl.USERPWD, ak_sk)
    return curl

def send_request(curl):
    response_data = ""

    def write_data(data):
        nonlocal response_data
        response_data += data.decode('utf-8')

    try:
        curl.setopt(pycurl.WRITEFUNCTION, write_data)   
        curl.perform()
    except pycurl.error as e:
        print("Pycurl error:", e)
        return None

    curl.close()

    try:
        return json.loads(response_data)
    except json.JSONDecodeError:
        print("JSON error: Failed to parse response data as JSON.")
        return None

def create_tag(tag, ak_sk):
    data = {"ResourceIds": [tag.resources_id],"Tags": [{"Key": tag.key, "Value": tag.value}]}
    json_data = json.dumps(data)

    # Setting the URL for create
    url = "https://api.eu-west-2.outscale.com/api/v1/CreateTags"

    curl = setup_curl(url, json_data, ak_sk)
    return send_request(curl)

def delete_tag(tag, ak_sk):
    data = {"ResourceIds": [tag.resources_id], "Tags": [{"Key": tag.key, "Value": tag.value}]}
    json_data = json.dumps(data)

    # Setting the URL for delete
    url = "https://api.eu-west-2.outscale.com/api/v1/DeleteTags"

    curl = setup_curl(url, json_data, ak_sk)
    return send_request(curl)

def read_security_group(tag, ak_sk, account_id):
    account = []
    account.append(account_id)
    data = {"Filters": {"AccountIds": account}}
    json_data = json.dumps(data)

    # Setting the URL for read
    url = "https://api.eu-west-2.outscale.com/api/v1/ReadSecurityGroups"

    response_data = send_request(setup_curl(url, json_data, ak_sk))

    if response_data:
        return response_data
    else:
        return None

def get_security_group(response_data):
    security_groups = response_data.get('SecurityGroups', [])

    for security_group in security_groups:
        if 'SecurityGroupId' in security_group and security_group['SecurityGroupId'].startswith("sg-"):
            return security_group['SecurityGroupId']
    return None

def main():
    # Getting the access key / secret key from the environment
    ak = os.getenv("OSC_ACCESS_KEY")
    sk = os.getenv("OSC_SECRET_KEY")
    ak_sk = f"{ak}:{sk}"
    account_id = get_account_id(ak_sk)

    if ak is None or sk is None is None:
        print("OSC_ACCESS_KEY or OSC_SECRET_KEY environment variable not set")
        return 1

    if len(ak) != AK_SIZE or len(sk) != SK_SIZE:
        print("Wrong size OSC_ACCESS_KEY or OSC_SECRET_KEY")
        return 1

    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <Key> <Value>")
        return 1

    security_group_id = ""
    new_tag = Tag(sys.argv[1], sys.argv[2], security_group_id)
    response_data = read_security_group(new_tag, ak_sk, account_id)

    if response_data:
        security_group_id = get_security_group(response_data)
        new_tag.resources_id = security_group_id
        print(f"RessourceId: {new_tag.resources_id}, Key: {new_tag.key}, Value: {new_tag.value}")

        response_json = create_tag(new_tag, ak_sk)
        if not response_json:
            return 1
        print(f"\nResponse for create tag : {json.dumps(response_json, indent=2)}")

        response_json = delete_tag(new_tag, ak_sk)
        if not response_json:
            return 1
        print(f"\nResponse for delete tag : {json.dumps(response_json, indent=2)}")

    return 0


if __name__ == "__main__":
    sys.exit(main())