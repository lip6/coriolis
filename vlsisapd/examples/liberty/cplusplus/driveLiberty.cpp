#include<string.h>

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Library.h"
#include "vlsisapd/liberty/WireLoad.h"
#include "vlsisapd/liberty/WireLoadSelection.h"
#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/Timing.h"

int main ( int argc, char * argv[] ) {
    LIB::Name libName = LIB::Name("test");
    LIB::Library* library = new LIB::Library(libName);

    std::string valeur, unit;

    // Attributes
    valeur="0.011";        library->addAttribute(LIB::Name("default_inout_pin_cap"),  LIB::Attribute::Double, valeur);
    valeur="enclosed";     library->addAttribute(LIB::Name("default_wire_load_mode"), LIB::Attribute::String, valeur);
    valeur="1", unit="ns"; library->addAttribute(LIB::Name("time_unit"),              LIB::Attribute::Unit,   valeur, unit);
    valeur="1", unit="pf"; library->addAttribute(LIB::Name("capacitive_load_unit"),   LIB::Attribute::Unit,   valeur, unit);

    // WireLoads
    library->addWireLoad(LIB::Name("medium"));
    LIB::WireLoad* wireLoad = library->getWireLoad(LIB::Name("medium"));
    valeur="200";           wireLoad->addAttribute(LIB::Name("slope"),         LIB::Attribute::Double, valeur);
    valeur="1"; unit="200"; wireLoad->addAttribute(LIB::Name("fanout_length"), LIB::Attribute::Pair, valeur, unit);

    // WireLoadSelection
    library->addWireLoadSelection(LIB::Name("medium"));
    LIB::WireLoadSelection* wireLoadSelection = library->getWireLoadSelection();
    wireLoadSelection->addWireLoadArea(0, 500, LIB::Name("small"));

    // Cells
    library->addCell(LIB::Name("inv"));
    LIB::Cell* cell = library->getCell(LIB::Name("inv"));
    valeur="1"; cell->addAttribute(LIB::Name("area"), LIB::Attribute::Double, valeur);
    valeur="inv"; cell->addAttribute(LIB::Name("cell_footprint"), LIB::Attribute::String, valeur);

    LIB::Pin* pin;

    // Pins
    cell->addPin(LIB::Name("e"));
    pin = cell->getPin(LIB::Name("e"));
    valeur="0.008"; pin->addAttribute(LIB::Name("capacitance"), LIB::Attribute::Double, valeur);
    valeur="input"; pin->addAttribute(LIB::Name("direction"), LIB::Attribute::String, valeur);

    cell->addPin(LIB::Name("s"));
    pin = cell->getPin(LIB::Name("s"));
    valeur="i'"; pin->addAttribute(LIB::Name("function"), LIB::Attribute::String, valeur);
    valeur="output"; pin->addAttribute(LIB::Name("direction"), LIB::Attribute::String, valeur);

    // Timing
    LIB::Timing * timing;
    pin->addTiming();
    timing = pin->getTiming().back();
    valeur="negative_unate"; timing->addAttribute(LIB::Name("timing_sense"), LIB::Attribute::String, valeur);
    valeur="e"; timing->addAttribute(LIB::Name("related_pin"), LIB::Attribute::String, valeur);
    valeur="0.101"; timing->addAttribute(LIB::Name("intrinsic_rise"), LIB::Attribute::Double, valeur);


    // Write
    valeur="test.lib"; library->writeToFile(valeur);

    return 0;
}

