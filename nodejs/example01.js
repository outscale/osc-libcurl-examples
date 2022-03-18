const curl = require("./libcurl")

var c = curl.functions.curl_easy_init();

var hs = null;

hs = curl.functions.curl_slist_append(hs, "Content-Type: application/json");

curl.functions.curl_easy_setopt('pointer')(c, curl.opt.HTTPHEADER, hs);

curl.functions.curl_easy_setopt('string')(c, curl.opt.URL,
		 "https://api.eu-west-2.outscale.com/api/v1/ReadNetAccessPointServices");
curl.functions.curl_easy_setopt('string')(c, curl.opt.POSTFIELDS, "");

var res = curl.functions.curl_easy_perform(c);

curl.functions.curl_slist_free_all(hs);

curl.functions.curl_easy_cleanup(c);

console.log() // New Line
