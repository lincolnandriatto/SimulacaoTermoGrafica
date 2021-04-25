cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project      (Ep)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


#apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev libxi-dev

#link_directories(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

if(WIN32)
  set(LIBS glfw3 opengl32 assimp)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS )
endif(WIN32)


set  (project_SOURCES main.cpp 
 ColorRGBA.hpp
Point.hpp
point3dutils.hpp
GL_interactor.cpp
scrInteractor.cpp
TrackBall.cpp
Gizmo.cpp
includes/imgui/imgui.cpp .
includes/imgui/imgui_demo.cpp
includes/imgui/imgui_draw.cpp
includes/imgui/imgui_widgets.cpp
includes/imgui/imgui_impl_glfw.cpp
includes/imgui/imgui_impl_opengl3.cpp
lgl/Error.cpp
lgl/Shader.cpp
)
set  (project_HEADERS 
Cores.h
GL_interactor.h
scrInteractor.h
TrackBall.h
Gizmo.h
of/ofBase.h
of/ofBinaryIO.h
of/ofBoundaryCellCIterator2D.h
of/ofBoundaryCellIterator2D.h
of/ofBoundaryCellIterator.h
of/ofCell2D.h
of/ofCell3D.h
of/ofCell.h
of/ofCellsIterator.h
of/ofCgmPointsReader.h
of/ofCgmReader.h
of/ofDelaunay2D.h
of/ofDiDo3D.h
of/ofGeometric.h
of/of.h
of/ofIterator.h
of/ofKdTree.h
of/ofList.h
of/ofMacros.h
of/ofMarkFrat.h
of/ofMesh2D.h
of/ofMesh3D.h
of/ofMesh.h
of/ofMeshNOSurface.h
of/ofMeshOper.h
of/ofMeshSurface.h
of/ofMeshWriter.h
of/ofMyCell.h
of/ofMyDelaunay2D.h
of/ofMyGeometric.h
of/ofMyMesh.h
of/ofMyOffReader.h
of/ofMyof.h
of/ofMyTraits.h
of/ofMyVertex3D.h
of/ofMyVis.h
of/ofOffPointsReader.h
of/ofOffReader.h
of/ofOffWriter.h
of/ofOfReader.h
of/ofOfWriter.h
of/ofOperators.h
of/ofParameters.h
of/ofPgReader.h
of/ofPlane.h
of/ofPoints2DReader.h
of/ofPolyDataPointsReader.h
of/ofPolyDataReader.h
of/ofPolyDataWriter.h
of/ofPolygonBoundaries.h
of/ofPolygon.h
of/ofPolyWriter.h
of/ofReader.h
of/ofRemesh.h
of/ofRuppert2D.h
of/ofSearchDIDO.h
of/ofSing.h
of/ofSingularVertex.h
of/ofTetGen3D.h
of/ofTetGen3DReader.h
of/ofTetGenReader.h
of/ofTopology.h
of/ofTraits.h
of/ofTransfSolution.h
of/ofTReader.h
of/ofVector.h
of/ofVertex2D.h
of/ofVertex3D.h
of/ofVertex.h
of/ofVertexStarIterator2D.h
of/ofVertexStarIterator3D.h
of/ofVertexStarIterator.h
of/ofVertexStarIteratorSurface.h
of/ofVertexStarIteratorSurfaceVertex.h
of/ofVerticesIterator.h
of/ofVis2D.h
of/ofVtkPointsReader.h
of/ofvtkreader.h
of/ofVtkWriter.h
of/ofWriter.h
of/ofWrlReader.h
of/ofXmlParser.h
of/pair.h
of/ShePolyWriter.h
)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries requried for other projects
add_library(STB_IMAGE "stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "glad.c")
set(LIBS ${LIBS} GLAD)

#macro(makeLink src dest target)
#  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
#endmacro()

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories( ${CMAKE_SOURCE_DIR}/includes/imgui)
include_directories( ${CMAKE_SOURCE_DIR}/VisOf )
include_directories( ${CMAKE_SOURCE_DIR}/VisOf/Utils )
include_directories( ${CMAKE_SOURCE_DIR}/VisOf/iterFunc )
include_directories( ${CMAKE_SOURCE_DIR}/of )
include_directories( ${OPENGL_INCLUDE_DIRS} )
ADD_EXECUTABLE  (ep main.cpp Gizmo.cpp
    includes/imgui/imgui.cpp
    includes/imgui/imgui_demo.cpp
    includes/imgui/imgui_draw.cpp
    includes/imgui/imgui_widgets.cpp
    includes/imgui/imgui_impl_glfw.cpp
    includes/imgui/imgui_impl_opengl3.cpp
    lgl/Error.cpp
    lgl/Shader.cpp
    scrInteractor.cpp
    TrackBall.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglfw -lGL -lX11 -lpthread -lm  -DIMGUI_IMPL_OPENGL_LOADER_GLAD")

target_link_libraries(ep ${OPENGL_LIBRARIES} ${LIBS}  )





