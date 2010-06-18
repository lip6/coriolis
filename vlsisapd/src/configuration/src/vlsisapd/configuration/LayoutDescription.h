
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./LayoutDescription.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __CFG_LAYOUT_DESCRIPTION__
#define  __CFG_LAYOUT_DESCRIPTION__

#include  <string>
#include  <vector>


namespace Cfg {

  class Configuration;
  class ConfigurationWidget;


// -------------------------------------------------------------------
// Class  :  "Cfg::WidgetDescription".


  class WidgetDescription {
    public:
      enum Type { Title=1, Section=2, Rule=3, Parameter=4 };
    public:
      inline static WidgetDescription* rule       ();
      inline static WidgetDescription* title      ( const std::string& title );
      inline static WidgetDescription* section    ( const std::string& title, int column );
      inline static WidgetDescription* parameter  ( const std::string& id
                                                  , const std::string& label 
                                                  , int                column
                                                  , int                flags
                                                  );
      inline        Type               getType    () const;
      inline        const std::string& getId      () const;
      inline        const std::string& getLabel   () const;
      inline        const std::string& getTitle   () const;
      inline        int                getColumn  () const;
      inline        int                getFlags   () const;
    private:
      Type         _type;
      std::string  _id;      // Alternate id, title or section.
      std::string  _label;
      int          _column;
      int          _flags;
    private:
      inline  WidgetDescription ( Type               type
                                , const std::string& id    ="<none>"
                                , const std::string& label ="<none>"
                                , int                column=0
                                , int                flags=0
                                );
  };


// Inline Functions.
  inline  WidgetDescription::WidgetDescription ( Type               type
                                               , const std::string& id 
                                               , const std::string& label 
                                               , int                column
                                               , int                flags )
    : _type(type), _id(id), _label(label), _column(column), _flags(flags)
  { }

  inline WidgetDescription* WidgetDescription::rule ()
  { return new WidgetDescription(Rule); }

  inline WidgetDescription* WidgetDescription::title ( const std::string& title )
  { return new WidgetDescription(Title,title); }

  inline WidgetDescription* WidgetDescription::section ( const std::string& title, int column )
  { return new WidgetDescription(Section,title,"<none>",column); }

  inline WidgetDescription* WidgetDescription::parameter ( const std::string& id
                                                         , const std::string& label 
                                                         , int                column
                                                         , int                flags
                                                         )
  { return new WidgetDescription(Parameter,id,label,column,flags); }

  inline WidgetDescription::Type  WidgetDescription::getType    () const { return _type; }
  inline const std::string&       WidgetDescription::getId      () const { return _id; }
  inline const std::string&       WidgetDescription::getLabel   () const { return _label; }
  inline const std::string&       WidgetDescription::getTitle   () const { return _id; }
  inline int                      WidgetDescription::getColumn  () const { return _column; }
  inline int                      WidgetDescription::getFlags   () const { return _flags; }


// -------------------------------------------------------------------
// Class  :  "Cfg::TabDescription".


  class TabDescription {
    public:
      inline                                        TabDescription ( const std::string& name );
      inline void                                   addWidget      ( WidgetDescription* );
      inline const std::string&                     getName        () const;
      inline const std::vector<WidgetDescription*>& getWidgets     () const;
    private:
      std::string                     _name;
      std::vector<WidgetDescription*> _widgets;
  };


// Inline Methods.
  inline TabDescription::TabDescription ( const std::string& name )
    : _name(name), _widgets()
  { }

  inline void  TabDescription::addWidget ( WidgetDescription* widget )
  { _widgets.push_back(widget); }

  inline const std::string& TabDescription::getName () const
  { return _name; }

  inline const std::vector<WidgetDescription*>& TabDescription::getWidgets () const
  { return _widgets; }


// -------------------------------------------------------------------
// Class  :  "Cfg::LayoutDescription".


  class LayoutDescription {
    public:
      inline                                     LayoutDescription ( Configuration* );
      inline void                                addTab            ( TabDescription* );
      inline TabDescription*                     getBackTab        ();
      inline const std::vector<TabDescription*>& getTabs           () const;
             ConfigurationWidget*                buildWidget       ();
    private:
      Configuration*               _configuration;
      std::vector<TabDescription*> _tabs;
  };


// Inline Methods.
  inline  LayoutDescription::LayoutDescription ( Configuration* cfg )
    : _configuration(cfg), _tabs()
  { }

  inline void  LayoutDescription::addTab ( TabDescription* tab )
  { _tabs.push_back(tab); }

  inline TabDescription* LayoutDescription::getBackTab ()
  { return _tabs.back(); }

  inline const std::vector<TabDescription*>& LayoutDescription::getTabs () const
  { return _tabs; }



} // End of Cfg namespace.


#endif  // __CFG_LAYOUT_DESCRIPTION__
