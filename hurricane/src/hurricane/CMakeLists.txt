
 include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                       ${HURRICANE_SOURCE_DIR}/src/configuration3 
                       ${RapidJSON_INCLUDE_DIRS}
                       ${Boost_INCLUDE_DIRS}
                       ${BZIP2_INCLUDE_DIR}
                       ${CMAKE_INSTALL_PREFIX}/include
                     )
                 set ( includes hurricane/Mask.h
                                hurricane/Flags.h
                                hurricane/Initializer.h
                                hurricane/DebugSession.h
                                hurricane/Backtrace.h
                                hurricane/FileReadGzStream.h
                                hurricane/FileWriteGzStream.h
                                hurricane/JsonWriter.h
                                hurricane/JsonObject.h
                                hurricane/JsonReader.h
                                hurricane/Signature.h
                                hurricane/Observer.h
                                hurricane/BasicLayer.h            hurricane/BasicLayers.h
                                hurricane/RegularLayer.h          hurricane/RegularLayers.h
                                hurricane/ViaLayer.h              hurricane/ViaLayers.h
                                hurricane/ContactLayer.h
                                hurricane/DiffusionLayer.h
                                hurricane/TransistorLayer.h       hurricane/TransistorLayers.h
                                hurricane/Box.h                   hurricane/Boxes.h
                                hurricane/Torus.h
                                hurricane/Cell.h                  hurricane/Cells.h
                                hurricane/CellsSort.h
                                hurricane/Collection.h
                                hurricane/Commons.h
                                hurricane/Component.h             hurricane/Components.h
                                hurricane/Contact.h               hurricane/Contacts.h
                                hurricane/DataBase.h
                                hurricane/DBo.h                   hurricane/DBos.h
                                hurricane/DeepNet.h
                                hurricane/DRCError.h
                                hurricane/Entities.h              hurricane/Entity.h
                                hurricane/Bug.h
                                hurricane/Error.h
                                hurricane/Exception.h
                                hurricane/Breakpoint.h
                                hurricane/Filter.h
                                hurricane/Go.h                    hurricane/Gos.h
                                hurricane/ExtensionGo.h           hurricane/ExtensionGos.h
                                hurricane/Text.h
                                hurricane/Hook.h                  hurricane/Hooks.h
                                hurricane/Horizontal.h            hurricane/Horizontals.h
                                hurricane/HyperNet.h
                                hurricane/Instance.h              hurricane/Instances.h
                                hurricane/Interruption.h
                                hurricane/Interval.h              hurricane/Intervals.h
                                hurricane/IntrusiveMap.h
                                hurricane/IntrusiveSet.h
                                hurricane/RbTree.h
                                hurricane/IntervalTree.h
                                hurricane/Layer.h                 hurricane/Layers.h
                                hurricane/Libraries.h             hurricane/Library.h
                                hurricane/ListCollection.h
                                hurricane/Locator.h
                                hurricane/MapCollection.h
                                hurricane/MultimapCollection.h
                                hurricane/MultisetCollection.h
                                hurricane/Marker.h                hurricane/Markers.h
                                hurricane/Name.h                  hurricane/Names.h
                                hurricane/NetAlias.h
                                hurricane/NetExternalComponents.h
                                hurricane/NetRoutingProperty.h
                                hurricane/Net.h                   hurricane/Nets.h
                                hurricane/Occurrence.h            hurricane/Occurrences.h
                                hurricane/Pad.h                   hurricane/Pads.h
                                hurricane/Pathes.h                hurricane/Path.h
                                hurricane/Pin.h                   hurricane/Pins.h
                                hurricane/Plug.h                  hurricane/Plugs.h
                                hurricane/Point.h                 hurricane/Points.h
                                hurricane/Primitives.h
                                hurricane/Properties.h            hurricane/Property.h
                                hurricane/QuadTree.h
                                hurricane/Quark.h                 hurricane/Quarks.h
                                hurricane/Query.h
                                hurricane/Record.h
                                hurricane/Reference.h             hurricane/References.h
                                hurricane/Region.h
                                hurricane/Relation.h
                                hurricane/RoutingPad.h            hurricane/RoutingPads.h
                                hurricane/Diagonal.h
                                hurricane/Rectilinear.h
                                hurricane/Rubber.h                hurricane/Rubbers.h
                                hurricane/Segment.h               hurricane/Segments.h
                                hurricane/Selectors.h
                                hurricane/SetCollection.h
                                hurricane/SharedName.h
                                hurricane/SharedPathes.h          hurricane/SharedPath.h
                                hurricane/Slice.h                 hurricane/Slices.h
                                hurricane/ExtensionSlice.h        hurricane/ExtensionSlices.h
                                hurricane/Slot.h
                                hurricane/Symbols.h
                                hurricane/Tabulation.h
                                hurricane/Technology.h
                                hurricane/Timer.h
                                hurricane/Transformation.h
                                hurricane/Polygon.h               hurricane/Polygons.h
                                hurricane/DbU.h
                                hurricane/UpdateSession.h
                                hurricane/VectorCollection.h
                                hurricane/Vertical.h              hurricane/Verticals.h
                                hurricane/Views.h
                                hurricane/Warning.h
                                hurricane/TextTranslator.h
                                hurricane/DeviceDescriptor.h
                                hurricane/ModelDescriptor.h
                                hurricane/Rule.h
                                hurricane/UnitRule.h
                                hurricane/PhysicalRule.h
                                hurricane/TwoLayersPhysicalRule.h
                     )
                 set ( cpps     Record.cpp
                                Slot.cpp
                                Initializer.cpp
                                Commons.cpp
                                Flags.cpp
                                Backtrace.cpp
                                FileReadGzStream.cpp
                                FileWriteGzStream.cpp
                                JsonWriter.cpp
                                JsonObject.cpp
                                JsonReader.cpp
                                Signature.cpp
                                Exception.cpp
                                Bug.cpp
                                Error.cpp
                                Breakpoint.cpp
                                DRCError.cpp
                                Warning.cpp
                                Interruption.cpp
                                Tabulation.cpp
                                DebugSession.cpp
                                Observer.cpp
                                DbU.cpp
                                Point.cpp
                                Box.cpp
                                Torus.cpp
                                Interval.cpp
                                Transformation.cpp
                                Name.cpp
                                DBo.cpp
                                DataBase.cpp
                                Technology.cpp
                                Layer.cpp
                                BasicLayer.cpp
                                RegularLayer.cpp
                                ViaLayer.cpp
                                ContactLayer.cpp
                                DiffusionLayer.cpp
                                TransistorLayer.cpp
                                Library.cpp
                                Entity.cpp
                                Cell.cpp
                                CellCollections.cpp
                                CellsSort.cpp
                                NetAlias.cpp
                                Net.cpp
                                DeepNet.cpp
                                HyperNet.cpp
                                Go.cpp
                                Text.cpp
                                ExtensionGo.cpp
                                Hook.cpp
                                Instance.cpp
                                Component.cpp
                                Plug.cpp
                                Contact.cpp
                                Pin.cpp
                                Segment.cpp
                                Vertical.cpp
                                Horizontal.cpp
                                Pad.cpp
                                RoutingPad.cpp
                                Diagonal.cpp
                                Rectilinear.cpp
                                Polygon.cpp
                                NetExternalComponents.cpp
                                NetRoutingProperty.cpp
                                Reference.cpp
                                Rubber.cpp
                                Quark.cpp
                                Property.cpp
                                Relation.cpp
                                SharedName.cpp
                                SharedPath.cpp
                                Path.cpp
                                Occurrence.cpp
                                Occurrences.cpp
                                QuadTree.cpp
                                Slice.cpp
                                ExtensionSlice.cpp
                                UpdateSession.cpp
                                Region.cpp
                                Query.cpp
                                Marker.cpp
                                Timer.cpp
                                TextTranslator.cpp
                                DeviceDescriptor.cpp
                                Rule.cpp
                                UnitRule.cpp
                                PhysicalRule.cpp
                                TwoLayersPhysicalRule.cpp
                 )
    
           add_library ( hurricane ${cpps} )
 target_link_libraries ( hurricane ${Boost_LIBRARIES} ${BZIP2_LIBRARIES} ${LIBBFD_LIBRARIES} )
 set_target_properties ( hurricane PROPERTIES VERSION 1.0 SOVERSION 1 )
               install ( TARGETS hurricane DESTINATION lib${LIB_SUFFIX} )
               install ( FILES ${includes} DESTINATION include/coriolis2/hurricane ) 
