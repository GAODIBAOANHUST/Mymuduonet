# 控制总编译

cmake_minimum_required(VERSION 3.0)
project(mymuduo)

# # mymuduo最终编译成so动态库,设置动态库的路径,放在根目录的lib文件夹下
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# # 设置调试信息
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC")

# # 添加包含头文件的路径
# include_directories(${PROJECT_SOURCE_DIR}/include)

# # 定义参与编译的源代码文件
# aux_source_directory(src SRC_LIST)

# # 编译生成动态库mymuduo
# add_library(mymuduo SHARED ${SRC_LIST})

add_subdirectory(src)