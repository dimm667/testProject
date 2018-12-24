cmake_minimum_required(VERSION 3.0)
project(testProject)


set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)


# link to glfw library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw-3.2.1/)

#link to glad
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/glad/include/
                    ${CMAKE_SOURCE_DIR}/deps/)

set(GLAD ${CMAKE_SOURCE_DIR}/deps/glad/include/glad/glad.h
         ${CMAKE_SOURCE_DIR}/deps/glad/src/glad.c)

set(STB_IMAGE
    ${CMAKE_SOURCE_DIR}/deps/stb_image.h)

set(SHADERS
    ${CMAKE_SOURCE_DIR}/shaders/vertex.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexSimply.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexGlyph.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragment.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentSimply.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentLamp.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentLighted.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentMaterial.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentDiffuseOnly.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentGlyph.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexSkyBox.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexSkyBox2.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentSkyBox.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexReflectSkyBox.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentReflectSkyBox.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentRefractSkyBox.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexInstancing.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexInstancing2.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentMaterialVertex.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexInstancingLighting.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexDirectionLight.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentDirectionLight.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexQuad.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentQuad.glsl
    ${CMAKE_SOURCE_DIR}/shaders/vertexPointLightShadow.glsl
    ${CMAKE_SOURCE_DIR}/shaders/geometryPointLightShadow.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentPointLightShadow.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentDefferedShading.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragmentMaterialVertexDeffered.glsl)

set(TEXTURES
     ${CMAKE_SOURCE_DIR}/textures/container.jpg
     ${CMAKE_SOURCE_DIR}/textures/wall.jpg
     ${CMAKE_SOURCE_DIR}/textures/container2.png
     ${CMAKE_SOURCE_DIR}/textures/container2_specular.png
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/back.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/bottom.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/front.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/left.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/right.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/lake/top.jpg
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_bk.JPG
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_dn.JPG
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_ft.JPG
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_lf.JPG
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_rt.JPG
     ${CMAKE_SOURCE_DIR}/textures/skybox/sor_cwd/cwd_up.JPG
     ${CMAKE_SOURCE_DIR}/textures/Heightmap.png
     ${CMAKE_SOURCE_DIR}/textures/myHeightmap.png
     )

 set(FONTS
     ${CMAKE_SOURCE_DIR}/fonts/arial.ttf)

SET(SOURCES

    shaders/UniformStructures/matrices.h
    shaderprogram.cpp
    shaderprogram.h
    viewport.h
    camera.cpp
    camera.h
    imageloader.cpp
    imageloader.h
    renderbuffer.h
    framebuffer.h
    texture.cpp
    texture.h
    mesh.cpp
    mesh.h
    model.cpp
    model.h
    cube.cpp
    cube.h
    fpsestimator.cpp
    fpsestimator.h
    voxel.cpp
    voxel.h
    voxexporter.cpp
    voxexporter.h
    uniformobject.cpp
    uniformobject.h
    worlddatacontainer.cpp
    worlddatacontainer.h
    main.cpp
    )

file(GLOB_RECURSE shaders_data ${CMAKE_SOURCE_DIR}/shaders/*)
file(GLOB_RECURSE textures_data LIST_DIRECTORIES true textures/*)
file(GLOB_RECURSE fonts_data ${CMAKE_SOURCE_DIR}/fonts/*)
file(GLOB_RECURSE libs_data ${CMAKE_SOURCE_DIR}/deps/libassimp.dll)
file(GLOB_RECURSE models_data LIST_DIRECTORIES true models/*)

link_directories(${CMAKE_SOURCE_DIR}/deps/)
add_library(assimp STATIC IMPORTED)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${STB_IMAGE}
    ${GLAD}
    ${SHADERS}
    ${TEXTURES}
    ${FONTS})

target_link_libraries(${PROJECT_NAME} glfw "assimp.dll" "freetype")

file(COPY ${shaders_data} DESTINATION shaders)
file(COPY ${textures_data} DESTINATION textures)
file(COPY ${fonts_data} DESTINATION fonts)
file(COPY ${libs_data} DESTINATION .)
file(COPY ${models_data} DESTINATION models/)




