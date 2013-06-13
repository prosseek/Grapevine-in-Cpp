grapevineCpp
============

## How to build
cd build
cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../code

## Selectively executing unit test
    ./gv --gtest_filter=UtilTest.*
refer to [stackoverflow]

[stackoverflow]: http://stackoverflow.com/questions/17093772/selectively-executing-unit-tests-with-googletest/17093852#17093852
