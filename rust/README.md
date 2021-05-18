In this directory there is 3 examples written in Rust showing how to use libcurl with [outscale's API](https://docs.outscale.com/api).
You can find a minimal implementation of libcurl using FII inside the /src/lib.rs file.

> **Note** : These examples only work with libcurl 7.75 or above

## Example 1
This example show how to make a simple call to the API.

## Example 2
This example show how to make a call to the API and retreive the data inside a variable.
> **Note** : Libcurl show a failure message saying that it failed to write the output. Ignore it.

## Example 3
This example show how to make a call to the API with POST data.

# Usage
To run the examples use :
```bash
cargo run --example NAME
```
NAME is the name of the example you want to run

# License

> Copyright Outscale SAS
>
> BSD-3-Clause

This project is compliant with [REUSE](https://reuse.software/).
