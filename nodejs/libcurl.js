var ffi = require('ffi-napi');

var lib_path = '/usr/lib/libcurl.so'

if (process.env.CURL_PATH)
    lib_path = process.env.CURL_PATH + "/libcurl.so";

var functions = ffi.Library(lib_path, {
    'curl_easy_init':      ['pointer' , [] ],
    'curl_easy_cleanup':   ['void' , ['pointer'] ],
    'curl_slist_append':   ['pointer', ['pointer', 'string'] ],
    'curl_slist_free_all': ['void', ['pointer']],
    'curl_easy_setopt':    ['int', ['pointer', 'int' ], {
        'varargs': true
    } ],
    'curl_easy_perform':   ['int', ['pointer']]
});

const CURLOPTTYPE_LONG  = 0;
const CURLOPTTYPE_OBJECTPOINT = 10_000;
const CURLOPTTYPE_FUNCTIONPOINT  = 20_000;
const _CURLOPTTYPE_OFF_T  = 30_000;
const CURLOPTTYPE_CBPOINT = CURLOPTTYPE_OBJECTPOINT;

const CURLOPT_URL = CURLOPTTYPE_OBJECTPOINT + 2;
const CURLOPT_VERBOSE = CURLOPTTYPE_LONG + 41;
const CURLOPT_AWS_SIGV4 = CURLOPTTYPE_OBJECTPOINT + 305;
const CURLOPT_USERPWD = CURLOPTTYPE_OBJECTPOINT + 5;
const CURLOPT_POSTFIELDS = CURLOPTTYPE_OBJECTPOINT + 15;
const CURLOPT_WRITEDATA = CURLOPTTYPE_CBPOINT + 1;
const CURLOPT_WRITEFUNCTION = CURLOPTTYPE_FUNCTIONPOINT + 11;
const CURLOPT_HTTPHEADER = CURLOPTTYPE_OBJECTPOINT + 23;

var opt = {
    "TYPE_LONG": CURLOPTTYPE_LONG,
    "TYPE_OBJECTPOINT" : CURLOPTTYPE_OBJECTPOINT,
    "TYPE_FUNCTIONPOINT " : CURLOPTTYPE_FUNCTIONPOINT,
    "TYPE_CBPOINT" : CURLOPTTYPE_OBJECTPOINT,
    "URL" : CURLOPT_URL,
    "VERBOSE" : CURLOPT_VERBOSE,
    "AWS_SIGV4" : CURLOPT_AWS_SIGV4,
    "USERPWD" : CURLOPT_USERPWD,
    "POSTFIELDS" : CURLOPT_POSTFIELDS,
    "WRITEDATA" : CURLOPT_WRITEDATA,
    "WRITEFUNCTION" : CURLOPT_WRITEFUNCTION,
    "HTTPHEADER" : CURLOPT_HTTPHEADER
};

module.exports = { functions , opt};
