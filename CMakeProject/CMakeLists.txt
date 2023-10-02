# CMakeList.txt : CMake project for CMakeProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeProject")

# Add source to this project's executable.
add_executable (CMakeProject "algorithm.cpp"  "main.cpp" "struct.h" )

#"funccos.cpp" "funcln.cpp" "funcpower.cpp" "funcsin.cpp" "functan.cpp"
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject PROPERTY CXX_STANDARD 20)
endif()

configure_file(${PROJECT_SOURCE_DIR}/plugins/funccos.dll ${PROJECT_BINARY_DIR}/plugins/funccos.dll COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/plugins/funcsin.dll ${PROJECT_BINARY_DIR}/plugins/funcsin.dll COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/plugins/funcln.dll ${PROJECT_BINARY_DIR}/plugins/funcln.dll COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/plugins/funcpower.dll ${PROJECT_BINARY_DIR}/plugins/funcpower.dll COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/plugins/functan.dll ${PROJECT_BINARY_DIR}/plugins/functan.dll COPYONLY)
# TODO: Add tests and install targets if needed.