cmake_minimum_required (VERSION 2.6)

project (SuperBible6)

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(COMMON_LIBS sb6 glfw GL rt)

add_library( sb6 src/sb6/sb6.cpp src/sb6/sb6ktx.cpp src/sb6/sb6object.cpp src/sb6/sb6shader.cpp src/sb6/gl3w.c )

add_executable(alienrain src/alienrain/alienrain.cpp)
TARGET_LINK_LIBRARIES(alienrain ${COMMON_LIBS})

add_executable(basicfbo src/basicfbo/basicfbo.cpp)
TARGET_LINK_LIBRARIES(basicfbo ${COMMON_LIBS})

# add_executable(bindlesstexture src/bindlesstexture/bindlesstexture.cpp)
# TARGET_LINK_LIBRARIES(bindlesstexture sb6 glfw GL)

add_executable(blendmatrix src/blendmatrix/blendmatrix.cpp)
TARGET_LINK_LIBRARIES(blendmatrix ${COMMON_LIBS})

add_executable(blinnphong src/blinnphong/blinnphong.cpp)
TARGET_LINK_LIBRARIES(blinnphong ${COMMON_LIBS})

add_executable(bumpmapping src/bumpmapping/bumpmapping.cpp)
TARGET_LINK_LIBRARIES(bumpmapping ${COMMON_LIBS})

add_executable(clipdistance src/clipdistance/clipdistance.cpp)
TARGET_LINK_LIBRARIES(clipdistance ${COMMON_LIBS})

add_executable(csflocking src/csflocking/csflocking.cpp)
TARGET_LINK_LIBRARIES(csflocking ${COMMON_LIBS})

add_executable(cubemapenv src/cubemapenv/cubemapenv.cpp)
TARGET_LINK_LIBRARIES(cubemapenv ${COMMON_LIBS})

add_executable(cubicbezier src/cubicbezier/cubicbezier.cpp)
TARGET_LINK_LIBRARIES(cubicbezier ${COMMON_LIBS})

add_executable(deferredshading src/deferredshading/deferredshading.cpp)
TARGET_LINK_LIBRARIES(deferredshading ${COMMON_LIBS})

add_executable(depthclamp src/depthclamp/depthclamp.cpp)
TARGET_LINK_LIBRARIES(depthclamp ${COMMON_LIBS})

add_executable(dispmap src/dispmap/dispmap.cpp)
TARGET_LINK_LIBRARIES(dispmap ${COMMON_LIBS})

add_executable(dof src/dof/dof.cpp)
TARGET_LINK_LIBRARIES(dof ${COMMON_LIBS})

add_executable(envmapsphere src/envmapsphere/envmapsphere.cpp)
TARGET_LINK_LIBRARIES(envmapsphere ${COMMON_LIBS})

add_executable(equirectangular src/equirectangular/equirectangular.cpp)
TARGET_LINK_LIBRARIES(equirectangular ${COMMON_LIBS})

add_executable(fragcolorfrompos src/fragcolorfrompos/fragcolorfrompos.cpp)
TARGET_LINK_LIBRARIES(fragcolorfrompos ${COMMON_LIBS})

add_executable(fragmentlist src/fragmentlist/fragmentlist.cpp)
TARGET_LINK_LIBRARIES(fragmentlist ${COMMON_LIBS})

add_executable(grass src/grass/grass.cpp)
TARGET_LINK_LIBRARIES(grass ${COMMON_LIBS})

add_executable(gsculling src/gsculling/gsculling.cpp)
TARGET_LINK_LIBRARIES(gsculling ${COMMON_LIBS})

add_executable(gslayered src/gslayered/gslayered.cpp)
TARGET_LINK_LIBRARIES(gslayered ${COMMON_LIBS})

add_executable(gsquads src/gsquads/gsquads.cpp)
TARGET_LINK_LIBRARIES(gsquads ${COMMON_LIBS})

add_executable(gstessellate src/gstessellate/gstessellate.cpp)
TARGET_LINK_LIBRARIES(gstessellate ${COMMON_LIBS})

