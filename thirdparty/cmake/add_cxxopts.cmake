include(FetchContent)

FetchContent_Declare(cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v2.2.0
)

FetchContent_GetProperties(cxxopts)
if(NOT cxxopts_POPULATED)
  FetchContent_Populate(cxxopts)
  set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "")
  set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "")
  add_subdirectory(${cxxopts_SOURCE_DIR} ${cxxopts_BINARY_DIR})
endif()
