grapevineCpp
============

## How to build
cd build
cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../code

## Selectively executing unit test
    ./gv --gtest_filter=UtilTest.*

Or make and run only the Context* tests

    make && ./gv --gtest_filter=Context*

refer to [stackoverflow]

## Throw errors
C++ doesn't have a good way to point out where the error occurs. Throw Error() object as follows to get the error.

    throw Error(__FILE__ + std::to_string(__LINE__));

## Think
1. We don't copy the map into the ContextSummary now, but it's dangerous not to get all the information in one Summary. 

[stackoverflow]: http://stackoverflow.com/questions/17093772/selectively-executing-unit-tests-with-googletest/17093852#17093852
