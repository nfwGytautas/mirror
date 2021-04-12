# CMakeList.txt : CMake project for mrrc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("mrrc")

message(STATUS "CMake source directory set to: ${CMAKE_SOURCE_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package (LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add source to this project's executable.
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/mrrc/src/*.cpp" "${CMAKE_SOURCE_DIR}/mrrc/src/*.hpp")
message(STATUS "Project source files: ${SOURCE_FILES}")
add_executable (mrrc ${SOURCE_FILES})

target_include_directories (mrrc PUBLIC "${CMAKE_SOURCE_DIR}/mrrc/src/")

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader x86asmparser x86codegen x86desc x86disassembler x86info)
#llvm_map_components_to_libnames(llvm_libs Analysis native ExecutionEngine InstCombine Object OrcJIT RuntimeDyld ScalarOpts support core irreader x86asmparser x86codegen x86desc x86disassembler x86info)

# Link against LLVM libraries
message(STATUS "Linking against: ${llvm_libs}")
target_link_libraries(mrrc ${llvm_libs})
set_property(TARGET mrrc PROPERTY CXX_STANDARD 17)

# TODO: Add tests and install targets
