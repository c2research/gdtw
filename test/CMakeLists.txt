# keep tests in a seperate folder
file(GLOB_RECURSE TEST_SRCS RELATIVE ${PROJECT_SOURCE_DIR}/test *.cpp)

# for each test file
foreach(testSrc ${TEST_SRCS})

  # extract file names (w/o extension)
  get_filename_component(testName ${testSrc} NAME_WE)

  # set compile target
  add_executable(${testName} ${testSrc})

  if (UNIX)
    target_compile_definitions(${testName} PRIVATE "BOOST_TEST_DYN_LINK=1")
  endif()

  #link to Boost libraries AND targets and dependencies
  target_link_libraries(${testName} onlineLib distanceLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

  # move tests to testOut
  set_target_properties(${testName} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/testOut)

  # add this to test execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/testOut/${testName})

endforeach(testSrc)
