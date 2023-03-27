#CMakeLists.txt

# 指定工程的名字和支持语言
# PROJECT (HELLO)
PROJECT (HELLO CXX)

# SET 变量用来生命变量 使用${variable_name} 读取变量
SET(SRC_LIST hello.cpp)

# 隐式生命两个变量 NAME_BINARY_DIR 和 NAME_SOURCE_DIR
# 建议直接使用PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR
MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${HELLO_SOURCE_DIR})

MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${PROJECT_SOURCE_DIR})

# 添加编译目标，如果源文件有多个，可以空格或分号隔开
ADD_EXECUTABLE(hello.out ${SRC_LIST})

# 内部构建和外部构建
# 内部构建在同级生成很多临时文件，不方便清理；推荐外部构建到build文件夹

# 两种方式
# 1. cmake -B ./build
# 2. cd build; cmake ..