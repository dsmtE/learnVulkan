cmake_minimum_required(VERSION 3.10)

project(learnVulkan
    VERSION 0.0.1
    LANGUAGES CXX C    # C for GLFW
)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to RelWithDebInfo if not specified
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Specifies the build type on single-configuration generators" FORCE)
endif ()

if (WIN32)
    # Reduce the Win32 header files size
	add_definitions(-DWIN32_LEAN_AND_MEAN)

    add_compile_options("/W3")
	# Enabling Multi-Processor compilation
	add_compile_options("/MP")
	
elseif (UNIX)
	
	add_definitions(-DUNIX)
	add_compile_options("-Wall")
    add_compile_options("-pedantic")
	add_compile_options("-fvisibility=hidden")

endif ()

# Set output build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Remove CONFIGURATION_TYPES sub directories
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR}/lib)
endforeach()

# /////////////////////////////// DEPENDENCIES ////////////////////////////////

set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

# ////////// Vulkan ////////// #
find_package(Vulkan REQUIRED)

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else ()
    message(STATUS ${Vulkan_LIBRARY})
endif ()

find_program(Vulkan_GLSLANG_VALIDATOR 
	NAMES glslangValidator 
	HINTS ENV VULKAN_SDK 
	PATH_SUFFIXES bin)
	
if (NOT Vulkan_GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found!")
endif()

# ////////// GLM ////////// #
set(GLM_LIB_NAME glm)
set(GLM_INC_PATH ${LIB_DIR}/glm)
add_library(${GLM_LIB_NAME} INTERFACE)
target_include_directories(${GLM_LIB_NAME} INTERFACE ${GLM_INC_PATH})

# ////////// stb_image ////////// #
set(STB_IMG_LIB_NAME stb_image)
set(STB_IMG_INC_PATH ${LIB_DIR}/stb)
add_library(${STB_IMG_LIB_NAME} INTERFACE)
target_include_directories(${STB_IMG_LIB_NAME} INTERFACE ${STB_IMG_INC_PATH})

# ////////// GLFW ////////// #
set(GLFW_LIB_NAME glfw)
set(GLFW_INC_PATH ${LIB_DIR}/${GLFW_LIB_NAME}/include)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(GLFW_VULKAN_STATIC  OFF CACHE BOOL "" FORCE)    # OFF by default
#set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)    # OFF by default
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${LIB_DIR}/${GLFW_LIB_NAME})

# ////////// IMGUI ////////// #
set(IMGUI_LIB_NAME imgui)
set(IMGUI_INC_PATH ${LIB_DIR}/imgui)

set(IMGUI_SOURCES
    ${LIB_DIR}/imgui/imgui.cpp
    ${LIB_DIR}/imgui/imgui_demo.cpp
    ${LIB_DIR}/imgui/imgui_draw.cpp
    ${LIB_DIR}/imgui/imgui_widgets.cpp
    ${LIB_DIR}/imgui/examples/imgui_impl_glfw.cpp
    ${LIB_DIR}/imgui/examples/imgui_impl_vulkan.cpp
)

set(IMGUI_HEADERS
    ${LIB_DIR}/imgui/imconfig.h
    ${LIB_DIR}/imgui/imgui.h
    ${LIB_DIR}/imgui/imgui_internal.h
    ${LIB_DIR}/imgui/imstb_rectpack.h
    ${LIB_DIR}/imgui/imstb_textedit.h
    ${LIB_DIR}/imgui/imstb_truetype.h
    ${LIB_DIR}/imgui/examples/imgui_impl_glfw.h
    ${LIB_DIR}/imgui/examples/imgui_impl_vulkan.h
)

add_library(${IMGUI_LIB_NAME} STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(${IMGUI_LIB_NAME} PUBLIC ${IMGUI_INC_PATH} ${GLFW_INC_PATH} ${Vulkan_INCLUDE_DIRS})

# ///////////////////////////////// INCLUDES & LIBRARIES //////////////////////////////////

set(INCLUDE_DIRS
    ${Vulkan_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIR}
    ${STB_IMG_INCLUDE_DIR}
    ${IMGUI_INC_PATH}
)

set(LIBRARIES
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB_NAME}
    ${GLM_LIB_NAME}
    ${STB_IMG_LIB_NAME}
    ${IMGUI_LIB_NAME}
)

set(exe_name "learnVulkan")
add_subdirectory(assets)
add_subdirectory(src)
