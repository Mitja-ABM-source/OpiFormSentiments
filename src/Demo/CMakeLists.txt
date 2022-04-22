PROJECT(Demo)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/OpiformSentiments
)

ADD_EXECUTABLE(Demo
	main.cpp
)

TARGET_LINK_LIBRARIES(Demo
	OpiformSentiments
)

SET_PROPERTY(TARGET Demo PROPERTY FOLDER "Executable")