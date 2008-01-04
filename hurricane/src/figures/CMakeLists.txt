include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/hurricane)

set(includes InstanceFigure.h Utils.h)
set(cpps InstanceFigure.cpp CellScene.cpp)

add_library(hurricanefigs SHARED ${cpps})
target_link_libraries(hurricanefigs ${QT_LIBRARIES} hurricane)

install(FILES ${includes} DESTINATION /include/hurricane) 
install(TARGETS hurricanefigs DESTINATION /lib)

