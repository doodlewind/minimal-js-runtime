cmake_minimum_required(VERSION 3.10)
project(main)
add_executable(main
        src/main.c
        src/cutils.c)

# libuv
include_directories(/usr/local/include)
add_library(libuv STATIC IMPORTED)
set_target_properties(libuv
        PROPERTIES IMPORTED_LOCATION
        "/usr/local/lib/libuv.a")

# quickjs
add_library(quickjs STATIC IMPORTED)
set_target_properties(quickjs
        PROPERTIES IMPORTED_LOCATION
        "/usr/local/lib/quickjs/libquickjs.a")

target_link_libraries(main
        libuv
        quickjs)
