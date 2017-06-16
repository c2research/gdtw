project(distanceLib)

file(GLOB_RECURSE SRC_FILES RELATIVE ${PROJECT_SOURCE_DIR} *.cpp)
list(REMOVE_ITEM SRC_FILES "MatlabMex.cpp")

if (Matlab_FOUND)
    matlab_add_mex(
        NAME gdtw
        SRC ${SRC_FILES} MatlabMex.cpp
    )
endif()

add_library(distanceLib ${SRC_FILES})