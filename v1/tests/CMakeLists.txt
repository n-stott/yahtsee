project(yahtsee)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_consistency test_consistency.cpp)
target_include_directories(test_consistency PUBLIC ../include)
target_link_libraries(test_consistency PUBLIC libyahtsee)
add_test(NAME consistency COMMAND test_consistency)

add_executable(test_speed test_speed.cpp)
target_include_directories(test_speed PUBLIC ../include)
target_link_libraries(test_speed PUBLIC libyahtsee)
add_test(NAME speed COMMAND test_speed)