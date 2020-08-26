file(GLOB_RECURSE src_files_utilities Utilities/*)

file(GLOB_RECURSE src_files_vulkan Vulkan/*)

set(src_files
	main.cpp
    App.cpp
    App.hpp
)

# Setup Groups
source_group("Utilities" FILES ${src_files_utilities})
source_group("Vulkan" FILES ${src_files_vulkan})
source_group("Main" FILES ${src_files})


include_directories(.)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${glfw3_INCLUDE_DIRS})
include_directories(${glm_INCLUDE_DIRS})
include_directories(${Vulkan_INCLUDE_DIRS})
link_directories(${Vulkan_LIBRARY})

add_executable(${exe_name} 
	${src_files_utilities} 
	${src_files_vulkan} 
	${src_files}
)

# Enable msvc warnings
if(MSVC)
	target_compile_options(${exe_name} PRIVATE /W3)
else()
	target_compile_options(${exe_name} PRIVATE -Wall -Wno-missing-braces -Wno-attributes -pedantic)
endif()

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${exe_name} PUBLIC ${LIBRARIES})
target_include_directories(${exe_name} PUBLIC ${INCLUDE_DIRS})
add_dependencies(${exe_name} Assets Shaders)