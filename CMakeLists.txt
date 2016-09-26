cmake_minimum_required(VERSION 2.6)
project(terris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")

file(COPY img DESTINATION .)

file(GLOB GAME_SOURCES "*.cpp" "*.h")

add_executable(terris ${GAME_SOURCES})

target_link_libraries(terris -lSDL2 -lSDL2_image -lSDL2_ttf)
