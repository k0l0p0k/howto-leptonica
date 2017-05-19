CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(testbed)

INCLUDE_DIRECTORIES(. /usr/include/leptonica 
	${CMAKE_SOURCE_DIR}/../tess/tesseract/ccstruct/
	${CMAKE_SOURCE_DIR}/../tess/tesseract/ccutil/
	${CMAKE_SOURCE_DIR}/../tess/tesseract/viewer/
	              /usr/local/include/tesseract)
AUX_SOURCE_DIRECTORY(. SRC_LIST)

FIND_LIBRARY(LIBTESS libtesseract.so /usr/local/lib)
FIND_LIBRARY(LIBLEPT liblept.so /usr/local/lib)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})


TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${LIBTESS} ${LIBLEPT})








