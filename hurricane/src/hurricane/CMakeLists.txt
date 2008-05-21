

 include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane )
                 set ( includes hurricane/BasicLayer.h            hurricane/BasicLayers.h
                                hurricane/Boxes.h                 hurricane/Box.h
                                hurricane/Cell.h                  hurricane/Cells.h
                                hurricane/Collection.h
                                hurricane/Commons.h
                                hurricane/Component.h             hurricane/Components.h
                                hurricane/CompositeLayer.h        hurricane/CompositeLayers.h
                                hurricane/Contact.h               hurricane/Contacts.h
                                hurricane/DataBase.h
                                hurricane/DBo.h                   hurricane/DBos.h
                                hurricane/DeepNet.h
                                hurricane/DisplaySlot.h           hurricane/DisplaySlots.h
                                hurricane/DRCError.h
                                hurricane/Entities.h              hurricane/Entity.h
                                hurricane/Error.h
                                hurricane/Exception.h
                                hurricane/Filter.h
                                hurricane/Go.h                    hurricane/Gos.h
                                hurricane/Hook.h                  hurricane/Hooks.h
                                hurricane/Horizontal.h            hurricane/Horizontals.h
                                hurricane/HyperNet.h
                                hurricane/Instance.h              hurricane/Instances.h
                                hurricane/Interruption.h
                                hurricane/Interval.h              hurricane/Intervals.h
                                hurricane/IntrusiveMap.h
                                hurricane/IntrusiveSet.h
                                hurricane/Layer.h                 hurricane/Layers.h
                                hurricane/Libraries.h             hurricane/Library.h
                                hurricane/ListCollection.h
                                hurricane/Locator.h
                                hurricane/MapCollection.h
                                hurricane/Marker.h                hurricane/Markers.h
                                hurricane/MultisetCollection.h
                                hurricane/Name.h                  hurricane/Names.h
                                hurricane/NetExternalComponents.h
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
                                hurricane/Record.h
                                hurricane/Reference.h             hurricane/References.h
                                hurricane/Region.h
                                hurricane/Relation.h
                                hurricane/RoutingPad.h            hurricane/RoutingPads.h
                                hurricane/Rubber.h                hurricane/Rubbers.h
                                hurricane/Segment.h               hurricane/Segments.h
                                hurricane/Selectors.h
                                hurricane/SetCollection.h
                                hurricane/SharedName.h
                                hurricane/SharedPathes.h          hurricane/SharedPath.h
                                hurricane/Slice.h                 hurricane/Slices.h
                                hurricane/SlotAdapter.h
                                hurricane/Slot.h
                                hurricane/Symbols.h
                                hurricane/Tabulation.h
                                hurricane/Technology.h
                                hurricane/Timer.h
                                hurricane/Transformation.h
                                hurricane/Unit.h
                                hurricane/UpdateSession.h
                                hurricane/UserGo.h                hurricane/UserGos.h
                                hurricane/VectorCollection.h
                                hurricane/Vertical.h              hurricane/Verticals.h
                                hurricane/Views.h
                                hurricane/Warning.h
                     )
                 set ( cpps     BasicLayer.cpp
                                Box.cpp
                                CellCollections.cpp
                                Cell.cpp
                                Commons.cpp
                                Component.cpp
                                CompositeLayer.cpp
                                Contact.cpp
                                DataBase.cpp
                                DBo.cpp
                                DeepNet.cpp
                                DisplaySlot.cpp
                                DRCError.cpp
                                Entity.cpp
                                Error.cpp
                                Exception.cpp
                                Go.cpp
                                Hook.cpp
                                Horizontal.cpp
                                HyperNet.cpp
                                Instance.cpp
                                Interruption.cpp
                                Interval.cpp
                                Layer.cpp
                                Library.cpp
                                Marker.cpp
                                Name.cpp
                                Net.cpp
                                NetExternalComponents.cpp
                                Occurrence.cpp
                                Pad.cpp
                                Path.cpp
                                Pin.cpp
                                Plug.cpp
                                Point.cpp
                                Property.cpp
                                QuadTree.cpp
                                Quark.cpp
                                Record.cpp
                                Reference.cpp
                                Region.cpp
                                Relation.cpp
                                RoutingPad.cpp
                                Rubber.cpp
                                Segment.cpp
                                SharedName.cpp
                                SharedPath.cpp
                                Slice.cpp
                                SlotAdapter.cpp
                                Slot.cpp
                                Tabulation.cpp
                                Technology.cpp
                                Timer.cpp
                                Transformation.cpp
                                Unit.cpp
                                UpdateSession.cpp
                                UserGo.cpp
                                Vertical.cpp
                                Warning.cpp
                 )
    
     add_library ( hurricane SHARED ${cpps} )
    
         install ( FILES   ${includes} DESTINATION /include/hurricane) 
         install ( TARGETS hurricane   DESTINATION /lib)
