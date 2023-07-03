set(CMAKE_CXX_STANDARD 20)

add_executable(yahtsee
    src/logic.cpp
    src/main.cpp
)
target_include_directories(yahtsee PUBLIC include)