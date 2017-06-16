project(onlineGdtw)

add_executable(online Main.cpp GDTWOnline.cpp)
add_library(onlineLib GDTWOnline.cpp)

target_link_libraries(online distanceLib ${Readline_LIBRARY})

set_target_properties(online PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
