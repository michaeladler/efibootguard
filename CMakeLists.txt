#
# EFI Boot Guard
#
# Copyright (c) Siemens AG, 2023
#
# Authors:
#  Michael Adler <michael.adler@siemens.com>
#  psy <patrice.salathe@code-intelligence.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#
# SPDX-License-Identifier:	GPL-2.0

cmake_minimum_required(VERSION 3.16)
project(efibootguard)

find_package(cifuzz)
enable_fuzz_testing()

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-O0 -g -fno-sanitize=undefined)
add_link_options(-fuse-ld=lld)

add_library(
    sut STATIC
    mocks/src/mocks.c
    env/env_api.c
    env/env_api_fat.c
    env/env_config_file.c
    env/env_config_partitions.c
    env/env_disk_utils.c
    env/uservars.c
    tools/bg_envtools.c
    tools/bg_printenv.c
    tools/bg_setenv.c
    tools/ebgpart.c
    tools/fat.c
)

target_include_directories(sut PUBLIC
    mocks/include
    tools
    include
)

add_fuzz_test(fuzztest "fuzztests/fuzztest.cpp")

target_link_libraries(fuzztest PRIVATE sut)
