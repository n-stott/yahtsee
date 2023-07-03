project(yahtsee)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_consistency test_consistency.cpp ../src/logic.cpp)
target_include_directories(test_consistency PUBLIC ../include)
add_test(NAME consistency COMMAND test_consistency)

add_executable(test_speed test_speed.cpp ../src/logic.cpp ../src/arena.cpp ../src/agents/random.cpp)
target_include_directories(test_speed PUBLIC ../include)
add_test(NAME speed COMMAND test_speed)