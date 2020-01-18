cmake_minimum_required(VERSION 3.10)
project(main)
add_executable(main src/main.c)

# quickjs
include_directories(/usr/local/include)
add_library(quickjs STATIC IMPORTED)
set_target_properties(quickjs
        PROPERTIES IMPORTED_LOCATION
        "/usr/local/lib/quickjs/libquickjs.a")

target_link_libraries(main
        quickjs)
