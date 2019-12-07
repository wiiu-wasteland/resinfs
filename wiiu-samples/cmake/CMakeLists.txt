cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/wut/share/wut.toolchain.cmake")

project(ramfs-helloworld C)
include("${DEVKITPRO}/wut/share/wut.cmake" REQUIRED)
include("${DEVKITPRO}/portlibs/wiiu/share/ramfs.cmake" REQUIRED)

# add source files
add_executable(ramfs-helloworld
	helloworld.c)

# create and add ramfs from "ramfs" directory
ramfs_add(ramfs-helloworld "ramfs")

# link requireed libraries
target_link_libraries(ramfs-helloworld
	whb
	coreinit
	proc_ui
	sysapp)

# build an rpx
wut_create_rpx(ramfs-helloworld.rpx ramfs-helloworld)
