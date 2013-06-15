cmake_minimum_required(VERSION 2.8)
project( grapevine )

# cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../code
# ./gv --gtest_filter=Context*

# http://www.cmake.org/Wiki/CMake_Useful_Variables#Compilers_and_Tools
set(CMAKE_CXX_FLAGS_DEBUG, ON)
set(CMAKE_CXX_COMPILER "/opt/local/bin/g++-mp-4.8")
#set(CMAKE_CXX_COMPILER "/usr/bin/llvm-g++-4.2")
# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
set(CMAKE_CXX_FLAGS "-std=c++11 -m64")
# cmake -DCMAKE_OSX_ARCHITECTURES=x86_64

include_directories(/usr/local/include)
include_directories(.)
include_directories(./src)
include_directories(./src/util)
include_directories(./test)
link_directories(/usr/local/lib)

set(grapevine_srcs 
    main.cpp 
    
    # bloomier filter code
    src/contextSummary.cpp
    src/groupContextSummary.cpp
    src/groupDefinition.cpp
    src/serializer.cpp
    src/contextHandler.cpp
    src/contextShim.cpp
    src/contextSummarySerializer.cpp
    src/datagramContextShim.cpp
    src/contextShimmedDatagramSocket.cpp
    
    src/util/util.cpp
    src/util/groupUtils.cpp
    src/util/serializerUtils.cpp
    
    # test code
    test/test_groupContextSummary.cc
    test/test_contextSummary.cc
    
    test/test_serializer.cc
    test/test_contextHandler.cc
    test/test_contextShim.cc
    test/test_contextSummarySerializer.cc
    test/test_datagramContextShim.cc
    test/test_contextShimmedDatagramSocket.cc
    
    test/test_util.cc
    test/test_groupUtils.cc
    test/test_error.cc
    test/test_serializerUtils.cc
    )
    
add_executable(gv ${grapevine_srcs})
target_link_libraries(gv gtest.a)