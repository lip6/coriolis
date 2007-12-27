include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/hurricane)

set(includes CellFigure.h InstanceFigure.h SliceFigure.h GoFigure.h SegmentFigure.h)
set(cpps CellFigure.cpp InstanceFigure.cpp SliceFigure.cpp GoFigure.cpp SegmentFigure.cpp)

add_library(hurricanefigs SHARED ${cpps})
target_link_libraries(hurricanefigs ${QT_LIBRARIES} hurricane)

install(FILES ${includes} DESTINATION /include/hurricane) 
install(TARGETS hurricanefigs DESTINATION /lib)
