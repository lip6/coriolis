
               include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                         ${HURRICANE_SOURCE_DIR}/src/viewer
                         ${CONFIGURATION_INCLUDE_DIR}
                         ${Boost_INCLUDE_DIRS}
                         ${PYTHON_INCLUDE_PATH}
                       )

                   set ( mocincludes hurricane/viewer/HApplication.h
                                     hurricane/viewer/PaletteItem.h
                                     hurricane/viewer/PaletteNamedItem.h
                                     hurricane/viewer/PaletteLayerItem.h
                                     hurricane/viewer/PaletteExtensionGoItem.h
                                     hurricane/viewer/PaletteWidget.h
                                     hurricane/viewer/GraphicsWidget.h
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
                                     hurricane/viewer/RecordModel.h
                                     hurricane/viewer/InspectorWidget.h
                                     hurricane/viewer/SelectionPopupModel.h
                                     hurricane/viewer/SelectionPopup.h
                                     hurricane/viewer/SelectionModel.h
                                     hurricane/viewer/SelectionWidget.h
                                     hurricane/viewer/NetlistModel.h
                                     hurricane/viewer/NetlistWidget.h
                                     hurricane/viewer/DisplayFilterWidget.h
                                     hurricane/viewer/ControllerWidget.h
                                     hurricane/viewer/ScriptWidget.h
                                     hurricane/viewer/StratusWidget.h
                       )
                   set ( includes    hurricane/viewer/ScreenUtilities.h
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
                                     hurricane/viewer/StratusScript.h
                       )
                   set ( pyincludes  hurricane/viewer/PyHSVr.h
                                     hurricane/viewer/PyDrawingStyle.h
                                     hurricane/viewer/PyDrawingGroup.h
                                     hurricane/viewer/PyDisplayStyle.h
                                     hurricane/viewer/PyHApplication.h
                                     hurricane/viewer/PyGraphics.h
                                     hurricane/viewer/PyCellViewer.h
                       )
                   set ( cpps        HApplication.cpp
                                     ScreenUtilities.cpp 
                                     DisplayStyle.cpp 
                                     ColorScale.cpp
                                     Graphics.cpp
                                     GraphicsWidget.cpp
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
                                     DisplayFilterWidget.cpp
                                     ControllerWidget.cpp
                                     ScriptWidget.cpp
                                     StratusScript.cpp
                                     StratusWidget.cpp
                       )
                   set ( pycpps      PyHSVr.cpp
                                     PyDrawingStyle.cpp
                                     PyDrawingGroup.cpp
                                     PyDisplayStyle.cpp
                                     PyHApplication.cpp
                                     PyGraphics.cpp
                                     PyViewer.cpp
                                     PyCellViewer.cpp
                       )
# source2 & include2 for module that are *not* Python wrappers but true
# Hurricane modules.
                   set ( sources2   Script.cpp )
                   set ( includes2  hurricane/viewer/Script.h )

          qt4_wrap_cpp ( MOC_SRCS    ${mocincludes} )
     qt4_add_resources ( RCC_SRCS    CellViewer.qrc )

           add_library ( viewer      ${cpps} ${MOC_SRCS} ${RCC_SRCS} ${sources2} ${pycpps} )
 set_target_properties ( viewer      PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries ( viewer      hurricane isobar ${CONFIGURATION_LIBRARY} ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${QT_LIBRARIES} )
           add_library ( pyViewer    MODULE ${pycpps} )
 target_link_libraries ( pyViewer    viewer )
 set_target_properties ( pyViewer    PROPERTIES
                                     COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                     OUTPUT_NAME   "Viewer"
                                     PREFIX        ""
                       )


               install ( FILES       ${includes}
                                     ${includes2}
                                     ${mocincludes}
                                     ${pyincludes}  DESTINATION include/coriolis2/hurricane/viewer ) 
               install ( TARGETS     viewer         DESTINATION lib${LIB_SUFFIX} )
               install ( TARGETS     pyViewer       DESTINATION ${PYTHON_SITE_PACKAGES} )
