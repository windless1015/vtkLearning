cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(vtkTransformExplanation)
set(VTK_DIR D:\\Librarys\\vtk9.1.0\\vs2022_build)
find_package(VTK COMPONENTS 
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "vtkTransformExplanation: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(vtkTransformExplanation MACOSX_BUNDLE
 Vector3d.h
 Vector3d.cpp
 TransformAssistant.h
 TransformAssistant.cpp
 vtkTransformExplanation.cxx )
  target_link_libraries(vtkTransformExplanation PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS vtkTransformExplanation
  MODULES ${VTK_LIBRARIES}
)