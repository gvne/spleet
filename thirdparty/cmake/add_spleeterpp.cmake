include(FetchContent)

FetchContent_Declare(spleeterpp
  GIT_REPOSITORY https://github.com/gvne/spleeterpp.git
  GIT_TAG 1495e1343b054b287e52fdcc03eca310650108a0
)

FetchContent_GetProperties(spleeterpp)
if(NOT spleeterpp_POPULATED)
  FetchContent_Populate(spleeterpp)
  set(spleeter_enable_filter OFF CACHE BOOL "")
  set(spleeter_enable_tests OFF CACHE BOOL "")
  add_subdirectory(${spleeterpp_SOURCE_DIR} ${spleeterpp_BINARY_DIR})
endif()
