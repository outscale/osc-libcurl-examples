extern crate libc;

use std::env;
use std::ffi::{CStr, CString};
use std::cell::RefCell;
use std::rc::Rc;

use rust::*;

// Used to write the data we are getting from libcurl 
fn callback(data: *const libc::c_char,
		       size: libc::size_t,
		       nmemb: libc::size_t,
		       userp: Rc<RefCell<&str>>) -> libc::size_t
{
    unsafe {
	let data_str = CStr::from_ptr(data);
	let data_slice = data_str.to_str().unwrap();

	*userp.borrow_mut() = data_slice;
	
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

	// Will hold the data from our request
	let response = Rc::new(RefCell::new(""));

	// Initialize the curl handler
	let c = curl_easy_init();

	// See what curl is doing
	curl_easy_setopt(c, CURLOPT_VERBOSE, 1);

	// Setting url and post field
	curl_easy_setopt(c, CURLOPT_URL, url.to_str().unwrap());
	curl_easy_setopt(c, CURLOPT_POSTFIELDS, data.to_str().unwrap());
	
	// Authentification
	curl_easy_setopt(c, CURLOPT_AWS_SIGV4, provider.to_str().unwrap());
	curl_easy_setopt(c, CURLOPT_USERPWD, ak_sk.to_str().unwrap());

	// First tells what fuction to use, second tells wich variable to use 
	curl_easy_setopt(c, CURLOPT_WRITEFUNCTION,
			 callback as fn(*const i8, usize, usize, Rc<RefCell<&str>>)-> usize);
	
	curl_easy_setopt(c, CURLOPT_WRITEDATA, Rc::clone(&response));   
	
	// Perform
	curl_easy_perform(c);

	curl_easy_cleanup(c);


	//Getting the wraped value by shadowing the variable response
	let response = &(*response.take());
	
	println!("Output : {} \n", response);
    }
    
    
}
