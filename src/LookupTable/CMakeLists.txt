cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(LookupTable)

find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  CommonDataModel
  FiltersPoints
  FiltersSources
  ImagingSources
  IOGeometry
  IOLegacy
  IOPLY
  IOXML
  ImagingCore
  InteractionStyle
  RenderingAnnotation
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "LookupTable: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(LookupTable MACOSX_BUNDLE LookupTable.cpp )
  target_link_libraries(LookupTable PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS LookupTable
  MODULES ${VTK_LIBRARIES}
)