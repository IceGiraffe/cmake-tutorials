PROJECT(HELLO)

# ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
add_subdirectory(src bin)

# set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../)

# # /home/wang/cmake-tutorials/2
# message(STATUS ${CMAKE_INSTALL_PREFIX})
# # /home/wang/cmake-tutorials/2/build
# message(STATUS ${PROJECT_BINARY_DIR})
# # /home/wang/cmake-tutorials/2
# message(STATUS ${PROJECT_SOURCE_DIR})


# 安装文件
# 默认安装路径为/usr/local/*****
# 安装路径为 /usr/local/share/doc/cmake
install(FILES COPYRIGHT README DESTINATION share/doc/cmake)

# 非目标的可执行文件安装使用PROGRAMS
# /usr/local/bin
# 命令不区分大小写，参数区分大小写
install(PROGRAMS runhello.sh DESTINATION bin/)

INSTALL(DIRECTORY doc/ DESTINATION share/doc/cmake)

INSTALL(TARGETS hello DESTINATION bin/)