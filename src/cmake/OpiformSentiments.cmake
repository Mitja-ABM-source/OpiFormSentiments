#organize project hierarchy
macro(organize_project_hierarchy)
	foreach(f ${ARGN})
    #Directory of the source
    get_filename_component(PARENT_DIR "${f}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${f}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${f}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${f}")
endforeach()
endmacro()