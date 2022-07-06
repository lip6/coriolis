#pragma once
#include <string>
#include <vector>
#include <hurricane/DbU.h>

namespace Hurricane {
  class Layer;
  class Cell;
  class Net;
  class RoutingPad;
  class Contact;
  class Segment;
}

namespace CRL {
  class CellGauge;
  class RoutingGauge;
  class RoutingLayerGauge;
}

namespace Seabreeze {

  using std::string;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::DbU;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::Net;
  using Hurricane::Contact;
  using Hurricane::Segment;

//------------------------------------------------------------------------
// Class : "Seabreeze::Configuration"

  class Configuration {
    public :
    // Constructor & Destructor
      virtual Configuration* clone           () const;
                             Configuration   ();
                             Configuration   ( const Configuration& );
                            ~Configuration   ();
    // Methods
              double          getRct         ();
              double          getRsm         ();
              double          getCsm         ();
      virtual Record*        _getRecord      () const;
      virtual string         _getString      () const;
      virtual string         _getTypeName    () const;
    protected :
    // Attributes
      double _Rct;
      double _Rsm;
      double _Csm;
    private :
      Configuration& operator = ( const Configuration& ) = delete;
  };
}
