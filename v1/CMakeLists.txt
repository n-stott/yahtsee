project(yahtsee)
cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)

add_library(libyahtsee
    src/logic.cpp
    src/arena.cpp
    src/agents/random.cpp
)
# target_compile_options(yahtsee PUBLIC -g)
target_compile_options(libyahtsee PUBLIC -Werror -Wall -Wextra -Wpedantic)
target_include_directories(libyahtsee PUBLIC include)

add_executable(yahtsee
    src/main.cpp
)
target_compile_options(yahtsee PUBLIC -Werror -Wall -Wextra -Wpedantic)
target_include_directories(yahtsee PUBLIC include)
target_link_libraries(yahtsee PUBLIC libyahtsee)

enable_testing()
add_subdirectory(tests)