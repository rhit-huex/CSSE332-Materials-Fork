# top file of src/ directory

macro(add_test_executable prefix)
  add_executable(${prefix}.run ${ARGN})
  target_link_libraries(${prefix}.run pthread)
  target_include_directories(${prefix}.run
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src)
  add_test(NAME ${prefix} COMMAND $<TARGET_FILE:${prefix}.run>)
endmacro(add_test_executable)

# each problem will be compiled into a library that the tests use
if(BUILD_SOLUTIONS)
  add_test_executable(redhen redhen_solution.c)
  add_test_executable(priority priority_solution.c)
  add_test_executable(threejobs threejobs_solution.c)
else(BUILD_SOLUTIONS)
  add_test_executable(redhen redhen.c)
  add_test_executable(priority priority.c)
  add_test_executable(threejobs threejobs.c)
endif(BUILD_SOLUTIONS)

