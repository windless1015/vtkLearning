cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(voxelDisplay)

find_package(VTK REQUIRED COMPONENTS 
  CommonColor
  CommonCore
  CommonDataModel
  IOGeometry
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  ImagingStencil
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "voxelDisplay: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(voxelDisplay MACOSX_BUNDLE voxelDisplay.cpp )
  target_link_libraries(voxelDisplay PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS voxelDisplay
  MODULES ${VTK_LIBRARIES}
)