add_executable(hdrbloom src/hdrbloom/hdrbloom.cpp)
TARGET_LINK_LIBRARIES(hdrbloom ${COMMON_LIBS})

add_executable(hdrexposure src/hdrexposure/hdrexposure.cpp)
TARGET_LINK_LIBRARIES(hdrexposure ${COMMON_LIBS})

add_executable(hdrtonemap src/hdrtonemap/hdrtonemap.cpp)
TARGET_LINK_LIBRARIES(hdrtonemap ${COMMON_LIBS})

add_executable(indexedcube src/indexedcube/indexedcube.cpp)
TARGET_LINK_LIBRARIES(indexedcube ${COMMON_LIBS})

add_executable(instancedattribs src/instancedattribs/instancedattribs.cpp)
TARGET_LINK_LIBRARIES(instancedattribs ${COMMON_LIBS})

add_executable(julia src/julia/julia.cpp)
TARGET_LINK_LIBRARIES(julia ${COMMON_LIBS})

add_executable(ktxview src/ktxview/ktxview.cpp)
TARGET_LINK_LIBRARIES(ktxview ${COMMON_LIBS})

add_executable(linesmooth src/linesmooth/linesmooth.cpp)
TARGET_LINK_LIBRARIES(linesmooth ${COMMON_LIBS})

add_executable(msaanative src/msaanative/msaanative.cpp)
TARGET_LINK_LIBRARIES(msaanative ${COMMON_LIBS})

add_executable(multidrawindirect src/multidrawindirect/multidrawindirect.cpp)
TARGET_LINK_LIBRARIES(multidrawindirect ${COMMON_LIBS})

add_executable(multimaterial src/multimaterial/multimaterial.cpp)
TARGET_LINK_LIBRARIES(multimaterial ${COMMON_LIBS})

add_executable(multiscissor src/multiscissor/multiscissor.cpp)
TARGET_LINK_LIBRARIES(multiscissor ${COMMON_LIBS})

add_executable(multiviewport src/multiviewport/multiviewport.cpp)
TARGET_LINK_LIBRARIES(multiviewport ${COMMON_LIBS})

add_executable(noise src/noise/noise.cpp)
TARGET_LINK_LIBRARIES(noise ${COMMON_LIBS})

add_executable(noperspective src/noperspective/noperspective.cpp)
TARGET_LINK_LIBRARIES(noperspective ${COMMON_LIBS})

add_executable(normalviewer src/normalviewer/normalviewer.cpp)
TARGET_LINK_LIBRARIES(normalviewer ${COMMON_LIBS})

add_executable(objectexploder src/objectexploder/objectexploder.cpp)
TARGET_LINK_LIBRARIES(objectexploder ${COMMON_LIBS})

add_executable(perpixelgloss src/perpixelgloss/perpixelgloss.cpp)
TARGET_LINK_LIBRARIES(perpixelgloss ${COMMON_LIBS})

add_executable(phonglighting src/phonglighting/phonglighting.cpp)
TARGET_LINK_LIBRARIES(phonglighting ${COMMON_LIBS})

add_executable(polygonsmooth src/polygonsmooth/polygonsmooth.cpp)
TARGET_LINK_LIBRARIES(polygonsmooth ${COMMON_LIBS})

add_executable(prefixsum src/prefixsum/prefixsum.cpp)
TARGET_LINK_LIBRARIES(prefixsum ${COMMON_LIBS})

add_executable(prefixsum2d src/prefixsum2d/prefixsum2d.cpp)
TARGET_LINK_LIBRARIES(prefixsum2d ${COMMON_LIBS})

add_executable(programinfo src/programinfo/programinfo.cpp)
TARGET_LINK_LIBRARIES(programinfo ${COMMON_LIBS})

add_executable(raytracer src/raytracer/raytracer.cpp)
TARGET_LINK_LIBRARIES(raytracer ${COMMON_LIBS})

add_executable(rimlight src/rimlight/rimlight.cpp)
TARGET_LINK_LIBRARIES(rimlight ${COMMON_LIBS})

