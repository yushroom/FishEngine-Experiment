cmake_minimum_required(VERSION 3.9)
project(FishEngine_Python)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


# glfw
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
ADD_SUBDIRECTORY(./ThirdParty/glfw/)
include_directories(./ThirdParty/glfw/include)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "ThirdParty")

# glew
if (MSVC)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251" )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/glew/include)
    #set(BUILD_UTILS OFF CACHE BOOL "" FORCE)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/ThirdParty/glew/build/cmake)

    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    # set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "ThirdParty/glew")
    SET_TARGET_PROPERTIES(glew_s PROPERTIES FOLDER "ThirdParty/glew")
endif()


# Autodesk FBX sdk
# SET( FBXSDK_DIR "" CACHE PATH "Autodesk FBX SDK root directory" )
if (WIN32)
    SET( FBXSDK_DIR "D:\\library\\fbxsdk\\2018.1.1")
else()
    SET( FBXSDK_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/FBX SDK/2018.1.1")
endif()
# SET( FBXSDK_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/FBX SDK/2018.1.1")
include_directories( ${FBXSDK_DIR}/include )
IF (WIN32)
    SET( FBXSDK_LIB_DIR ${FBXSDK_DIR}/lib/vs2015/x64 )
    SET( FBXSDK_LIB_DEBUG ${FBXSDK_LIB_DIR}/debug/libfbxsdk-md.lib )
    SET( FBXSDK_LIB_RELEASE ${FBXSDK_LIB_DIR}/release/libfbxsdk-md.lib )
    SET( FBXSDK_LIB debug ${FBXSDK_LIB_DEBUG} optimized ${FBXSDK_LIB_RELEASE} )
ELSE()
    SET( FBXSDK_LIB_DIR ${FBXSDK_DIR}/lib/clang )
    # SET( FBXSDK_LIB_DEBUG ${FBXSDK_LIB_DIR}/debug/libfbxsdk.a )
    SET( FBXSDK_LIB_RELEASE ${FBXSDK_LIB_DIR}/release/libfbxsdk.a )
    SET( FBXSDK_LIB ${FBXSDK_LIB_RELEASE})
ENDIF()


# nVidia PhysX
if (MSVC)
    SET( PhysX_ROOT_DIR "D:\\library\\PhysX-3.4-master\\PhysX_3.4" )
else()
    SET( PhysX_ROOT_DIR "/Users/yushroom/program/library/PhysX-3.4-3.4.1/PhysX_3.4")
endif()
include_directories( ${PhysX_ROOT_DIR}/Include )
include_directories( ${PhysX_ROOT_DIR}/../PxShared/include )
include_directories( ${PhysX_ROOT_DIR}/Snippets )
if (MSVC)
    FILE(GLOB PhysXSDK_LIBRARIES ${PhysX_ROOT_DIR}/Lib/vc15win64/*DEBUG*lib ${PhysX_ROOT_DIR}/../PxShared/lib/vc15win64/*DEBUG*lib)
else()
    FILE(GLOB PhysXSDK_LIBRARIES ${PhysX_ROOT_DIR}/Lib/osx64/*.a ${PhysX_ROOT_DIR}/../PxShared/lib/osx64/*.a)
endif()
# message("PhysXSDK_LIBRARIES" ${PhysXSDK_LIBRARIES})


set(NANOVG_DIR "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/nanovg/nanovg")
include_directories(${NANOVG_DIR}/../)
file(GLOB_RECURSE NANOVG_HEADERS ${NANOVG_DIR}/*.h)
add_library(nanovg ${NANOVG_DIR}/nanovg.c ${NANOVG_HEADERS})
set_target_properties(nanovg PROPERTIES FOLDER "ThirdParty")

macro(AutoGroup root_dir)
	foreach(group ${ARGN})
		file(GLOB_RECURSE files ${root_dir}/${group}/*.hpp ${root_dir}/${group}/*.inl ${root_dir}/${group}/*.cpp)
		source_group(${group} FILES ${files})
	endforeach(group)
endmacro()

include_directories(${CMAKE_CURRENT_LIST_DIR}/Include/)


# python3
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})


if (APPLE)
	find_package(Boost REQUIRED COMPONENTS filesystem)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

	find_package(folly REQUIRED)
	include_directories(${FOLLY_INCLUDE_DIR})
endif()

add_subdirectory(./ThirdParty/pybind11)
include_directories(./ThirdParty/pybind11/include)


# glm
include_directories(./ThirdParty/glm)
#FishGUI
SET(FishGUI_DIR ../FishGUI)
SET(FishGUI_INCLUDE_DIR ${FishGUI_DIR}/include)
SET(FishGUI_SRC_DIR ${FishGUI_DIR}/src)
# aux_source_directory(${FishGUI_SRC_DIR} SRCS)
file(GLOB_RECURSE SRCS ${FishGUI_SRC_DIR}/*.cpp)
FILE(GLOB_RECURSE HEADERS ${FishGUI_DIR}/include/FishGUI/*.hpp)
include_directories( ${FishGUI_INCLUDE_DIR} )
set(FishGUI_EXTENSION_DIR ${FishGUI_DIR}/examples/common)
include_directories(${FishGUI_EXTENSION_DIR})
FILE(GLOB_RECURSE EXTENSIONS ${FishGUI_EXTENSION_DIR}/*.hpp ${FishGUI_EXTENSION_DIR}/*.cpp)
source_group("extensions" FILES ${EXTENSIONS})
add_library(FishGUI ${SRCS} ${HEADERS} ${EXTENSIONS})

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/Include/FishEngine/*.hpp ${CMAKE_CURRENT_LIST_DIR}/Include/FishEngine/*.inl)
file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/Source/FishEngine/*.cpp)
add_library(FishEngine ${HEADERS} ${SRCS})

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/Include/FishEditor/*.hpp ${CMAKE_CURRENT_LIST_DIR}/Include/FishEditor/*.inl ${CMAKE_CURRENT_LIST_DIR}/Source/FishEditor/*.hpp)
file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/Source/FishEditor/*.cpp)
add_library(FishEditor ${HEADERS} ${SRCS})

add_executable(Test test.cpp)
# add_executable(Test MACOSX_BUNDLE test.cpp)
target_link_libraries(Test FishEngine)
target_link_libraries(Test FishEditor)
# target_link_libraries(Test FishGame)
target_link_libraries(Test nanovg)
target_link_libraries(Test glfw)
target_link_libraries(Test ${Boost_LIBRARIES})
# target_link_libraries(Test ${PYTHON_LIBRARY})
target_link_libraries(Test ${FBXSDK_LIB})
target_link_libraries(Test pybind11::embed)
target_link_libraries(Test ${FOLLY_LIBRARIES})
target_link_libraries(Test ${PhysXSDK_LIBRARIES})
if (MSVC)
	target_link_libraries(Test glew_s)
	# target_link_libraries(Test opengl32.lib)
else()
	set_target_properties(Test PROPERTIES LINK_FLAGS "-framework OpenGL")

	SET( INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/MacOSXBundleInfo.plist.in)
	IF (NOT EXISTS ${INFO_PLIST})
		MESSAGE(FATAL_ERROR "${INFO_PLIST} not found!")
	ENDIF()
	set_target_properties(Test PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${INFO_PLIST}" )
endif()

file(GLOB_RECURSE SRCS ./Source/Editor/*.cpp ./Source/Editor/*.hpp)
add_executable(Editor ${SRCS})
target_link_libraries(Editor FishEngine)
target_link_libraries(Editor FishEditor)
target_link_libraries(Editor FishGUI)
target_link_libraries(Editor nanovg)
target_link_libraries(Editor glfw)
target_link_libraries(Editor ${FBXSDK_LIB})
target_link_libraries(Editor pybind11::embed)
target_link_libraries(Editor ${FOLLY_LIBRARIES})
target_link_libraries(Editor ${Boost_LIBRARIES})
target_link_libraries(Editor ${PhysXSDK_LIBRARIES})
if (MSVC)
	target_link_libraries(Editor glew_s)
else()
	set_target_properties(Editor PROPERTIES LINK_FLAGS "-framework OpenGL")

	SET( INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/MacOSXBundleInfo.plist.in)
	IF (NOT EXISTS ${INFO_PLIST})
		MESSAGE(FATAL_ERROR "${INFO_PLIST} not found!")
	ENDIF()
	set_target_properties(Editor PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${INFO_PLIST}" )
endif()


AutoGroup(${CMAKE_CURRENT_LIST_DIR}/Include/FishEngine Util Math Render Internal Component UI System Physics)
AutoGroup(${CMAKE_CURRENT_LIST_DIR}/Source/FishEngine Math Render Component UI System Physics)