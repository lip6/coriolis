#include <string>
#include <vector>
#include <set>

#include "hurricane/Property.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Segment.h"
#include "Configuration.h"
#include "Tree.h"

namespace Hurricane {
  class Net;
  class Instance;
}

namespace Seabreeze {

  using namespace std;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::RoutingPad;
  using Hurricane::Contact;
  using Hurricane::Instance;
  using Hurricane::Segment;
  using Hurricane::PrivateProperty;

//----------------------------------------------------------
// Class : Seabreeze::Elmore

  class Elmore {
    public:
                                  Elmore          ( Net* net = NULL );
                                 ~Elmore          ();
      void                        contFromNet     ( Net* net );
      void                        buildTree       ( RoutingPad* rp ); 
      void                        build_from_Node ( Node* source, Segment* seg );
      Contact*                    build_branch    ( double* R, double* C, Contact* contact );
      void                        Set_RC          ( double* R, double* C, Contact* ct, Segment* sm );
      void                        clearTree       ();
      Tree*                       getTree         ();
      inline const set<Contact*>& get_conts       () const;
      inline       Configuration* getConfig       ();
      int                         delayElmore     ( RoutingPad* rp );
      void                        toTREE          ( ostream& ) const;
    private:
      Configuration* _config;
      set<Contact*>  _conts;
      set<Contact*>  checker;
      Tree*          _tree;
  };
  
  inline const set<Contact*>& Elmore::get_conts () const
  {
    return _conts;
  }

  inline Configuration* Elmore::getConfig ()
  {
    return _config;
  }

//---------------------------------------------------------
// Class : Seabreeze::ElmoreProperty

  class ElmoreProperty : public Hurricane::PrivateProperty {
      friend class ElmoreExtension;
    public:
      static Name _name;
    public:
      static  ElmoreProperty* create       ( Net* net );
              Name            getName      () const;
      virtual string          _getTypeName () const;  
      inline  Elmore*         getElmore    ();  
    protected:
    // Attribute
      Elmore _elmore;
    protected:
    // Constructor
      inline ElmoreProperty ( Net* );
  };
  
  inline ElmoreProperty::ElmoreProperty ( Net* n )
    : PrivateProperty()
    , _elmore(NULL)
  {}

  inline Elmore* ElmoreProperty::getElmore()
  {
    return &_elmore;
  }

//---------------------------------------------------------
// Class : Seabreeze::ElmoreExtension

  class ElmoreExtension {
    public:
      static        void        destroyAll ();
      static        Elmore*     create     ( Net* );
      static inline Tree*       getTree    ( Elmore* );
      static inline void        toTREE     ( const Elmore*, std::ostream& );
      static        void        destroy    ( Net* net );
    private:
      static Net* Nets;
  };

  inline Tree* ElmoreExtension::getTree ( Elmore* elmore )
  {
    return elmore->getTree();
  }

  inline void ElmoreExtension::toTREE ( const Elmore* elmore, std::ostream& out )
  {
    elmore->toTREE(out);
  }
}  // Seabreeze Namespace
