set(srcs cppassert.cpp)
add_executable(cppassertExample ${srcs})

target_link_libraries(cppassertExample PRIVATE cppassert)
