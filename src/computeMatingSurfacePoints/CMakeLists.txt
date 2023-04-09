cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(computeMatingSurfacePoints)
set(VTK_DIR D:\\Librarys\\vtk9.1.0\\vs2022_build)
find_package(VTK COMPONENTS 
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "computeMatingSurfacePoints: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(computeMatingSurfacePoints MACOSX_BUNDLE
 ../utility/Vector3d.h
 ../utility/Vector3d.cpp
 ../utility/TransformAssistant.h
 ../utility/TransformAssistant.cpp
 ../utility/DisplayComponents.h
 ../utility/DisplayComponents.cpp
 computeMatingSurfacePoints.cxx )
  target_link_libraries(computeMatingSurfacePoints PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS computeMatingSurfacePoints
  MODULES ${VTK_LIBRARIES}
)