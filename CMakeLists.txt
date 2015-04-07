CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(sdlglapp)

ADD_EXECUTABLE(sdlglapp
	dependencies/tinyobjloader/tiny_obj_loader.h
	dependencies/tinyobjloader/tiny_obj_loader.cc
	include/BinaryData.h
	include/Camera.h
	include/Common.h
	include/Frustum.h
	include/GpuProgram.h
	include/Material.h
	include/SceneNode.h
	include/Renderer.h
	include/Shader.h
	include/Terrain.h
	include/Texture.h
	src/BinaryData.cpp
	src/Camera.cpp
	src/Frustum.cpp
	src/GpuProgram.cpp
	src/main.cpp
	src/Material.cpp
	src/SceneNode.cpp
	src/Renderer.cpp
	src/Shader.cpp
	src/Terrain.cpp
	src/Texture.cpp
)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image)
#PKG_SEARCH_MODULE(SDL2_ttf REQUIRED SDL2_ttf)
#PKG_SEARCH_MODULE(SDL2_mixer REQUIRED SDL2_mixer)
#PKG_SEARCH_MODULE(SDL2_net REQUIRED SDL2_net)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include)
include_directories(dependencies/glm)
include_directories(dependencies/tinyobjloader)
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if(NOT OPENGL_GLU_FOUND)
	message(ERROR " OPENGL GLU not found")
endif(NOT OPENGL_GLU_FOUND)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SDL2_image_INCLUDE_DIRS})
LINK_DIRECTORIES(${SDL2_LIBRARY_DIRS})
LINK_DIRECTORIES(${SDL2_image_Library_DIRS})
TARGET_LINK_LIBRARIES(sdlglapp
	${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY} ${SDL2_image_LIBRARIES}
)

#${SDL2_LIBRARIES} ${SDL2_image_LIBRARIES} ${SDL2_ttf_LIBRARIES} ${SDL2_mixer_LIBRARIES} ${SDL2_net_LIBRARIES} ${OPENGL_LIBRARIES} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY})
