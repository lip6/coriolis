include(${QT_USE_FILE})

include_directories(${HURRICANE_SOURCE_DIR}/src/hurricane)

set(mocincludes RecordModel.h HInspectorWidget.h)
set(cpps RecordModel.cpp HInspectorWidget.cpp)

QT4_WRAP_CPP(MOC_SRCS ${mocincludes})

add_library(hinspector SHARED ${cpps} ${MOC_SRCS})
target_link_libraries(hinspector ${QT_LIBRARIES} hurricane)

install(FILES HInspectorWidget.h DESTINATION /include/hurricane) 
install(TARGETS hinspector DESTINATION /lib)
