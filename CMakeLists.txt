cmake_minimum_required(VERSION 3.0)
project(LambdaEngine)

# Definitions for debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_BUILD)
endif()

# Find SDL2, SDL2_image, and SDL2_ttf
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Include directories for SDL dependencies
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# Set directories and base directory
set(basedir lambda_engine)

# Set directories and base directory
set(dirs)
file(GLOB subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${basedir} ${basedir}/*)
foreach(dir ${subdirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${basedir}/${dir})
        list(APPEND dirs ${dir})
    endif()
endforeach()


# Collect all source files
set(SOURCES src/main.cpp)
foreach(dir IN ITEMS ${dirs})
    # Get all .cpp files in the directory
    file(GLOB CURR_SOURCES "${basedir}/${dir}/*.cpp")
    list(APPEND SOURCES ${CURR_SOURCES})
endforeach()

message ("${SOURCES}")
# Add the executable
add_executable(main ${SOURCES})

# Include directories for header files
foreach(dir IN ITEMS dirs)
    target_include_directories(main PUBLIC "${basedir}/${dir}")
endforeach()

# Add the lambda_engine directory to include directories
include_directories(${basedir})

# Link SDL dependencies
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
