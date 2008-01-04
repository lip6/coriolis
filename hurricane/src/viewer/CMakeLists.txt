include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/hurricane)

set(includes CellWidget.h)
set(cpps CellWidget.cpp)

add_library(hurricaneviewer SHARED ${cpps})
target_link_libraries(hurricaneviewer ${QT_LIBRARIES} hurricane)

install(FILES ${includes} DESTINATION /include/hurricane) 
install(TARGETS hurricaneviewer DESTINATION /lib)
