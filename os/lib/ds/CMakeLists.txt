cmake_minimum_required(VERSION 2.6)
enable_testing()
find_package(GTest MODULE REQUIRED)
add_executable(
    runUnitTests 
    static_linked_list_test.cpp 
    static_linked_list.c
    static_heap.c 
)
target_link_libraries(runUnitTests PRIVATE GTest::GTest GTest::Main)
add_test(runUnitTests runUnitTests)
    
