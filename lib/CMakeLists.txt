project(gdtw)

file(GLOB_RECURSE SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)

matlab_add_mex(
    NAME gdtw
    SRC ${SRC_FILES}
)