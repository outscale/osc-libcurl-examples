# Easy API request without authentication
#
#
#    NOTICE : This example only works with libcurl 7.75 or above
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

  enum :option, [:CURLOPT_URL,           option_objectpoint + 2,
                 :CURLOPT_USERPWD,       option_objectpoint + 5,
                 :CURLOPT_VERBOSE,       option_long + 41,
                 :CURLOPT_POSTFIELDS,    option_objectpoint + 15,
                 :CURLOPT_POSTFIELDSIZE, option_long + 60,
                 :CURLOPT_AWS_SIGV4,     option_objectpoint + 305
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

  attach_function :easy_setopt_long, :curl_easy_setopt, [:pointer, :option, :long], :int
  attach_function :easy_setopt_string, :curl_easy_setopt, [:pointer, :option, :string], :int
  attach_function :easy_setopt_pointer, :curl_easy_setopt, [:pointer, :option, :pointer], :int
  attach_function :easy_setopt_curl_off_t, :curl_easy_setopt, [:pointer, :option, :curl_off_t], :int
end

# We let FFI manage pointers for us
c = FFI::AutoPointer.new(Curl.curl_easy_init, Curl.method(:curl_easy_cleanup))

# Putting the URL and posible POST data
Curl.easy_setopt_string(c, :CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/ReadNetAccessPointServices")
Curl.easy_setopt_string(c, :CURLOPT_POSTFIELDS, "")

# Let's see what curl is doing
Curl.easy_setopt_long(c, :CURLOPT_VERBOSE, 1)


Curl.curl_easy_perform(c)
