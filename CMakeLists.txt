# Cmake params
cmake_minimum_required(VERSION 3.9)
# Export data for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CPP params
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Wall -Wextra -pedantic -Werror")

# Set cmake standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Speed up
set(UNITY_BUILD true)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	# set(CMAKE_BUILD_TYPE "RelWithDebInfo")

	# string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
	# string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
	# add_link_options(-fsanitize=address)

	# string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=undefined")
	# string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=undefined")
	# add_link_options(-fsanitize=undefined)

	# string(APPEND CMAKE_CXX_FLAGS_DEBUG " -ftime-trace")

	# Include what you use
	# find_program(IWYU_PATH NAMES include-what-you-use iwyu)
	# if(NOT IWYU_PATH)
	# 	message(FATAL_ERROR "Could not find the program include-what-you-use")
	# endif()
	# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
endif()

# Define project
project(goto_elimination LANGUAGES CXX VERSION 1.0.0)

include_directories(lib/ast lib/goto_elimination)

# Add goto_elimination
add_subdirectory(lib)

add_executable(app main.cc)
target_link_libraries(app goto_elimination) 

# Add test
add_subdirectory(tests)

###########################
#  compile_commands.json  #
###########################

add_custom_target(
    copy-compile-commands
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}
    )
