include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/src/hurricane)

set(includes CellEditor.h CellScene.h)
set(exports CellEditor.h)
set(cpps CellGraphicsItem.cpp InstanceGraphicsItem.cpp SegmentFigure.cpp
        CellScene.cpp CellEditor.cpp)

QT4_WRAP_CPP(MOC_SRCS ${includes})

add_library(hurricaneeditor SHARED ${cpps} ${MOC_SRCS})
target_link_libraries(hurricaneeditor ${QT_LIBRARIES} hurricane)

install(FILES ${exports} DESTINATION /include/hurricane) 
install(TARGETS hurricaneeditor DESTINATION /lib)
