project(onlineGdtw)

add_executable(online Main.cpp GDTWOnline.cpp)

target_link_libraries(online ${Readline_LIBRARY} distanceLib)

set_target_properties(online PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})