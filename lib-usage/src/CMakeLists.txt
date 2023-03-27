INCLUDE_DIRECTORIES(/usr/include/hello)
add_executable(main main.cpp)
target_link_libraries(main libhello.so)