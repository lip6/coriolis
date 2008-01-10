set(includes BasicLayer.h BasicLayers.h Boxes.h Box.h Cell.h
Cells.h Collection.h Commons.h Component.h Components.h CompositeLayer.h
CompositeLayers.h Contact.h Contacts.h DataBase.h DBo.h DBos.h
DeepNet.h DisplaySlot.h DisplaySlots.h DRCError.h Entities.h
Entity.h Error.h Exception.h Filter.h Go.h Gos.h
Hook.h Hooks.h Horizontal.h Horizontals.h 
HyperNet.h Instance.h Instances.h Interruption.h Interval.h
Intervals.h IntrusiveMap.h IntrusiveSet.h Layer.h Layers.h Libraries.h
Library.h ListCollection.h Locator.h MapCollection.h Marker.h Markers.h
MultisetCollection.h Name.h Names.h NetExternalComponents.h Net.h
Nets.h Occurrence.h Occurrences.h Pad.h Pads.h Pathes.h Path.h
Pin.h Pins.h Plug.h Plugs.h Point.h Points.h Primitives.h Properties.h
Property.h QuadTree.h Quark.h Quarks.h Record.h Reference.h References.h Region.h
Relation.h RoutingPad.h RoutingPads.h Rubber.h Rubbers.h Segment.h Segments.h Selectors.h
SetCollection.h SharedName.h SharedPathes.h SharedPath.h Slice.h Slices.h
SlotAdapter.h Slot.h Symbols.h Tabulation.h Tag.h Tags.h Technology.h Timer.h
Transformation.h Unit.h UpdateSession.h UserGo.h UserGos.h VectorCollection.h Vertical.h
Verticals.h Views.h Warning.h)

set(cpps BasicLayer.cpp Box.cpp CellCollections.cpp Cell.cpp
Commons.cpp Component.cpp CompositeLayer.cpp Contact.cpp DataBase.cpp
DBo.cpp DeepNet.cpp DisplaySlot.cpp DRCError.cpp Entity.cpp
Error.cpp Exception.cpp Go.cpp Hook.cpp Horizontal.cpp HyperNet.cpp
Instance.cpp Interruption.cpp Interval.cpp Layer.cpp Library.cpp
Marker.cpp Name.cpp Net.cpp NetExternalComponents.cpp Occurrence.cpp
Pad.cpp Path.cpp Pin.cpp Plug.cpp Point.cpp Property.cpp
QuadTree.cpp Quark.cpp Record.cpp Reference.cpp Region.cpp
Relation.cpp RoutingPad.cpp Rubber.cpp Segment.cpp SharedName.cpp
SharedPath.cpp Slice.cpp SlotAdapter.cpp Slot.cpp Tabulation.cpp
Tag.cpp Technology.cpp Timer.cpp Transformation.cpp Unit.cpp UpdateSession.cpp
UserGo.cpp Vertical.cpp Warning.cpp)

add_library(hurricane SHARED ${cpps})

install(FILES ${includes} DESTINATION /include/coriolis/hurricane) 
install(TARGETS hurricane DESTINATION /lib)
