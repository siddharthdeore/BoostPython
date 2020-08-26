# author : siddharth deore
# email  : deore.1823670@studenti.uniroma1.it
cmake_minimum_required (VERSION 3.8)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(
    "BoostPython" # This will exposed as the variable PROJECT_NAME.
    VERSION 0.1.0 # Used for installation and defines variables PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, and PROJECT_VERSION_TWEAK.
    LANGUAGES CXX # Used to determine the languages to use based on file extensions
	DESCRIPTION "Cross platform Boost Python Library Wrapper"
	)

# CPP standard
set(CMAKE_CXX_STANDARD 17)

# Build settings
if(NOT CMAKE_BUILD_TYPE)
  #SET(CMAKE_BUILD_TYPE "DEBUG")
  SET(CMAKE_BUILD_TYPE "RELEASE")
  #SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
  #SET(CMAKE_BUILD_TYPE "MINSIZEREL")
endif()

# directory structure
include_directories(src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# requirements: Python
# set environment variables: PYTHON_EXECUTABLE, PYTHON_INCLUDE_DIRS, PYTHON_LIBRARIES

# requirements: Boost
# set environment variables: BOOST_ROOT, BOOST_LIBRARYDIR, BOOST_INCLUDEDIR

# Boost configration flags
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_COMPILER "-vc142")
#set(Boost_ARCHITECTURE "-x86")
set(Boost_ARCHITECTURE "-x64")

# Find python
FIND_PACKAGE(PythonInterp)
# Find boost for installed python version
if (PYTHONINTERP_FOUND)
  if (UNIX AND NOT APPLE)
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_SUFFIX} numpy${PYTHON_VERSION_SUFFIX})
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost COMPONENTS python numpy)
        FIND_PACKAGE(PythonInterp)
        FIND_PACKAGE(PythonLibs REQUIRED)
    endif()
  else()	
    if (PYTHON_VERSION_MAJOR EQUAL 3)
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} numpy${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        FIND_PACKAGE(PythonInterp 3)
        FIND_PACKAGE(PythonLibs 3 REQUIRED)
    else()
        FIND_PACKAGE(Boost COMPONENTS python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} numpy${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR})
        FIND_PACKAGE(PythonInterp)
        FIND_PACKAGE(PythonLibs REQUIRED)
    endif()
  endif()

else()
    message("Python not found Make sure you have python installed with envirnment variables")
endif()



if(Boost_FOUND)
	message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
	message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
	message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")

	message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost_LIBRARY_DIR = ${Boost_LIBRARY_DIRS}")
	message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

	# include directories
	include_directories(${PROJECT_SOURCE_DIR}/src)
	include_directories(${PROJECT_SOURCE_DIR}/include)
	include_directories(${PYTHON_INCLUDE_DIRS})
	include_directories(${Boost_INCLUDE_DIRS})

	
	# - This is where you add the source files

	# Add source to this project's executable.
	add_executable (BoostPython
	"src/main.cpp"
	"include/NumPyArrayData.h"
	) 
	# e.g.  PROJECT_NAME.exe on windows
	
	# Add a shared module - modules are intended to be imported at runtime.
	add_library(pyBoostPython MODULE
	"src/pyBoostPython.cpp"
	"include/NumPyArrayData.h"

	)
	# e.g.  PROJECT_NAME.dll on windows PROJECT_NAME.so on UNIX


	# link static libraries 
	if((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") OR APPLE)
	     target_link_libraries(pyBoostPython ${Boost_LIBRARIES})
	elseif(MSVC)
		add_definitions(/DBOOST_PYTHON_STATIC_LIB) 
		# This is also required to force the usage of libboost_python37-vc142-mt-x64-1_70.lib instead of boost_python37-vc142-mt-x64-1_70.lib
		target_link_libraries(pyBoostPython ${Boost_LIBRARIES})
		# This includes the Boost Python library Even though Boost Python library is included statically, in Windows it has a dependency to the Python library.	
		target_link_libraries(pyBoostPython ${PYTHON_LIBRARIES})
		# target_compile_definitions(BoostPython_ext PRIVATE BOOST_AUTO_LINK_SYSTEM)


	endif()


ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Make sure Boost is installed")
ENDIF()