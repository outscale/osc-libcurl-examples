# Note

**REQUIRE PHP 8.2 OR SUPERRIOR**

As this is a fearly recent versions of PHP, you might have to compile it.

it's like 
```
./buildconf ./configure --with-curl; make -jNUMBER_OF_CORE_FLEX; ./sapi/cli/php PATH/TO/PHP8.2/FILE
```

Also note that in comparaison to other language, this example use PHP curl modules(and not FFI),
which is generally package by default with PHP.

# Examples:

0. simple call to read images, with a small filter, data manipulate as string
1. similar example, with 2 diferents call, with object-like manipulation of data
