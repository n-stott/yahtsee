set(CMAKE_CXX_STANDARD 20)

add_library(gamelogic
    src/logic/evaluator.cpp
    src/logic/simulator.cpp
    src/logic/gamegraph.cpp
    src/arena/arena.cpp
    src/agents/random.cpp
    src/agents/greedy.cpp
    src/agents/smartgreedy.cpp
    src/agents/smartergreedy.cpp
    src/agents/pgreedy.cpp
    src/agents/bonuspriorityzing.cpp
    src/agents/learner.cpp
)
target_include_directories(gamelogic PUBLIC include)

add_executable(yahtsee
    src/main.cpp
)
target_include_directories(yahtsee PUBLIC include)
target_link_libraries(yahtsee PUBLIC gamelogic)