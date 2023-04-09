
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(vtkBillboardTextActor3D)
set(VTK_DIR D:\\Librarys\\vtk9.1.0\\vs2022_build)
find_package(VTK COMPONENTS)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "vtkBillboardTextActor3D: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(vtkBillboardTextActor3D MACOSX_BUNDLE
 vtkBillboardTextActor3D.cxx )
 target_link_libraries(vtkBillboardTextActor3D PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS vtkBillboardTextActor3D
  MODULES ${VTK_LIBRARIES}
)