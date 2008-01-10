include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/src/hurricane)

set(includes InstanceFigure.h Utils.h)
set(cpps InstanceFigure.cpp CellScene.cpp)

add_library(hurricaneeditor SHARED ${cpps})
target_link_libraries(hurricaneeditor ${QT_LIBRARIES} hurricane)

install(FILES ${includes} DESTINATION /include/coriolis/hurricane) 
install(TARGETS hurricaneeditor DESTINATION /lib)
