file(GLOB font_files fonts/*.ttf)
file(GLOB model_files models/*.obj models/*.mtl)
file(GLOB texture_files textures/*.jpg textures/*.png textures/*.txt)

macro(copy_assets asset_files dir_name copied_files)
	foreach(asset ${${asset_files}})
		#message("asset: ${asset}")
		get_filename_component(file_name ${asset} NAME)
		get_filename_component(full_path ${asset} ABSOLUTE)
		set(output_dir ${CMAKE_CURRENT_BINARY_DIR}/${dir_name})
		set(output_file ${output_dir}/${file_name})
		set(${copied_files} ${${copied_files}} ${output_file})
		set(${copied_files} ${${copied_files}} PARENT_SCOPE)
		set_source_files_properties(${asset} PROPERTIES HEADER_FILE_ONLY TRUE)
		if (WIN32)
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND xcopy \"${full_path}\" \"${output_file}*\" /Y /Q /F
				DEPENDS ${full_path}
			)
		else()
			add_custom_command(
				OUTPUT ${output_file}
				COMMAND mkdir --parents ${output_dir} && cp --force --link \"${full_path}\" \"${output_file}\"
				DEPENDS ${full_path}
			)
		endif()
	endforeach()
endmacro()

copy_assets(font_files fonts copied_fonts)
copy_assets(model_files models copied_models)
copy_assets(texture_files textures copied_textures)

source_group("Fonts" FILES ${font_files})
source_group("Textures" FILES ${texture_files})

add_custom_target(
	Assets 
	DEPENDS ${copied_fonts} ${copied_models} ${copied_textures} 
	SOURCES ${font_files} ${model_files} ${texture_files})