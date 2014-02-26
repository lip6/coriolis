//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>
//
// Authors-Tag 


#include "hurricane/Property.h"
#include "hurricane/Instance.h"
#include "nimbus/StepProperty.h"

namespace Nimbus {

  using namespace Hurricane;

  typedef  StandardPrivateProperty<unsigned>  InstanceProperty;

  template<>
  Name InstanceProperty::_name = "Nimbus::InstanceProperty";


unsigned getStep(const Instance& instance)
{
    Property* property = instance.getProperty(InstanceProperty::staticGetName());
    if (!property)
        return 0;
    
    if (!dynamic_cast<InstanceProperty*>(property))
    {
        throw Error("bad property type");
    }

    return (static_cast<InstanceProperty*>(property))->getValue();
}

void setStep(Instance& instance, unsigned step)
{
    Property* property = instance.getProperty(InstanceProperty::staticGetName());
    if (!property)
    {
        property = InstanceProperty::create(step);
        instance.put(property);
        return;
    }
    if (!dynamic_cast<InstanceProperty*>(property))
    {
        throw Error("bad property type");
    }
    (static_cast<InstanceProperty*>(property))->setValue(step);
}

} // End of Nimbus namespace.
