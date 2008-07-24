#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Transistor.h"
#include "MetaTransistor.h"

namespace {

Transistor::Type metaTransistorTypeToTransistorType(const MetaTransistor::Type& type) {
    switch (type) {
        case MetaTransistor::Type::UNDEFINED:
            return Transistor::Type::UNDEFINED;
        case MetaTransistor::Type::NMOS:
            return Transistor::Type::NMOS;
        case MetaTransistor::Type::PMOS:
            return Transistor::Type::PMOS;
        default:
            throw Error("Unknown MetaTransistor Type");
    }
}

}

const Name MetaTransistor::DrainName("DRAIN");
const Name MetaTransistor::SourceName("SOURCE");
const Name MetaTransistor::GridName("GRID");
const Name MetaTransistor::BulkName("BULK");
const Name MetaTransistor::AnonymousName("ANONYMOUS");

MetaTransistor::Type::Type(const Code& code):
    _code(code)
{}

MetaTransistor::Type::Type(const Type& type):
    _code(type._code)
{}

MetaTransistor::Type& MetaTransistor::Type::operator=(const Type& type) {
    _code = type._code;
    return *this;
}

MetaTransistor::MetaTransistor(Library* library, const Name& name):
    Device(library, name),
    _drain(NULL),
    _source(NULL),
    _grid(NULL),
    _bulk(NULL),
    _anonymous(NULL),
    _type(),
    _m(0), _l(0.0), _w(0.0),
    _transistors()
{}

MetaTransistor* MetaTransistor::create(Library* library, const Name& name) {
    MetaTransistor* mTransistor = new MetaTransistor(library, name);

    mTransistor->_postCreate();

    return mTransistor;
}

void MetaTransistor::_postCreate() {
    Inherit::_postCreate();

    _drain = Net::create(this, DrainName);
    _drain->setExternal(true);
    _source = Net::create(this, SourceName);
    _source->setExternal(true);
    _grid = Net::create(this, GridName);
    _grid->setExternal(true);
    _bulk = Net::create(this, BulkName);
    _bulk->setExternal(true);
    _anonymous = Net::create(this, AnonymousName);

    setTerminal(false);
}

void MetaTransistor::setType(Type type) {
    if (type != _type) {
        UpdateSession::open();
        _type = type;
        Transistor::Type ttype = metaTransistorTypeToTransistorType(_type);
        for (Transistors::iterator tit = _transistors.begin();
                tit != _transistors.end();
                tit++) {
            (*tit)->setType(ttype);
        }
        UpdateSession::close();
    }
}


void MetaTransistor::setM(unsigned m) {
    assert(_transistors.size() == _m);
    assert(getInstances().getSize() == _m);
    if (_m != m) {
        UpdateSession::open();
        if (m > _m) {
            Library* library = getLibrary();
            Transformation transformation;
            for (unsigned i=_m; i<m; i++) {
                string transistorNameStr(getString(getName()));
                transistorNameStr += "_Transistor_" + getString(i);
                Name transistorName(transistorNameStr);
                Transistor* transistor = Transistor::create(library, transistorName);
                transistor->setType(metaTransistorTypeToTransistorType(_type));
                transistor->setL(_l);
                transistor->setW(_w);
                _transistors.push_back(transistor);
                Instance* instance = Instance::create(this, transistorName,
                        transistor, transformation, Instance::PlacementStatus::FIXED);
                instance->getPlug(transistor->getNet(GridName))->setNet(_grid);
                instance->getPlug(transistor->getNet(SourceName))->setNet(_source);
                instance->getPlug(transistor->getNet(DrainName))->setNet(_drain);
                instance->getPlug(transistor->getNet(BulkName))->setNet(_bulk);
            }
        } else {
            for (unsigned i=m; i<_m; i++) {
                Transistor* transistor = _transistors.back();
                transistor->destroy();
                _transistors.pop_back();
            }
        }
        UpdateSession::close();
        _m = m;
        updateLayout();
    }
}

void MetaTransistor::setW(DbU::Unit value) {
    _w = value;
    for (Transistors::iterator tit = _transistors.begin();
            tit != _transistors.end();
            tit++) {
        (*tit)->setW(_w);
    }
}

void MetaTransistor::setL(DbU::Unit value) {
    _l = value;
    for (Transistors::iterator tit = _transistors.begin();
            tit != _transistors.end();
            tit++) {
        (*tit)->setL(_l);
    }
    updateLayout();
}

void MetaTransistor::updateLayout() {
    if (_m > 0) {
        assert(_transistors.size() == _m);
        assert(getInstances().getSize() == _m);

        Transformation transformation(0, 0);
        
        UpdateSession::open();
        for_each_instance(instance, getInstances()) {
            instance->setTransformation(transformation);
            Box abox = instance->getAbutmentBox();
            transformation = Transformation(transformation.getTx() + abox.getWidth(), 0);
            end_for;
        }
        UpdateSession::close();
    }
}
