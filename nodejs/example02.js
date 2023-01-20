const ffi = require('ffi-napi');
const ref = require('ref-napi');
const curl = require("./libcurl");

const ak = process.env.OSC_ACCESS_KEY
const sk = process.env.OSC_SECRET_KEY

const ak_sk = ak + ":" + sk

var c = curl.functions.curl_easy_init();

var hs = null;

hs = curl.functions.curl_slist_append(hs, "Content-Type: application/json");

var ustring = ""

const wr_func = ffi.Callback('int', ['string', 'int', 'int', 'pointer'],
			     function (data, size, nmemb, user) {
				 ustring += data;
				 return nmemb;
			   })


curl.functions.curl_easy_setopt('pointer')(c, curl.opt.HTTPHEADER, hs);

curl.functions.curl_easy_setopt('string')(c, curl.opt.AWS_SIGV4, 'osc');

curl.functions.curl_easy_setopt('string')(c, curl.opt.USERPWD, ak_sk);

curl.functions.curl_easy_setopt('string')(c, curl.opt.URL,
		 "https://api.eu-west-2.outscale.com/api/v1/ReadVms");
curl.functions.curl_easy_setopt('string')(c, curl.opt.POSTFIELDS, "");
curl.functions.curl_easy_setopt('pointer')(c, curl.opt.WRITEFUNCTION, wr_func);

var res = curl.functions.curl_easy_perform(c);

console.log(ustring, "\n");

curl.functions.curl_easy_cleanup(c);
