cmake_minimum_required(VERSION 3.0) # setting this is required
project(bb_solver)            # this sets the project name

###############################################################################
## file globbing ##############################################################
###############################################################################

# These instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`.
file(GLOB_RECURSE sources      src/main/*.cpp src/main/*.h)
file(GLOB_RECURSE sources_test src/test/*.cpp)
file(GLOB_RECURSE data resources/*)
# You can use set(sources src/main.cpp) etc if you don't want to
# use globbing to find files automatically.

###############################################################################
## target definitions #########################################################
###############################################################################

# The data is just added to the executable, because in some IDEs (QtCreator) 
# files are invisible when they are not explicitly part of the project.
# add_executable(bb_solver ${sources} ${data})
add_executable(bb_solver src/main/main.cpp)

# Just for example add some compiler flags.
target_compile_options(bb_solver PUBLIC)
set_target_properties(bb_solver PROPERTIES
            CXX_STANDARD 17
            )
set(CMAKE_CXX_FLAGS "-O3")
# This allows to include files relative to the root of the src directory with a <> pair
target_include_directories(bb_solver PUBLIC src/main)

# This copies all resource files in the build directory.
# We need this, because we want to work with paths relative to the executable.
# file(COPY ${data} DESTINATION resources)

###############################################################################
## dependencies ###############################################################
###############################################################################

# This defines the variables Boost_LIBRARIES that containts all library names
# that we need to link into the program.
# find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)

# target_link_libraries(bb_solver PUBLIC
#   ${Boost_LIBRARIES}
#   # here you can add any library dependencies
# )