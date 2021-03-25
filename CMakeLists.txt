# Cmake params
cmake_minimum_required(VERSION 3.9)
# Export data for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CPP params
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	# set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
  string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")

  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Wall -Wextra -pedantic -Werror")

  # string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=undefined")
  # string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fsanitize=undefined")
	# add_compile_options(-Wall -Wextra -pedantic -Werror -fsanitize=address)
	add_link_options(-fsanitize=undefined)
endif()

# Set cmake standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Define project
project(goto_elimination LANGUAGES CXX
    VERSION 1.0.0
)

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
    copy-compile-commands ALL
    DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
    )
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
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
