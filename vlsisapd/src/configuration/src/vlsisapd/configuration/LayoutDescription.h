// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2010-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/configuration/LayoutDescription.h"  |
// +-----------------------------------------------------------------+


#ifndef  CFG_LAYOUT_DESCRIPTION_H
#define  CFG_LAYOUT_DESCRIPTION_H

#include  <string>
#include  <vector>
#include  <map>
#include  <iostream>


namespace Cfg {

  class Configuration;
  class ConfigurationWidget;
  class LayoutDescription;


// -------------------------------------------------------------------
// Class  :  "Cfg::WidgetDescription".

  class WidgetDescription {
    public:
      enum Type { Title=1, Section=2, Rule=3, Parameter=4 };
    public:
      static std::string  typeToString ( Type );
      static Type         stringToType ( const std::string& );
    public:
      inline static WidgetDescription* rule       ();
      inline static WidgetDescription* title      ( const std::string& title );
      inline static WidgetDescription* section    ( const std::string& title, int column );
      inline static WidgetDescription* parameter  ( const std::string& id
                                                  , const std::string& label 
                                                  , int                column
                                                  , int                span
                                                  , int                flags
                                                  );
      inline        Type               getType    () const;
      inline        const std::string& getId      () const;
      inline        const std::string& getLabel   () const;
      inline        int                getColumn  () const;
      inline        int                getSpan    () const;
      inline        int                getFlags   () const;
    private:
      Type         _type;
      std::string  _id;                // Note: title, section & rule have no id.
      std::string  _label;
      int          _column;
      int          _span;
      int          _flags;
    private:
      inline  WidgetDescription ( Type               type
                                , const std::string& id    ="<none>"
                                , const std::string& label ="<none>"
                                , int                column=0
                                , int                span  =1
                                , int                flags =0
                                );
  };


// Inline Functions.
  inline  WidgetDescription::WidgetDescription ( Type               type
                                               , const std::string& id 
                                               , const std::string& label 
                                               , int                column
                                               , int                span
                                               , int                flags )
    : _type(type), _id(id), _label(label), _column(column), _span(span), _flags(flags)
  { }

  inline WidgetDescription* WidgetDescription::rule ()
  { return new WidgetDescription(Rule); }

  inline WidgetDescription* WidgetDescription::title ( const std::string& title )
  { return new WidgetDescription(Title,"<none>",title); }

  inline WidgetDescription* WidgetDescription::section ( const std::string& section, int column )
  { return new WidgetDescription(Section,"<none>",section,column); }

  inline WidgetDescription* WidgetDescription::parameter ( const std::string& id
                                                         , const std::string& label 
                                                         , int                column
                                                         , int                span
                                                         , int                flags
                                                         )
  { return new WidgetDescription(Parameter,id,label,column,span,flags); }

  inline WidgetDescription::Type  WidgetDescription::getType    () const { return _type; }
  inline const std::string&       WidgetDescription::getId      () const { return _id; }
  inline const std::string&       WidgetDescription::getLabel   () const { return _label; }
  inline int                      WidgetDescription::getColumn  () const { return _column; }
  inline int                      WidgetDescription::getSpan    () const { return _span; }
  inline int                      WidgetDescription::getFlags   () const { return _flags; }


// -------------------------------------------------------------------
// Class  :  "Cfg::TabDescription".

  class TabDescription {
    public:
      inline                                        TabDescription ( LayoutDescription*, const std::string& name, const std::string& id );
             void                                   addWidget      ( WidgetDescription* );
      inline const std::string&                     getName        () const;
      inline const std::string&                     getId          () const;
      inline const std::vector<WidgetDescription*>& getWidgets     () const;
    private:
      LayoutDescription*              _layout;
      std::string                     _name;
      std::string                     _id;
      std::vector<WidgetDescription*> _widgets;
  };


// Inline Methods.
  inline TabDescription::TabDescription ( LayoutDescription* layout, const std::string& name, const std::string& id )
    : _layout(layout), _name(name), _id(id), _widgets()
  { }

  inline const std::string& TabDescription::getName () const
  { return _name; }

  inline const std::string& TabDescription::getId () const
  { return _id; }

  inline const std::vector<WidgetDescription*>& TabDescription::getWidgets () const
  { return _widgets; }


// -------------------------------------------------------------------
// Class  :  "Cfg::LayoutDescription".


  class LayoutDescription {
    public:
      inline static size_t                       getTimestamp      ();
    public:
      inline                                     LayoutDescription ( Configuration* );
             WidgetDescription*                  getWidget         ( const std::string& id );
             void                                addWidgetLookup   ( WidgetDescription* );
      inline void                                addTab            ( TabDescription* );
      inline void                                addTab            ( const std::string& tabName, const std::string& id );
      inline TabDescription*                     getBackTab        ();
             TabDescription*                     getTab            ( const std::string& tabName, const std::string& id="no_id" );
      inline const std::vector<TabDescription*>& getTabs           () const;
             void                                addRule           ( const std::string& tabName );
             void                                addTitle          ( const std::string& tabName
                                                                   , const std::string& title );
             void                                addSection        ( const std::string& tabName
                                                                   , const std::string& section
                                                                   , int                column=0 );
             void                                addParameter      ( const std::string& tabName
                                                                   , const std::string& id
                                                                   , const std::string& label
                                                                   , int                column=0
                                                                   , int                span  =1
                                                                   , unsigned int       flags =0 );
             ConfigurationWidget*                buildWidget       ( unsigned int       flags );
             void                                writeToStream     ( std::ostream&, const std::string& ) const;
    private:
      static size_t                                         _timestamp;
             Configuration*                                 _configuration;
             std::vector<TabDescription*>                   _tabs;
             std::map<const std::string,WidgetDescription*> _widgets;
  };


// Inline Methods.
  inline size_t  LayoutDescription::getTimestamp () { return _timestamp; }

  inline LayoutDescription::LayoutDescription ( Configuration* cfg )
    : _configuration(cfg), _tabs(), _widgets()
  { }

  inline void  LayoutDescription::addTab ( TabDescription* tab )
  { _tabs.push_back(tab); }

  inline void  LayoutDescription::addTab ( const std::string& tabName, const std::string& id )
  { addTab ( new TabDescription(this,tabName,id) ); }

  inline TabDescription* LayoutDescription::getBackTab ()
  { return _tabs.back(); }

  inline const std::vector<TabDescription*>& LayoutDescription::getTabs () const
  { return _tabs; }



} // End of Cfg namespace.


#endif  // __CFG_LAYOUT_DESCRIPTION__
