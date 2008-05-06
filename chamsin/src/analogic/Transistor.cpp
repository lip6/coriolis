#include "UpdateSession.h"
using namespace Hurricane;

#include "Transistor.h"

void Transistor::createLayout() {
    UpdateSession::open();

    //Net* source = transistor->getSource();
    //Net* drain  = transistor->getDrain();
    //Net* grid   = transistor->getGrid();

    UpdateSession::close();
}
