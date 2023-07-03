set(CMAKE_CXX_STANDARD 20)

add_executable(yahtsee
    src/logic.cpp
    src/arena.cpp
    src/agents/random.cpp
    src/main.cpp
)
# target_compile_options(yahtsee PUBLIC -g)
target_include_directories(yahtsee PUBLIC include)