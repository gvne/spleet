cmake_minimum_required(VERSION 3.14)

project(spleet-thirdparty)

# Thirdparty
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(add_spleeterpp)
include(add_cxxopts)
