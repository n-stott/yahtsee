project(yahtsee)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)

add_executable(yahtsee
    src/logic.cpp
    src/arena.cpp
    src/agents/random.cpp
    src/main.cpp
)
# target_compile_options(yahtsee PUBLIC -g)
target_include_directories(yahtsee PUBLIC include)

enable_testing()
add_subdirectory(tests)