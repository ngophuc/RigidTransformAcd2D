cmake_minimum_required(VERSION 2.8)
project(RigidTransform2D)
cmake_policy(SET CMP0042 NEW)

# SOURCE LIST
aux_source_directory(. SRC_LIST)
file(GLOB SOURCES
    src/*.h src/*.cpp
    src/edge_visibility/*.h src/edge_visibility/*.cpp
    src/hull*.h src/hull/*.cpp
    src/mathtool/*.h src/mathtool/*.cpp
    libs/FIST/*.h libs/FIST/*.cpp
    )

# INCLUDE LIST
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/src/edge_visibility")
include_directories ("${PROJECT_SOURCE_DIR}/src/hull")
include_directories ("${PROJECT_SOURCE_DIR}/src/mathtool")
include_directories ("${PROJECT_SOURCE_DIR}/libs/FIST")

#Required in DGtal
FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
    message(STATUS "Boost and boost_program_options found.")
    include_directories( ${Boost_INCLUDE_DIRS} )
    SET(DGtalToolsLibDependencies ${DGtalToolsLibDependencies}
        ${Boost_LIBRAIRIES}
        ${Boost_PROGRAM_OPTIONS_LIBRARY})
    SET(DGtalLibInc ${Boost_INCLUDE_DIRS})
endif( Boost_FOUND)

add_definitions(-g)

add_executable(polygonalization polygonalization ConnectedComponent Utility)
target_link_libraries(polygonalization ${DGTAL_LIBRARIES} ${DGtalToolsLibDependencies})

add_executable(decomposeShapeAcd2d acd2d_main.cpp ${SOURCES})
target_link_libraries(decomposeShapeAcd2d ${PROJECT_SOURCE_DIR}/libs/FIST/libFIST.osx.a)

add_executable(transformDecomShape transformDecomShape Halfplane Boundingbox Utility RigidTransform)
target_link_libraries(transformDecomShape ${DGTAL_LIBRARIES} ${DGtalToolsLibDependencies})
