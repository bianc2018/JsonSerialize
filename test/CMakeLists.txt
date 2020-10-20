# all cpp files
#file(GLOB_RECURSE CPP_SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE CPP_SRC_LIST ./*.cpp)
# generate target for every cpp file
foreach(FILE_PATH ${CPP_SRC_LIST})
    MESSAGE(${FILE_PATH})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE_PATH})
    add_executable(test_${FILE_NAME} ${FILE_PATH} )
    target_link_libraries(test_${FILE_NAME}  PRIVATE gtest_main cjson ${DEFAULT_LIB})
	add_test(JsonSerializeTest test_${FILE_NAME})
endforeach()