cmake_minimum_required(VERSION 3.16)
project(BASIC-Interpreter2024)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        Basic/Basic.cpp
        Basic/evalstate.cpp
        Basic/exp.cpp
        Basic/parser.cpp
        Basic/program.cpp
        Basic/statement.cpp
        Basic/Utils/error.cpp Basic/Utils/error.hpp Basic/Utils/tokenScanner.cpp Basic/Utils/tokenScanner.hpp
        Basic/Utils/strlib.cpp
        )

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif()
