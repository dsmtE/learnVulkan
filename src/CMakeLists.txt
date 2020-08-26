file(GLOB_RECURSE src_files_utilities Utilities/*)

file(GLOB_RECURSE src_files_vulkan Vulkan/*)

set(src_files
	main.cpp
    TriangleApp.cpp
    TriangleApp.hpp
)

# Setup Groups
source_group("Main" FILES ${src_files})
source_group("Vulkan" FILES ${src_files_vulkan})
source_group("Utilities" FILES ${src_files_utilities})

include_directories(.)

add_executable(${exe_name} 
	${src_files}
	${src_files_vulkan}
	${src_files_utilities} 
)

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${exe_name} PUBLIC ${LIBRARIES})
target_include_directories(${exe_name} PUBLIC ${INCLUDE_DIRS})
add_dependencies(${exe_name} Assets Shaders)