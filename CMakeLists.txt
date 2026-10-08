cmake_minimum_required(VERSION 3.20)
project(solar_sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5          # or the newest tag on raylib's releases page
  GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG 4.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raygui)

add_executable(solar_sim src/main.cpp src/sim/simulation.cpp)
target_link_libraries(solar_sim PRIVATE raylib)
target_include_directories(solar_sim PRIVATE src)
target_include_directories(solar_sim SYSTEM PRIVATE ${raygui_SOURCE_DIR}/src)