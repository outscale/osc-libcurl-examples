# Easy API CALL with authentication and data in postfield
#
#    Replace ACCESSKEY:SECRETKEY by yours
#
#    NOTICE : This example only works with libcurl 7.75 or above
#

require 'ffi'
require 'json'

$str_out = ""

module Curl
  extend FFI::Library
  lib_path = '/usr/lib/libcurl.so'

  if ENV.include?("CURL_PATH") then
    lib_path = ENV["CURL_PATH"] + "/libcurl.so"
  end
  ffi_lib lib_path #Path to your curl library

  typedef :long_long, :curl_off_t

  #Curl use different ranges of values for the defines we do the same
  option_long          = 0
  option_objectpoint   = 10000
  option_functionpoint = 20000
  option_off_t         = 30000
  option_cbpoint       = option_objectpoint

  enum :option, [:CURLOPT_URL,           option_objectpoint + 2,
                 :CURLOPT_USERPWD,       option_objectpoint + 5,
                 :CURLOPT_HTTPHEADER,    option_objectpoint + 23,
                 :CURLOPT_VERBOSE,       option_long + 41,
                 :CURLOPT_POSTFIELDS,    option_objectpoint + 15,
                 :CURLOPT_POSTFIELDSIZE, option_long + 60,
                 :CURLOPT_AWS_SIGV4,     option_objectpoint + 305,
                 :CURLOPT_WRITEDATA,     option_cbpoint + 1,
                 :CURLOPT_WRITEFUNCTION, option_functionpoint + 11
                ]

  #Attaching functions
  attach_function :curl_easy_init, [], :pointer
  attach_function :curl_easy_cleanup, [:pointer], :void
  attach_function :curl_easy_perform, [:pointer], :int
  attach_function :curl_slist_append, [:pointer, :string], :pointer
  attach_function :curl_slist_free_all, [:pointer], :void

=begin
  To use the right easy_setopt function refer to this :
         easy_setopt_long       : option_long
         easy_setopt_string     : option_objectpoint
         easy_setopt_pointer    : option_functionpoint
         easy_setopt_curl_off_t : option_off_t

  Explanation : In the enum, each symbol is associated with a value in the shape of : option_* + value
                Use the above table to match the rigth option and method.

=end
  attach_function :easy_setopt_long, :curl_easy_setopt, [:pointer, :option, :long], :int
  attach_function :easy_setopt_string, :curl_easy_setopt, [:pointer, :option, :string], :int
  attach_function :easy_setopt_pointer, :curl_easy_setopt, [:pointer, :option, :pointer], :int
  attach_function :easy_setopt_curl_off_t, :curl_easy_setopt, [:pointer, :option, :curl_off_t], :int

  #Struct that holds the data returned by libcurl as a pointer and the size of the data
  class DataStruct < FFI::Struct


    layout :data, :pointer,
           :size, :size_t
  end

  # Function called by CURLOPT_WRITEFUNCTION. It copy the data of data pointer into userp and return a size
  Callback = FFI::Function.new(:size_t, [:pointer, :size_t, :size_t, DataStruct.by_ref]) do |data, size, nmemb, userp|
    realsize = size * nmemb

    $str_out = $str_out + data.read_string(realsize)
    realsize
  end

end

# Data for the post request as a string
data = '{ "IpRange": "10.0.0.1/16" }'

# We let FFI manage pointers
c = FFI::AutoPointer.new(Curl.curl_easy_init, Curl.method(:curl_easy_cleanup))
hs = FFI::AutoPointer.new(Curl.curl_slist_append(hs, "Content-Type: application/json"), Curl.method(:curl_slist_free_all))

# Putting the URL and posible POST data
Curl.easy_setopt_pointer(c, :CURLOPT_HTTPHEADER, hs)
Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/CreateNet")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, data)

# The first function tells libcurl to use our callback method
# The second line we pass the varaible where the data should be written
Curl.easy_setopt_pointer(c, :CURLOPT_WRITEFUNCTION, Curl::Callback)
Curl.easy_setopt_pointer(c, :CURLOPT_WRITEDATA, nil)

# To authenticate
Curl.easy_setopt_string(c, :CURLOPT_AWS_SIGV4, "osc")

aksk = ENV["OSC_ACCESS_KEY"] + ":" + ENV["OSC_SECRET_KEY"]
Curl.easy_setopt_string(c, :CURLOPT_USERPWD, aksk)


Curl.curl_easy_perform(c)
jsom_file = JSON.parse($str_out)
print("Create new net: ", jsom_file["Net"]["NetId"], "\n")
net_id = jsom_file["Net"]["NetId"]

Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadNets")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, "")
$str_out = ""
Curl.curl_easy_perform(c)
print("Read Nets :", $str_out, "\n")

Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/DeleteNet")
data = '{"NetId": "' + net_id + '"}'
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, data)
$str_out = ""
Curl.curl_easy_perform(c)
print("Delete ", net_id, " output:", $str_out, "\n")

Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadNets")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, "")
$str_out = ""
Curl.curl_easy_perform(c)
print($str_out, "\n")
