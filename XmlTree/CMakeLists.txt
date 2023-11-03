cmake_minimum_required(VERSION 3.26)
project(XmlTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(XmlTree main.cpp XmlParser.cpp XmlResource.cpp)
