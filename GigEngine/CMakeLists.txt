cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(CMAKE_FOLDER_OLD ${CMAKE_FOLDER})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set( THIRD_PARTY_PROJECT
   Editor
   Engine
)


include("FetchContent")
FetchContent_Declare(
	GLFW
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.8)
	
set(CMAKE_FOLDER "${CMAKE_FOLDER_OLD}/vendor/glfw")
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(GLFW)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v5.2.5)
	
	set(CMAKE_FOLDER "${CMAKE_FOLDER_OLD}/vendor/assimp")
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
	set(ASSIMP_BUILD_TESTS OFF)
FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
    LibMath
    GIT_REPOSITORY https://github.com/Angel-2180/LibMaths.git)

set(CMAKE_FOLDER "${CMAKE_FOLDER_OLD}/vendor/lm")
FetchContent_MakeAvailable(LibMath)


set(CMAKE_FOLDER ${CMAKE_FOLDER_OLD})

set( THIRD_PARTY_INCLUDE
${glfw_SOURCE_DIR}
${assimp_SOURCE_DIR}
)

include_directories(${MAIN_PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
include_directories(${MAIN_PROJECT_NAME} PRIVATE ${DEPENDS_DIR}/Assimp/include)
include_directories(${MAIN_PROJECT_NAME} PRIVATE ${LIBMATHS_INCLUDE_DIR})


foreach( project_i ${THIRD_PARTY_PROJECT})
add_subdirectory(${project_i})
include_directories(${MAIN_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${project_i}/${INCLUDE_FOLDER})
endforeach( project_i )

foreach( include_i ${THIRD_PARTY_INCLUDE})
include_directories(${MAIN_PROJECT_NAME} PRIVATE ${DEPENDS_DIR}/${include_i}/${INCLUDE_FOLDER})
endforeach( include_i )



set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_PROJECT_NAME}/${SOURCE_FOLDER})
file(GLOB_RECURSE SOURCE_FILES 
${PROJECT_SOURCE_DIR}/*.c
${PROJECT_SOURCE_DIR}/*.cc
${PROJECT_SOURCE_DIR}/*.cpp
${PROJECT_SOURCE_DIR}/*.cxx
${PROJECT_SOURCE_DIR}/*.c++)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_PROJECT_NAME}/${INCLUDE_FOLDER})
file(GLOB_RECURSE HEADER_FILES 
${PROJECT_INCLUDE_DIR}/*.h
${PROJECT_INCLUDE_DIR}/*.hpp
${PROJECT_INCLUDE_DIR}/*.inl)

add_executable(${MAIN_PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )

target_include_directories(${MAIN_PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR})
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE ${THIRD_PARTY_PROJECT})
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE ${LIBMATHS_LIBRARY})
target_include_directories(${PROJECT_EDITOR} PRIVATE ${ENGINE_INCLUDE_FOLDER})

set_target_properties(${MAIN_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration))
target_compile_options(${MAIN_PROJECT_NAME} PRIVATE /W4 /WX)
#Copy resources Folder
add_custom_command(TARGET ${MAIN_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
		${RESOURCES_DIR}
        $<TARGET_FILE_DIR:${MAIN_PROJECT_NAME}>/Resources)
		

#look for shader
file(GLOB_RECURSE TARGET_SHADER_FILES #any name for extentions --> choice 
	${RESOURCES_DIR}/*.vert
	${RESOURCES_DIR}/*.frag
	${RESOURCES_DIR}/*.geom
	${RESOURCES_DIR}/*.tesc
	${RESOURCES_DIR}/*.tese
	${RESOURCES_DIR}/*.comp
	${RESOURCES_DIR}/*.glsl)

#Add raw file to main with filter
source_group(Shaders FILES ${TARGET_SHADER_FILES})
target_sources(${MAIN_PROJECT_NAME} PRIVATE ${TARGET_SHADER_FILES})