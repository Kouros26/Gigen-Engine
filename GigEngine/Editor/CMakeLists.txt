# set the project name
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(PROJECT_EDITOR ${CURRENT_FOLDER_NAME})
set(SOURCE_FOLDER "Sources")
set(INCLUDE_FOLDER "Headers")

###############################
#                             #
# Sources                     #
#                             #
###############################

# Add source files
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FOLDER})
file(GLOB_RECURSE SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/*.c
	${PROJECT_SOURCE_DIR}/*.cc
	${PROJECT_SOURCE_DIR}/*.cpp
	${PROJECT_SOURCE_DIR}/*.cxx
	${PROJECT_SOURCE_DIR}/*.c++)
	
# Add header files
set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_FOLDER})
file(GLOB_RECURSE HEADER_FILES 
	${PROJECT_INCLUDE_DIR}/*.h
	${PROJECT_INCLUDE_DIR}/*.hpp
	${PROJECT_INCLUDE_DIR}/*.inl)
	
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${SOURCE_FILES})

###############################
#                             #
# Library                  	  #
#                             #
###############################

add_library(${PROJECT_EDITOR} STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_EDITOR} PRIVATE ${INCLUDE_FOLDER})
target_include_directories(${PROJECT_EDITOR} PRIVATE ${DEPENDS_DIR})
target_include_directories(${PROJECT_EDITOR} PUBLIC
	${DEPENDS_DIR}/GLAD/include)
	
#add ImGUI
file(GLOB_RECURSE IMGUI_SOURCES ${DEPENDS_DIR}/IMGUI/*.cpp ${DEPENDS_DIR}/ImGUI/*.h)
add_library(ImGui STATIC EXCLUDE_FROM_ALL ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${DEPENDS_DIR}/IMGUI)
target_include_directories(ImGui PUBLIC ${GLFW3_INCLUDE_DIR})
target_include_directories(ImGui PUBLIC ${IMGUI_SOURCES}/GLAD/include)
target_include_directories(ImGui PUBLIC ${MATHS_DIR})
target_include_directories(ImGui PUBLIC ${ASSIMP_INCLUDE_DIR})
target_include_directories(ImGui PUBLIC ${BULLET_INCLUDE_DIR})
target_link_libraries(${PROJECT_EDITOR} PUBLIC ImGui)

foreach( project_i ${THIRD_PARTY_PROJECT})
	include_directories(${PROJECT_EDITOR} PRIVATE ${CMAKE_SOURCE_DIR}/${project_i}/${INCLUDE_FOLDER})
endforeach( project_i )

foreach( include_i ${THIRD_PARTY_INCLUDE})
	include_directories(${PROJECT_EDITOR} PRIVATE ${DEPENDS_DIR}/${include_i}/${INCLUDE_FOLDER})
endforeach( include_i )

set(PROJECT_EDITOR ${PROJECT_EDITOR} PARENT_SCOPE)