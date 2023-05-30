project(yahtsee)
set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.18)

add_library(gamelogic
    src/logic/evaluator.cpp
)
target_include_directories(gamelogic PUBLIC include)

add_executable(yahtsee
    src/main.cpp
)
target_include_directories(yahtsee PUBLIC include)
target_link_libraries(yahtsee PUBLIC gamelogic)