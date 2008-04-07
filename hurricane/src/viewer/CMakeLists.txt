include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/src/hurricane)

set(includes ScreenUtilities.h DisplayStyle.h)
set(mocincludes ScreenLayer.h LayersList.h CellWidget.h CellViewer.h)
set(exports CellViewer.h)
set(cpps ScreenUtilities.cpp DisplayStyle.cpp ScreenLayer.cpp LayersList.cpp CellWidget.cpp CellViewer.cpp)

QT4_WRAP_CPP(MOC_SRCS ${mocincludes})
QT4_ADD_RESOURCES(RCC_SRCS CellViewer.qrc)

add_library(hurricaneviewer SHARED ${cpps} ${MOC_SRCS} ${RCC_SRCS})
target_link_libraries(hurricaneviewer ${QT_LIBRARIES} hurricane)

install(FILES ${exports} DESTINATION /include/hurricane) 
install(TARGETS hurricaneviewer DESTINATION /lib)
