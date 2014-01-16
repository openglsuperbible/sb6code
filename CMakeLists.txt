cmake_minimum_required (VERSION 2.6)

project (SuperBible6)

#Sets the directory for cmake to find Cmake configration packages,
#these packages allow for cmake to find libraries that are not included
#in the standard installation of Cmake. The standard packages can be
#found @ /usr/share/cmake-2.8/Modules (on Unix Machines)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
                      ${CMAKE_MODULE_PATH})

#Attempt to search for the necessary Library requirements
find_package(OpenGL)
find_package(GLFW)

LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#If we found the two libraries 
if(${OPENGL_FOUND} STREQUAL "TRUE" AND ${GLFW_FOUND} STREQUAL "YES")
	include_directories(${OPENGL_INCLUDE_DIR})
	include_directories(${GLFW_INCLUDE_DIR})
	set(COMMON_LIBS sb6 ${OPENGL_LIBRARIES} ${GLFW_LIBRARY})
else()
	if(WIN32)
		set(COMMON_LIBS sb6 optimized GLFW_r32 debug GLFW_d32)
	elif (UNIX)
		set(COMMON_LIBS sb6 glfw ${GLFW_LIBRARIES} GL rt dl)
	else()
		set(COMMON_LIBS sb6)
	endif()
endif()

IF(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY})
  SET(EXTRA_LIBS ${EXTRA_LIBS} /usr/local/lib/libglfw.a)
ENDIF(APPLE)

set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

add_library(sb6
src/sb6/sb6.cpp
src/sb6/sb6ktx.cpp
src/sb6/sb6object.cpp
src/sb6/sb6shader.cpp
src/sb6/gl3w.c
)

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(EXAMPLES
  alienrain
  basicfbo
  blendmatrix
  blinnphong
  bumpmapping
  clipdistance
  csflocking
  cubemapenv
  cubicbezier
  deferredshading
  depthclamp
  dispmap
  dof
  envmapsphere
  equirectangular
  fragcolorfrompos
  fragmentlist
  grass
  gsculling
  gslayered
  gsquads
  gstessellate
  hdrbloom
  hdrexposure
  hdrtonemap
  indexedcube
  instancedattribs
  julia
  ktxview
  linesmooth
  movingtri
  msaanative
  multidrawindirect
  multimaterial
  multiscissor
  multiviewport
  noise
  noperspective
  normalviewer
  objectexploder
  perf-readpixels
  perpixelgloss
  phonglighting
  polygonsmooth
  prefixsum
  prefixsum2d
  programinfo
  raytracer
  rimlight
  sampleshading
# sb6mrender
  shadowmapping
  shapedpoints
  simpleclear
  simpletexcoords
  simpletexture
  singlepoint
  singletri
  sparsetexture
  spinnycube
  springmass
  ssao
  starfield
  stereo
  subroutines
  tessellatedcube
  tessellatedgstri
  tessellatedtri
  tessmodes
  tesssubdivmodes
  toonshading
  tunnel
  wrapmodes
)

if (WIN32)
  set(EXAMPLES ${EXAMPLES} win32raw)
endif()

#if(UNIX)
#  add_executable(xraw src/xraw/xraw.c)
#  target_link_libraries(xraw GL X11 m)
#endif(UNIX)

foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} WIN32 src/${EXAMPLE}/${EXAMPLE}.cpp)
  target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
  if(MSVC)
configure_file(${PROJECT_SOURCE_DIR}/build/vs2010/VisualStudioUserFile.vcproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcproj.user @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/vs2010/VisualStudioUserFile.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcxproj.user @ONLY)
  endif(MSVC)
endforeach(EXAMPLE)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories( include )
include_directories( extern/glfw-2.7.6/include )

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})
