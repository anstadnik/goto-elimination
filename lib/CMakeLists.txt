file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cc ast/*.cc goto_elimination/*.cc) 

add_library(${PROJECT_NAME} ${SOURCES})

# Add algs
add_subdirectory(algs)
target_link_libraries(${PROJECT_NAME} algs)

# target_precompile_headers(${PROJECT_NAME} PRIVATE ast/ast.h)

# Threads
# find_package(Threads)
# target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# OpenMP
# option(USE_OPENMP "flag to use OpenMP for multithreading" ON)
# if(USE_OPENMP)
#     find_package(OpenMP 3.0 COMPONENTS CXX)
#     if (OpenMP_CXX_FOUND)
#         message(STATUS "Building with OpenMP Multithreading.")
#     else()
#         message(STATUS "No OpenMP found, no multithreading.")
#     endif()
# endif()
