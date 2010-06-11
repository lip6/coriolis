#ifndef __GDS_ELEMENT_H
#define __GDS_ELEMENT_H

namespace AGDS {
class Element {
    protected:
        inline   Element (int layer);

    public:
        virtual ~Element ();
        virtual bool write (std::ofstream &file) = 0;

    protected:
        int _layer;
};

inline Element::Element(int layer) : _layer(layer) {}
}
#endif

