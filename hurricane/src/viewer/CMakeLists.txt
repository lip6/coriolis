include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/src/hurricane)

set(includes CellWidget.h LayersWidget.h CellViewer.h)
set(exports CellViewer.h)
set(cpps CellWidget.cpp LayersWidget.cpp CellViewer.cpp)

QT4_WRAP_CPP(MOC_SRCS ${includes})


add_library(hurricaneviewer SHARED ${cpps} ${MOC_SRCS})
target_link_libraries(hurricaneviewer ${QT_LIBRARIES} hurricane)

install(FILES ${exports} DESTINATION /include/coriolis/hurricane) 
install(TARGETS hurricaneviewer DESTINATION /lib)
