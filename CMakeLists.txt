# ///////////////////////////////// SETTINGS //////////////////////////////////

cmake_minimum_required(VERSION 3.10)

project(learnVulkan
    VERSION 0.0.1
    LANGUAGES CXX C    # C for GLFW
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set output build directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

# ////////////////////////////// PROJECT FILES ////////////////////////////////

set(MAIN_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE SRC_FILES  ${CMAKE_SOURCE_DIR}/src/*.cpp )
LIST(REMOVE_ITEM SRC_FILES ${MAIN_FILE})

# /////////////////////////////// DEPENDENCIES ////////////////////////////////

set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

# ////////// Vulkan ////////// #
find_package(Vulkan REQUIRED)

# ////////// GLM ////////// #
set(GLM_LIB_NAME glm)
set(GLM_INC_PATH ${LIB_DIR}/glm)
add_library(${GLM_LIB_NAME} INTERFACE)
target_include_directories(${GLM_LIB_NAME} INTERFACE ${GLM_INC_PATH})

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
    ${GLM_INCLUDE_DIR}
    ${IMGUI_INC_PATH}
)

set(LIBRARIES
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB_NAME}
    ${GLM_LIB_NAME}
    ${IMGUI_LIB_NAME}
)

# /////////////////////////////////// BUILD ///////////////////////////////////

get_filename_component(FILE_NAME ${MAIN_FILE} NAME_WE)
add_executable(${FILE_NAME} ${MAIN_FILE} ${SRC_FILES})

# Enable warnings
if(MSVC)
	target_compile_options(${FILE_NAME} PRIVATE /W3)
else()
	target_compile_options(${FILE_NAME} PRIVATE -Wall -Wno-missing-braces -Wno-attributes -pedantic)
endif()

target_link_libraries(${FILE_NAME} PUBLIC ${LIBRARIES})
target_include_directories(${FILE_NAME} PUBLIC ${INCLUDE_DIRS})
