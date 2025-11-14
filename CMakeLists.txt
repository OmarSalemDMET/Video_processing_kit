# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and language
project(MovingBackgroundFilter
    VERSION 1.0
    LANGUAGES CXX
)

# Set the C++ standard (modern C++ is highly recommended)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- 1. FIND OPENCV ---
# This command searches for the OpenCV package configuration file.
# It sets OPENCV_FOUND, OPENCV_INCLUDE_DIRS, and OPENCV_LIBS (or similar variables).
find_package(OpenCV REQUIRED)

# Check if OpenCV was found and print confirmation (optional, but good practice)
if(NOT OPENCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please ensure it is installed and configured correctly.")
endif()

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries to link: ${OpenCV_LIBS}")

set(PROJECT_SOURCE_FILES
    main.cpp
    vid.cpp

)

# --- 2. DEFINE EXECUTABLE TARGET ---
# Define the executable target using the source file.
add_executable(app ${PROJECT_SOURCE_FILES})


# --- 3. LINK LIBRARIES AND INCLUDES ---
# Link the executable against the necessary OpenCV libraries.
# target_link_libraries is used here as it handles both public and private dependencies.
target_link_libraries(app PRIVATE ${OpenCV_LIBS})

# Add the necessary include directories for OpenCV headers.
# Note: In modern CMake, target_link_libraries often automatically handles includes 
# if the package provides targets (like OpenCV::core, OpenCV::highgui), 
# but using target_include_directories is explicit and safe.
target_include_directories(app PRIVATE ${OpenCV_INCLUDE_DIRS})

# If you prefer using targets (modern CMake recommended way):
# find_package(OpenCV REQUIRED COMPONENTS core highgui imgcodecs)
# target_link_libraries(opencv_app PRIVATE OpenCV::core OpenCV::highgui OpenCV::imgcodecs)