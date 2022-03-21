# Example 1
This example show how to make a call to the API that do not need authentication.

# Example 2
This example show how to make a call to the API that need authentication and save the result into a variable.

# Example 3
More complex example that create, read and destroy a resource. (also using a different methode to read curl return data).

# Build and Run

- install ruby-ffi
- set CURL_PATH to the good environement variable unless /usr/lib/libcurl.so existe
- be sure ak, sk are set as OSC_ACCESS_KEY and OSC_SECRET_KEY
- `ruby example02.rb` or  `CURL_PATH=/usr/lib64 ruby example02.rb`
