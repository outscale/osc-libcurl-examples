extern crate libc;

use std::env;
use std::ffi::{CStr, CString};
use std::ptr;

use rust::*;

fn callback(data: *const libc::c_char,
		       size: libc::size_t,
		       nmemb: libc::size_t,
		       userp: *mut libc::c_char) -> libc::size_t
{
    unsafe {

	size + nmemb
    }
}

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

	let mut response = CString::new("lu").unwrap();

	// Initialize the curl handler
	let c = curl_easy_init();

	// See what curl is doing
	curl_easy_setopt(c, CURLOPT_VERBOSE, 1);

	// Setting url
	curl_easy_setopt(c, CURLOPT_URL, url.to_str().unwrap());

	curl_easy_setopt(c, CURLOPT_POSTFIELDS, data.to_str().unwrap());
	// Authentification
	curl_easy_setopt(c, CURLOPT_AWS_SIGV4, provider.to_str().unwrap());
	curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk.to_str().unwrap());

	//
	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION,
			 callback as fn(*const i8, usize, usize, *mut i8)-> usize);
	
	curl_easy_setopt(c, CURLOPT_WRITEDATA, response.to_str().unwrap());   
	
	// Perform
	curl_easy_perform(c);

	curl_easy_cleanup(c);
    }
    
    
}
