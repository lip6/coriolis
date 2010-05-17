#ifndef __GDS_ELEMENT_H
#define __GDS_ELEMENT_H

namespace IO {
class GdsElement {
    protected:
        inline   GdsElement (int layer);
        virtual ~GdsElement ();

    public:
        virtual bool write ( ofstream &file ) = 0;

    protected:
        int _layer;
};

inline GdsElement::GdsElement(int layer) : _layer(layer) {}
}
#endif

