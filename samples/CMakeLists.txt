set(srcs
    cppassert.cpp

)
set(CPPASSERT_EXAMPLE_NAME cppassertExample)
set(target_name ${CPPASSERT_EXAMPLE_NAME})

add_executable(${target_name} ${srcs})

target_link_libraries(${target_name} ${CPPASSERT_LIBNAME}  ${CPP_ASSERT_REQURED_LIBS})
