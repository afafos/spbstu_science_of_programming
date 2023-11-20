cmake_minimum_required(VERSION 3.26)
project(XmlTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(XmlTree main.cpp XmlParser.cpp XmlResource.cpp)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/tree.xml
        ${CMAKE_CURRENT_BINARY_DIR}/tree.xml
        COPYONLY
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/tree1.xml
        ${CMAKE_CURRENT_BINARY_DIR}/tree1.xml
        COPYONLY
)