# top level testing

macro(add_test_executable prefix)
  add_executable(${prefix}_tests.run ${prefix}_tests.c)
  target_link_libraries(${prefix}_tests.run ${prefix} cgrade)
  target_include_directories(${prefix}_tests.run
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src)
  add_test(NAME ${prefix} COMMAND $<TARGET_FILE:${prefix}_tests.run>)
endmacro(add_test_executable)

macro(add_debug_executable prefix)
  add_executable(${prefix}_debug.run ${prefix}_debug.c)
  target_link_libraries(${prefix}_debug.run ${prefix})
  target_include_directories(${prefix}_debug.run
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src)
endmacro(add_debug_executable)

# Problem 0: build tools
add_test_executable(hello)

# Problem 1: Fibonnaci bug
add_test_executable(fib)
add_debug_executable(fib)

# Problem 2: Vector
add_test_executable(vector)
add_debug_executable(vector)

# Problem 3: OO linked list
add_test_executable(db)
add_debug_executable(db)

# Extra sample code: ilist
add_test_executable(ilist)
add_debug_executable(ilist)

