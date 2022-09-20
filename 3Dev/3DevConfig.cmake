get_filename_component(3DEV_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(3DEV_INCLUDE_DIR "${3DEV_DIR}/../../../include/3Dev" ABSOLUTE)

find_package(SFML 2.5.1 COMPONENTS system window network audio REQUIRED)
find_package(assimp REQUIRED)
if(UNIX OR APPLE)
	find_package(GLEW 2.1.0 REQUIRED)
    if(APPLE)
		set(ANGELSCRIPT_LIBRARY_NAME Angelscript)
	else()
		set(ANGELSCRIPT_LIBRARY_NAME angelscript)
	endif()
endif()
find_package(OpenGL REQUIRED)
find_package(ReactPhysics3D REQUIRED)
find_package(LightLog REQUIRED)
find_package(glm REQUIRED)
find_package(jsoncpp REQUIRED)

set(3DEV_DEPS
    LightLog
    ReactPhysics3D::ReactPhysics3D
    OpenGL::GL
    GLEW
    glm::glm
    assimp::assimp
    Angelscript::${ANGELSCRIPT_LIBRARY_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
    jsoncpp_lib
)
