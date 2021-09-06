# Easy api call with authentication and writing result data into file
#
#   Replace ACCESSKEY:SECRETKEY by yours
#
#   NOTICE : This example only works with libcurl 7.75 or above
#

require 'ffi'

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
                 :CURLOPT_VERBOSE,       option_long + 41,
                 :CURLOPT_POSTFIELDS,    option_objectpoint + 15,
                 :CURLOPT_POSTFIELDSIZE, option_long + 60,
                 :CURLOPT_AWS_SIGV4,     option_objectpoint + 305,
                 :CURLOPT_WRITEDATA,     option_cbpoint + 1,
                 :CURLOPT_WRITEFUNCTION, option_functionpoint + 11,
                ]

  #Attaching functions
  attach_function :curl_easy_init, [], :pointer
  attach_function :curl_easy_cleanup, [:pointer], :void
  attach_function :curl_easy_perform, [:pointer], :int
  attach_function :curl_slist_append, [:pointer, :string], :pointer

=begin
  To use the right easy_setopt function refer to this :
         easy_setopt_long       : option_long
         easy_setopt_string     : option_objectpoint
         easy_setopt_pointer    : option_functionpoint
         easy_setopt_curl_off_t : option_off_t

  Explanation : In the enum, each symbol is associated with a value in the shape of : option_* + value
                Use the above table to match the rigth option and method.
=end

  #Struct that holds the data returned by libcurl as a pointer and the size of the data
  class DataStruct < FFI::Struct
    layout :data, :pointer,
           :size, :size_t
  end

  attach_function :easy_setopt_long, :curl_easy_setopt, [:pointer, :option, :long], :int
  attach_function :easy_setopt_string, :curl_easy_setopt, [:pointer, :option, :string], :int
  attach_function :easy_setopt_pointer, :curl_easy_setopt, [:pointer, :option, :pointer], :int
  attach_function :easy_setopt_curl_off_t, :curl_easy_setopt, [:pointer, :option, :curl_off_t], :int

  # Function called by CURLOPT_WRITEFUNCTION. It copy the data of data pointer into userp and return a size
  Callback = FFI::Function.new(:size_t, [:pointer, :size_t, :size_t, DataStruct.by_ref]) do |data, size, nmemb, userp|
    realsize = size * nmemb

    # We can't write literal string inside struct so we write the data to a pointer
    p_data = FFI::MemoryPointer.new(:char, realsize)
    p_data.write_string(data.read_string(realsize))
    userp[:size] = realsize
    userp[:data] = p_data
    realsize
  end
end

# We initialise a varaible with an empty struct
output = Curl::DataStruct.new
aksk = ENV["OSC_ACCESS_KEY"] + ":" + ENV["OSC_SECRET_KEY"]

# We let FFI manage pointers for us
c = FFI::AutoPointer.new(Curl.curl_easy_init, Curl.method(:curl_easy_cleanup))

# Putting the URL and posible POST data
Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadVms")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, "")

# To authenticate
Curl.easy_setopt_string(c, :CURLOPT_AWS_SIGV4, "osc")
Curl.easy_setopt_string(c, :CURLOPT_USERPWD, aksk)

# The first function tells libcurl to use our callback method
# The second line we pass the varaible where the data should be written
Curl.easy_setopt_pointer(c, :CURLOPT_WRITEFUNCTION, Curl::Callback)
Curl.easy_setopt_pointer(c, :CURLOPT_WRITEDATA, output)

# You can store the return value into a variable : it returns an integer
code = Curl.curl_easy_perform(c)
#To use the output we read the string referenced by the pointer
output_str = output[:data].read_string(output[:size])

puts output_str