add_executable(sampleshading src/sampleshading/sampleshading.cpp)
TARGET_LINK_LIBRARIES(sampleshading ${COMMON_LIBS})

# add_executable(sb6mrender src/sb6mrender/sb6mrender.cpp)
# TARGET_LINK_LIBRARIES(sb6mrender sb6 glfw GL)

add_executable(shadowmapping src/shadowmapping/shadowmapping.cpp)
TARGET_LINK_LIBRARIES(shadowmapping ${COMMON_LIBS})

add_executable(shapedpoints src/shapedpoints/shapedpoints.cpp)
TARGET_LINK_LIBRARIES(shapedpoints ${COMMON_LIBS})

add_executable(simpleclear src/simpleclear/simpleclear.cpp)
TARGET_LINK_LIBRARIES(simpleclear ${COMMON_LIBS})

add_executable(simpletexcoords src/simpletexcoords/simpletexcoords.cpp)
TARGET_LINK_LIBRARIES(simpletexcoords ${COMMON_LIBS})

add_executable(simpletexture src/simpletexture/simpletexture.cpp)
TARGET_LINK_LIBRARIES(simpletexture ${COMMON_LIBS})

add_executable(singlepoint src/singlepoint/singlepoint.cpp)
TARGET_LINK_LIBRARIES(singlepoint ${COMMON_LIBS})

add_executable(singletri src/singletri/singletri.cpp)
TARGET_LINK_LIBRARIES(singletri ${COMMON_LIBS})

add_executable(spinnycube src/spinnycube/spinnycube.cpp)
TARGET_LINK_LIBRARIES(spinnycube ${COMMON_LIBS})

add_executable(springmass src/springmass/springmass.cpp)
TARGET_LINK_LIBRARIES(springmass ${COMMON_LIBS})

add_executable(ssao src/ssao/ssao.cpp)
TARGET_LINK_LIBRARIES(ssao ${COMMON_LIBS})

add_executable(starfield src/starfield/starfield.cpp)
TARGET_LINK_LIBRARIES(starfield ${COMMON_LIBS})

add_executable(stereo src/stereo/stereo.cpp)
TARGET_LINK_LIBRARIES(stereo ${COMMON_LIBS})

add_executable(subroutines src/subroutines/subroutines.cpp)
TARGET_LINK_LIBRARIES(subroutines ${COMMON_LIBS})

add_executable(tessellatedcube src/tessellatedcube/tessellatedcube.cpp)
TARGET_LINK_LIBRARIES(tessellatedcube ${COMMON_LIBS})

add_executable(tessellatedgstri src/tessellatedgstri/tessellatedgstri.cpp)
TARGET_LINK_LIBRARIES(tessellatedgstri ${COMMON_LIBS})

add_executable(tessellatedtri src/tessellatedtri/tessellatedtri.cpp)
TARGET_LINK_LIBRARIES(tessellatedtri ${COMMON_LIBS})

add_executable(tessmodes src/tessmodes/tessmodes.cpp)
TARGET_LINK_LIBRARIES(tessmodes ${COMMON_LIBS})

add_executable(tesssubdivmodes src/tesssubdivmodes/tesssubdivmodes.cpp)
TARGET_LINK_LIBRARIES(tesssubdivmodes ${COMMON_LIBS})

add_executable(toonshading src/toonshading/toonshading.cpp)
TARGET_LINK_LIBRARIES(toonshading ${COMMON_LIBS})

add_executable(tunnel src/tunnel/tunnel.cpp)
TARGET_LINK_LIBRARIES(tunnel ${COMMON_LIBS})

# add_executable(win32raw src/win32raw/win32raw.cpp)
# TARGET_LINK_LIBRARIES(win32raw)

add_executable(wrapmodes src/wrapmodes/wrapmodes.cpp)
TARGET_LINK_LIBRARIES(wrapmodes ${COMMON_LIBS})

add_executable(xraw src/xraw/xraw.c)
TARGET_LINK_LIBRARIES(xraw GL m)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories( include )
include_directories( extern/glfw-2.7.6/include )

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})

