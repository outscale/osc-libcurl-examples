extern crate libc;

use std::env;
use std::ffi::CString;
use std::ptr;

use rust::*;

fn main() {
    unsafe {
        // Getting AK and SK from variable environement
        let ak = env::var("OSC_ACCESS_KEY").unwrap();
        let sk = env::var("OSC_SECRET_KEY").unwrap();

        let ak_sk = CString::new(ak + ":" + &sk).unwrap();

        // Due to rust strong safety we need to wrap all of our strings into a CString
        let data = CString::new("").unwrap();
        let url = CString::new("https://api.eu-west-2.outscale.com/api/v1/ReadAccounts").unwrap();
        let provider = CString::new("osc").unwrap();

        let content_type = CString::new("Content-Type: application/json").unwrap();
        let list = curl_slist_append(ptr::null_mut(), content_type.as_ptr());

        // Initialize the curl handler
        let c = curl_easy_init();

        // See what curl is doing
        // curl_easy_setopt(c, CURLOPT_VERBOSE, 1);

        // Setting url
        curl_easy_setopt(c, CURLOPT_URL, url.to_str().unwrap());

        curl_easy_setopt(c, CURLOPT_POSTFIELDS, data.to_str().unwrap());
        // Authentification
        curl_easy_setopt(c, CURLOPT_AWS_SIGV4, provider.to_str().unwrap());
        curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk.to_str().unwrap());
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, list);

        // Perform
        curl_easy_perform(c);

        curl_easy_cleanup(c);
        curl_slist_free_all(list);
    }
}
