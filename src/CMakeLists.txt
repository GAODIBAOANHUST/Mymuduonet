set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC_LIST)

# 生成库，动态库是SHARED,静态库是STATIC
add_library(mymuduo SHARED ${SRC_LIST})