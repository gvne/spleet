include(FetchContent)

FetchContent_Declare(spleeterpp
  GIT_REPOSITORY https://github.com/gvne/spleeterpp.git
  GIT_TAG f2efdb486c8425c9f1e73e81ff42ba858a8bd6fe
)

FetchContent_GetProperties(spleeterpp)
if(NOT spleeterpp_POPULATED)
  FetchContent_Populate(spleeterpp)
  set(spleeter_enable_filter OFF CACHE BOOL "")
  set(spleeter_enable_tests OFF CACHE BOOL "")
  add_subdirectory(${spleeterpp_SOURCE_DIR} ${spleeterpp_BINARY_DIR})
endif()
