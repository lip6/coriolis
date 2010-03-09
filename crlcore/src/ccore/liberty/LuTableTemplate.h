
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
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
// Date   : 15/05/2007
// Author : Marek Sroka                    <Marek.Sroka@lip6.fr>
//
// Authors-Tag 
#ifndef CRL_CLUTABLETEMPLATE_H
#define CRL_CLUTABLETEMPLATE_H

#include <string>
#include <vector>
using namespace std;

#include  "hurricane/DBo.h"
# include  "hurricane/Slot.h"
using namespace Hurricane;

namespace CRL {
    
class CLibertyTechnology;

class CLuTableTemplate : public DBo {

    // Types
    // *****

    friend class CLibertyTechnology;

    public:typedef DBo Inherit;

    public:class Variable {
    // ******************
               
        public:class Type {
        // **************
            public:enum Code { UNDEFINED=0, CONSTRAINED_PIN_TRANSITION=1, RELATED_PIN_TRANSITION=2, INPUT_NET_TRANSITION=3, TOTAL_OUTPUT_NET_CAPACITANCE=4 };
            private: Code _code;

            public: Type(const Code& code = UNDEFINED);
            public: Type(const Type& type);

            public: Type& operator=(const Type& type);

            public: operator const Code&() const {return _code;};

            public: const Code& getCode() const {return _code;};

            public: string  _getTypeName() const { return _TName("CLuTableTemplate::Variable::Type"); };
            public: string  _getString() const;
            public: Record* _getRecord() const;
        };

        private:Type          _type;
        private:vector<double> _indexes;
                 
        public:Variable(Type type);

        public:string  _getTypeName() const { return _TName("CLuTableTemplate::Variable"); };
        public:string  _getString() const;
        public:Record* _getRecord() const;

        public:Type getType() const { return _type; }
        public:int getSize() const { return _indexes.size(); }
        public:vector<double> getIndexes() const { return _indexes; }

        public:void SetIndexes(unsigned short count,double* values);

        public:void FindIndexes(double index_value,double& index_min,double& index_max,unsigned short& index_index) const;

        public:Variable* getClone() const;
    };
           
    // Attributes
    // **********

    private: string       _name;
    private: CLibertyTechnology* _technology;
    private: Variable*    _variables[3];

    // Constructor & Destructor
    // ************************
             
    private: CLuTableTemplate(string name, CLibertyTechnology* technology);
    public:static CLuTableTemplate* create(string name, CLibertyTechnology* technology);

    public: ~CLuTableTemplate();


    // Accessors
    // *********

    public:string getName() const { return _name; }
    public:CLibertyTechnology* getTechnology() const { return _technology; }

            
    // Updators
    // ********

    public:void AddVariable(unsigned short index,Variable::Type type);
    public:void SetVariableIndexes(unsigned short index,unsigned short count,double* values);

    // Others
    // ******

    public: string _getString() const;
    public: virtual string _getTypeName() const {return _TName("CLuTableTemplate");};
    public: virtual Record* _getRecord() const;

    protected:virtual void _postCreate();

    public:unsigned short getDimension() const;
    public:int getTableSize() const;
    public:int getVariableIndexSize(unsigned short index) const;
    public:vector<double> getVariableIndexes(unsigned short index) const;

    public:double getVariableUnit(unsigned short index) const;
           
    //! Recupere les indices dans les tableaux d'indice bornant une valeur
    /*!
     * /param index       indice de la variable concernée (1, 2 ou 3)
     * /param index_value valeur à borner
     * /param index_min   variable ou ranger la borne min
     * /param index_max   variable ou ranger la borne sup
     * /param index_index position de la borne min dans le tableau des indices
     *
     * Si la valeur ne peut pas être bornée (inferieure au premier indice, ou superieur au dernier)
     * les bornes renvoyées sont les deux indices les plus proches (cad les deux premiers ou les deux derniers)
     */
    public:void FindIndexes(unsigned short index,double index_value,double& index_min,double& index_max
                           ,unsigned short& index_index) const;

    public:CLuTableTemplate* getClone() const;
           
    public:CLuTableTemplate* RemoveVariable(unsigned short index);
};


} //namespace CRL


#endif //CRL_CLUTABLETEMPLATE_H
