extern crate libc;

pub use libc::c_int;

pub type CURLoption = libc::c_uint;

pub enum CURL {}

const CURLOPTTYPE_LONG: CURLoption = 0;
const CURLOPTTYPE_OBJECTPOINT: CURLoption = 10_000;
const CURLOPTTYPE_FUNCTIONPOINT: CURLoption = 20_000;
const _CURLOPTTYPE_OFF_T: CURLoption = 30_000;
const CURLOPTTYPE_CBPOINT: CURLoption = CURLOPTTYPE_OBJECTPOINT;

pub const CURLOPT_URL: CURLoption = CURLOPTTYPE_OBJECTPOINT + 2;
pub const CURLOPT_VERBOSE: CURLoption = CURLOPTTYPE_LONG + 41;
pub const CURLOPT_AWS_SIGV4: CURLoption = CURLOPTTYPE_OBJECTPOINT + 305;
pub const CURLOPT_USERPWD: CURLoption = CURLOPTTYPE_OBJECTPOINT + 5;
pub const CURLOPT_POSTFIELDS: CURLoption = CURLOPTTYPE_OBJECTPOINT + 15;
pub const CURLOPT_WRITEDATA: CURLoption = CURLOPTTYPE_CBPOINT + 1;
pub const CURLOPT_WRITEFUNCTION: CURLoption = CURLOPTTYPE_FUNCTIONPOINT + 11;
pub const CURLOPT_HTTPHEADER: CURLoption = CURLOPTTYPE_OBJECTPOINT + 23;

#[link(name = "curl")]
extern "C" {
    pub fn curl_easy_init() -> *mut CURL;
    pub fn curl_easy_cleanup(curl: *mut CURL);
    pub fn curl_easy_setopt(curl: *mut CURL, option: CURLoption, ...) -> libc::c_int;
    pub fn curl_easy_perform(curl: *mut CURL) -> libc::c_int;
    pub fn curl_slist_append(list: *mut libc::c_void, val: *const i8) -> *mut libc::c_void;
    pub fn curl_slist_free_all(list: *mut libc::c_void);
}
