# 相当于是让库作者自己管理自己编写的库
add_library(MathFunctions mysqrt.cxx)

# TODO 1: State that anybody linking to MathFunctions needs to include the
# current source directory, while MathFunctions itself doesn't.
# Hint: Use target_include_directories with the INTERFACE keyword
# This can be expressed with an INTERFACE usage requirement. Remember INTERFACE means things that consumers require but the producer doesn't.
target_include_directories(MathFunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Current src dir in this lib is " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Current bin dir in thie lib is " ${CMAKE_CURRENT_BINARY_DIR})
