cmake_minimum_required(VERSION 4.3.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(csql-wks LANGUAGES CXX)
set(SRCS
    "src/types/Database.cpp"
    "src/csql.cpp"
    "src/internal/string.cpp"
)
add_library(csql STATIC ${SRCS})
set_property(TARGET csql PROPERTY CXX_STANDARD 23)

target_include_directories(csql PRIVATE
    "include/"
    "internal/"
)

target_link_libraries(csql
    sqlite3
)

set(SRCS
    "test/src/main.cpp"
)
add_executable(test ${SRCS})
set_property(TARGET test PROPERTY CXX_STANDARD 23)

target_include_directories(test PRIVATE
    "include"
)

target_link_libraries(test
    csql
)

