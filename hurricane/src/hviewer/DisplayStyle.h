
// -*- C++ -*-


# ifndef  __DISPLAYSTYLE_H__
#   define  __DISPLAYSTYLE_H__


# include  <string>
# include  <map>

# include  <QColor>
# include  <QPen>
# include  <QBrush>

# include  "Commons.h"

# include  "ScreenUtilities.h"


namespace Hurricane {




  class DisplayStyle {

    // Sub-Class.
    protected:
      class DrawingStyle {
        // Attributes.
        protected:
          string  _pattern;
          QColor  _color;
          QPen    _pen;
          QBrush  _brush;
        // Constructor.
        public:
                               DrawingStyle ( const string& pattern    ="FFFFFFFFFFFFFFFF"
                                            ,       int     red        =0
                                            ,       int     green      =0
                                            ,       int     blue       =0
                                            ,       int     borderWidth=0
                                            );
        // Accessors.
        public:
          inline const string& getPattern   () const;
          inline const QColor& getColor     () const;
          inline const QPen&   getPen       () const;
          inline const QBrush& getBrush     () const;
      };


      typedef map<string,DrawingStyle>                  StyleMap;
      typedef map<string,DrawingStyle>::const_iterator  StyleIterator;

    // Attributes.
    protected:
      StyleMap  _drawingStyles;

    // Constructor.
    public:
                          DisplayStyle      ();

    // Accessors.
    public:
      const string&       getPattern        ( const string& key ) const;
      const QColor&       getColor          ( const string& key ) const;
      const QPen&         getPen            ( const string& key ) const;
      const QBrush&       getBrush          ( const string& key ) const;

    // Methods.
    protected:
      const DrawingStyle& findDrawingStyle ( const string& key ) const;
      

    // Modifiers.
    public:
            void          addDrawingStyle  ( const string& key
                                           , const string& pattern
                                           ,       int     red
                                           ,       int     green
                                           ,       int     blue
                                           ,       int     borderWidth
                                           );

  };




  // Functions.
  inline const string& DisplayStyle::DrawingStyle::getPattern () const { return _pattern; }
  inline const QColor& DisplayStyle::DrawingStyle::getColor   () const { return _color; }
  inline const QPen&   DisplayStyle::DrawingStyle::getPen     () const { return _pen; }
  inline const QBrush& DisplayStyle::DrawingStyle::getBrush   () const { return _brush; }

}


# endif
