# Cmake params
cmake_minimum_required(VERSION 3.9)
# Export data for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CPP params
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
	# string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -fno-omit-frame-pointer -fsanitize=address")
	string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO " -ftime-trace")
	# string(APPEND CMAKE_LINKER_FLAGS_RELWITHDEBINFO " -fno-omit-frame-pointer -fsanitize=address")
endif()

# Set cmake standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Define project
project(app CXX)
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp) 
include_directories(includes)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} algs) 

# Add libs
add_subdirectory(../algs libs/algs)

# Use threads
find_package(Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# Use openmp if found
option(USE_OPENMP "flag to use OpenMP for multithreading" ON)
if(USE_OPENMP)
    find_package(OpenMP 3.0 COMPONENTS CXX)
    if (OpenMP_CXX_FOUND)
        message(STATUS "Building with OpenMP Multithreading.")
    else()
        message(STATUS "No OpenMP found, no multithreading.")
    endif()
endif()

# Copy to source directory
add_custom_target(
    copy-compile-commands ALL
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/../compile_commands.json
    )
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/../compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}/../compile_commands.json
    DEPENDS
        # Unlike "proper" targets like executables and libraries, 
        # custom command / target pairs will not set up source
        # file dependencies, so we need to list file explicitly here
        generate-compile-commands
        ${CMAKE_BINARY_DIR}/compile_commands.json
    )

# Generate the compilation commands. Necessary so cmake knows where it came
# from and if for some reason you delete it.
add_custom_target(generate-compile-commands
    DEPENDS
        ${CMAKE_BINARY_DIR}/compile_commands.json
    )
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -B${CMAKE_BINARY_DIR} -S${CMAKE_SOURCE_DIR}
    )
