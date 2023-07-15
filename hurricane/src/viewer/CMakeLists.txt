# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<hurricane/src/viewer>" -*-

#              include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/utilities
                         ${HURRICANE_SOURCE_DIR}/src/configuration
                         ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                         ${HURRICANE_SOURCE_DIR}/src/viewer
                         ${QtX_INCLUDE_DIRS}
                         ${Boost_INCLUDE_DIRS}
                         ${Python_INCLUDE_DIRS}
                         ${CMAKE_INSTALL_PREFIX}/include
                       )

                   set( mocIncludes hurricane/viewer/HApplication.h
                                    hurricane/viewer/PaletteItem.h
                                    hurricane/viewer/PaletteNamedItem.h
                                    hurricane/viewer/PaletteLayerItem.h
                                    hurricane/viewer/PaletteExtensionGoItem.h
                                    hurricane/viewer/PaletteWidget.h
                                    hurricane/viewer/GraphicsWidget.h
                                    hurricane/viewer/ErrorWidget.h
                                    hurricane/viewer/ExceptionWidget.h
                                    hurricane/viewer/BreakpointWidget.h
                                    hurricane/viewer/GotoWidget.h
                                    hurricane/viewer/DynamicLabel.h
                                    hurricane/viewer/MousePositionWidget.h
                                    hurricane/viewer/SelectCommand.h
                                    hurricane/viewer/CellWidget.h
                                    hurricane/viewer/CellViewer.h
                                    hurricane/viewer/CellPrinter.h
                                    hurricane/viewer/CellImage.h
                                    hurricane/viewer/OpenBlobDialog.h
                                    hurricane/viewer/RecordModel.h
                                    hurricane/viewer/InspectorWidget.h
                                    hurricane/viewer/SelectionPopupModel.h
                                    hurricane/viewer/SelectionPopup.h
                                    hurricane/viewer/SelectionModel.h
                                    hurricane/viewer/SelectionWidget.h
                                    hurricane/viewer/NetlistModel.h
                                    hurricane/viewer/NetlistWidget.h
                                    hurricane/viewer/HierarchyModel.h
                                    hurricane/viewer/HierarchyWidget.h
                                    hurricane/viewer/DisplayFilterWidget.h
                                    hurricane/viewer/ControllerWidget.h
                                    hurricane/viewer/ScriptWidget.h
                                    hurricane/viewer/AboutWindow.h
                      )
                   set( includes    hurricane/viewer/ScreenUtilities.h
                                    hurricane/viewer/DisplayStyle.h
                                    hurricane/viewer/ColorScale.h
                                    hurricane/viewer/Graphics.h
                                    hurricane/viewer/Selector.h
                                    hurricane/viewer/Ruler.h
                                    hurricane/viewer/Command.h
                                    hurricane/viewer/AreaCommand.h
                                    hurricane/viewer/MoveCommand.h
                                    hurricane/viewer/ZoomCommand.h
                                    hurricane/viewer/RulerCommand.h
                                    hurricane/viewer/SelectCommand.h
                                    hurricane/viewer/HierarchyCommand.h
                                    hurricane/viewer/SelectorCriterion.h
                                    hurricane/viewer/CellWidgets.h
                                    hurricane/viewer/DesignBlob.h
                                    hurricane/viewer/JsonConfiguration.h
                      )
                   set( pyIncludes  hurricane/viewer/PyHSVr.h
                                    hurricane/viewer/PyDrawingStyle.h
                                    hurricane/viewer/PyDrawingGroup.h
                                    hurricane/viewer/PyDisplayStyle.h
                                    hurricane/viewer/PyHApplication.h
                                    hurricane/viewer/PyGraphics.h
                                    hurricane/viewer/PyCellViewer.h
                                    hurricane/viewer/PyErrorWidget.h
                                    hurricane/viewer/PyAboutWindow.h
                                    hurricane/viewer/Script.h
                      )
                   set( cpps        HApplication.cpp
                                    ScreenUtilities.cpp 
                                    DisplayStyle.cpp 
                                    ColorScale.cpp
                                    Graphics.cpp
                                    GraphicsWidget.cpp
                                    ErrorWidget.cpp
                                    ExceptionWidget.cpp
                                    BreakpointWidget.cpp
                                    GotoWidget.cpp
                                    PaletteItem.cpp
                                    PaletteNamedItem.cpp
                                    PaletteLayerItem.cpp
                                    PaletteExtensionGoItem.cpp
                                    PaletteWidget.cpp
                                    DynamicLabel.cpp
                                    MousePositionWidget.cpp
                                    Selector.cpp
                                    Command.cpp
                                    AreaCommand.cpp
                                    MoveCommand.cpp
                                    ZoomCommand.cpp
                                    RulerCommand.cpp
                                    SelectCommand.cpp
                                    HierarchyCommand.cpp
                                    SelectorCriterion.cpp
                                    CellWidget.cpp
                                    CellViewer.cpp
                                    CellPrinter.cpp
                                    CellImage.cpp
                                    OpenBlobDialog.cpp
                                    RecordModel.cpp
                                    InspectorWidget.cpp
                                    SelectionPopupModel.cpp
                                    SelectionPopup.cpp
                                    SelectionModel.cpp
                                    SelectionWidget.cpp
                                    Ruler.cpp
                                    NetInformations.cpp
                                    NetlistModel.cpp
                                    NetlistWidget.cpp
                                    HierarchyInformations.cpp
                                    HierarchyModel.cpp
                                    HierarchyWidget.cpp
                                    DisplayFilterWidget.cpp
                                    ControllerWidget.cpp
                                    ScriptWidget.cpp
                                    DesignBlob.cpp
                                    JsonConfiguration.cpp
                                    AboutWindow.cpp
                      )
                   set( pyCpps      PyHSVr.cpp
                                    PyDrawingStyle.cpp
                                    PyDrawingGroup.cpp
                                    PyDisplayStyle.cpp
                                    PyHApplication.cpp
                                    PyGraphics.cpp
                                    PyViewer.cpp
                                    PyCellViewer.cpp
                                    PyErrorWidget.cpp
                                    PyAboutWindow.cpp
                                    Script.cpp
                      )

          qtX_wrap_cpp( MOC_SRCS    ${mocIncludes} )
     qtX_add_resources( RCC_SRCS    CellViewer.qrc )

                   set( depLibs     hurricane
                                    isobar
                                    utils
                                    configuration
                                    ${UTILITIES_LIBRARY}
                                    ${LIBXML2_LIBRARIES}
                                    ${QtX_LIBRARIES}
				    
                      )

           add_library( viewer      ${cpps} ${MOC_SRCS} ${RCC_SRCS} ${pyCpps} )
 set_target_properties( viewer      PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( viewer      ${depLibs} )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Viewer
                        "viewer;${depLibs}"
                        include/coriolis2/hurricane/viewer
                      )

               install( FILES       ${includes}
                                    ${mocIncludes} DESTINATION include/coriolis2/hurricane/viewer ) 
               install( TARGETS     viewer         DESTINATION lib${LIB_SUFFIX} )
