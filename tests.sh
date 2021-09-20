#!/bin/bash

set -e


abort()
{
    echo >&2 '
***************
*** ABORTED ***
***************
'
    echo "An error occurred. Exiting..." >&2
    exit 1
}

trap 'abort' 0

cd ruby
ruby example01.rb | grep Services > /dev/null
ruby example02.rb | grep Vms > /dev/null
ruby example03.rb | grep Net > /dev/null
cd ..

cd c
make clean
make
./example01 | grep Images > /dev/null
./example02 > out
echo [ $( sed s/}{/},{/g out ) ]  > out
json-search  "SecurityGroup" out > /dev/null # check create
json-search "SecurityGroups" out > /dev/null # check read

# test 3 request have been executed
[ $(cat out | json-search   "RequestId"  | wc -l) == "5" ]

./example03 | grep Images > /dev/null
cd ..

trap - 0
