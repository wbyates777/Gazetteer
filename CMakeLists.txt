cmake_minimum_required(VERSION 3.14)
project(Gazetteer 
        VERSION 1.0.0 
        LANGUAGES CXX)

# Force modern C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the core library sources
set(LIB_SOURCES
    City.cpp
    Country.cpp
    Currency.cpp
    Gazetteer.cpp
    GeoCoord.cpp
    Locode.cpp
    MarketId.cpp
    Name.cpp
)

# Define the core library headers
set(LIB_HEADERS
    City.h
    Country.h
    Currency.h
    Gazetteer.h
    GeoCoord.h
    LOCODES_LARGE_HEADER.h
    LOCODES_SMALL_HEADER.h
    Locode.h
    MarketId.h
    Name.h
)

# 1. Create the Gazetteer library target
add_library(gazetteer_lib STATIC ${LIB_SOURCES} ${LIB_HEADERS})

# Configure include directories for the library
target_include_directories(gazetteer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 2. Create the main demo executable target
add_executable(gazetteer_demo main.cpp)

# Link the demo executable to your library
target_link_libraries(gazetteer_demo PRIVATE gazetteer_lib)

# 3. (Optional) Create the LOCODE utility executable target
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LOCODE_main.cpp")
    add_executable(locode_utility LOCODE_main.cpp)
    target_link_libraries(locode_utility PRIVATE gazetteer_lib)
endif()
