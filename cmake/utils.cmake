
function(target_link_libraries_and_dll target lib)
	target_link_libraries(${target} ${lib})
	add_custom_command(
	    TARGET ${target} POST_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy_if_different
	        $<TARGET_FILE:${lib}>
	        $<TARGET_FILE_DIR:${target}>
	)
endfunction()
