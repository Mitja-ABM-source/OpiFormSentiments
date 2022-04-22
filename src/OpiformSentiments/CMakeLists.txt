FILE(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB_RECURSE hdrs ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

ADD_LIBRARY(OpiformSentiments ${srcs} ${hdrs})

FILE(GLOB_RECURSE files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_SOURCE_DIR}/OpiformSentiments/*.h" "${CMAKE_SOURCE_DIR}/OpiformSentiments/*.cpp"
)

foreach(f ${hdrs})
	file(RELATIVE_PATH fr ${CMAKE_CURRENT_SOURCE_DIR} ${f})
	get_filename_component(p ${fr} PATH)
	install(FILES ${f} DESTINATION ${PROJECT_INSTALL_INCLUDE_DIR}/${p})
endforeach()

INSTALL(TARGETS OpiformSentiments 
	RUNTIME DESTINATION lib
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

SET_PROPERTY(TARGET OpiformSentiments PROPERTY FOLDER "Library")

set(files ${srcs} ${hdrs})
organize_project_hierarchy(${files})