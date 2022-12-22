add_subdirectory(ccore)
add_subdirectory(LibraryManager)
add_subdirectory(pyCRL)

if(CMAKE_BUILD_TYPE EQUAL "Release")
  add_subdirectory(x2y)
  add_subdirectory(cyclop)
endif()

