# Example 1
This example show how to make a call to the API that do not need authentication.

# Example 2
This example show how to make a call to the API that need authentication and save the result into a variable.

# Build and Run

- install ffi-napi (npm install ffi-napi)
- set CURL_PATH to the good environement variable unless /usr/lib/libcurl.so existe
- be sure ak, sk are set as OSC_ACCESS_KEY and OSC_SECRET_KEY
- `node example02.rb` or  `CURL_PATH=/usr/lib64 node example02.rb`
