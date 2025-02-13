cmake_minimum_required(VERSION 3.7...3.26)

# Allow IDE Source Tree Visualization
if(${CMAKE_VERSION} VERSION_LESS 3.26)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
    cmake_policy(SET CMP0143 NEW)
endif()

project(Minesweeper)

set(SRCS
    "src/main.cpp"
)


if("${MINESWEEPER_EXE_EXTENSION}" STREQUAL "")
    message("none")
    set(MINESWEEPER_EXE_NAME "minesweeper")
else()
    set(MINESWEEPER_EXE_NAME "minesweeper-${MINESWEEPER_EXE_EXTENSION}")
endif()

add_executable(${MINESWEEPER_EXE_NAME} ${SRCS})

target_compile_features(${MINESWEEPER_EXE_NAME} PRIVATE cxx_std_20)

# for Visual Studio/MSVC
set_target_properties(${MINESWEEPER_EXE_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
