cmake_minimum_required(VERSION 3.15)
project(vtkItkDisplay)
set(VTK_ITK_DISPLAY ${PROJECT_NAME})

#set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/VTK/x64)
#set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/ITK/x64)

set(VTK_DIR D:/projects/ultras/libs/VTK/x64/lib/cmake/vtk-8.2)
set(ITK_DIR D:/projects/ultras/libs/ITK/x64/lib/cmake/ITK-5.1)

find_package(ITK)
find_package(VTK REQUIRED)


#set(ITK_REQUIRED_MODULES "ITKCommon;ITKIOImageBase;ITKIOJPEG;ITKIOGDCM;ITKCommon;ITKImageIntensity;ITKVTK;ITKVtkGlue;ITKIONRRD")

include_directories(
${ITK_INCLUDE_DIRS}
${VTK_INCLUDE_DIRS}
)


add_executable(${VTK_ITK_DISPLAY} vtkItkDisplay.cpp)
target_link_libraries(${VTK_ITK_DISPLAY} 
 ${ITK_LIBRARIES}
 ${VTK_LIBRARIES}
)