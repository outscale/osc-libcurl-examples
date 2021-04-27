require 'ffi'

module Curl
  extend FFI::Library
  ffi_lib '/usr/local/lib/libcurl.so'

  typedef :long_long, :curl_off_t

  option_long          = 0
  option_objectpoint   = 10000
  option_functionpoint = 20000 
  option_off_t         = 3000
  
  enum :option, [:CURLOPT_URL,           option_objectpoint + 2,
                 :CURLOPT_USERPWD,       option_objectpoint + 5,
                 :CURLOPT_VERBOSE,       option_long + 41,
                 :CURLOPT_POSTFIELDS,    option_objectpoint + 15,
                 :CURLOPT_POSTFIELDSIZE, option_long + 60,
                 :CURLOPT_AWS_SIGV4,     option_objectpoint + 305
                ]

  attach_function :curl_easy_init, [], :pointer
  attach_function :curl_easy_cleanup, [:pointer], :void
  attach_function :curl_easy_perform, [:pointer], :int
  attach_function :curl_slist_append, [:pointer, :string], :pointer
  
  attach_function :easy_setopt_long, :curl_easy_setopt, [:pointer, :option, :long], :int
  attach_function :easy_setopt_string, :curl_easy_setopt, [:pointer, :option, :string], :int
  attach_function :easy_setopt_pointer, :curl_easy_setopt, [:pointer, :option, :pointer], :int
  attach_function :easy_setopt_curl_off_t, :curl_easy_setopt, [:pointer, :option, :curl_off_t], :int
  
end


data = '{ "IpRange": "10.0.0.1/16" }'

c = FFI::AutoPointer.new(Curl.curl_easy_init, Curl.method(:curl_easy_cleanup))
code = Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/CreateNet")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, data)
Curl.easy_setopt_long(c, :CURLOPT_VERBOSE, 1)
Curl.easy_setopt_string(c, :CURLOPT_AWS_SIGV4, "osc")
Curl.easy_setopt_string(c, :CURLOPT_USERPWD, "ACCESKEY:SECRETKEY")
Curl.curl_easy_perform(c)
