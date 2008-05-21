
// -*- C++ -*-


# ifndef  __DISPLAYSTYLE_H__
#   define  __DISPLAYSTYLE_H__


# include  <string>
# include  <map>

# include  <QColor>
# include  <QPen>
# include  <QBrush>

# include  "hurricane/Commons.h"
# include  "hurricane/Name.h"

# include  "hurricane/viewer/ScreenUtilities.h"


namespace Hurricane {




  class DrawingStyle {

    public:
    // Constructors & Destructors.
      static DrawingStyle* create       ( const Name&   name
                                        , const string& pattern    ="FFFFFFFFFFFFFFFF"
                                        ,       int     red        =0
                                        ,       int     green      =0
                                        ,       int     blue       =0
                                        ,       int     borderWidth=0
                                        ,       float   threshold  =1.0
                                        );
             DrawingStyle* link         ();
             size_t        unlink       ();

    // Accessors.
      inline const Name&   getName      () const;
      inline const string& getPattern   () const;
      inline const QColor& getColor     () const;
      inline const QPen&   getPen       () const;
      inline const QBrush& getBrush     () const;
      inline float         getThreshold () const;

    protected:
    // Internal - Attributes.
             const Name    _name;
                   string  _pattern;
                   QColor  _color;
                   QPen    _pen;
                   QBrush  _brush;
                   float   _threshold;
                   size_t  _refcount;

    // Internal - Constructors & Destructors.
                           DrawingStyle ( const Name&   name
                                        , const string& pattern    ="FFFFFFFFFFFFFFFF"
                                        ,       int     red        =0
                                        ,       int     green      =0
                                        ,       int     blue       =0
                                        ,       int     borderWidth=0
                                        ,       float   threshold  =1.0
                                        );
                           DrawingStyle ( const DrawingStyle& );
                          ~DrawingStyle ();
             DrawingStyle& operator=    ( const DrawingStyle& );

  };




  class DrawingGroup {

    public:
    // Constructors & Destructors.
                                    DrawingGroup     ( const Name&   name );
                                   ~DrawingGroup     ();
              DrawingGroup*         getClone         ();

    // Methods.
      inline const Name&            getName          () const;
      inline vector<DrawingStyle*>& getDrawingStyles ();
             size_t                 findIndex        ( const Name& key ) const;
             DrawingStyle*          find             ( const Name& key );
             DrawingStyle*          addDrawingStyle  ( const Name&   key
                                                     , const string& pattern
                                                     ,       int     red
                                                     ,       int     green
                                                     ,       int     blue
                                                     ,       int     borderWidth
                                                     ,       float   threshold
                                                     );
      
    protected:
    // Internal - Attributes.
             const Name             _name;
             vector<DrawingStyle*>  _drawingStyles;

    // Internal - Constructors & Destructors.
                                    DrawingGroup     ( const DrawingGroup& );
             DrawingGroup&          operator=        ( const DrawingGroup& );

  };




  class DisplayStyle {

    public:
    // Static Members.
      static const Name             Viewer;
      static const Name             Fallback;
      static const Name             Background;
      static const Name             Foreground;
      static const Name             Rubber;
      static const Name             Phantom;
      static const Name             Boundaries;
      static const Name             Marker;
      static const Name             SelectionDraw;
      static const Name             SelectionFill;
      static const Name             Grid;
      static const Name             Spot;
      static const Name             Ghost;
      static const Name             Text;
      static const Name             Undef;
      static const Name             UnmatchedGroup;

    // Constructor & Destructor.
                                    DisplayStyle     ( const Name& name );
                                   ~DisplayStyle     ();

    // Accessors.
             const Name&            getName          () const;
             const Name&            getGroup         ( const Name& key ) const;
             const string&          getPattern       ( const Name& key ) const;
             const QColor&          getColor         ( const Name& key ) const;
             const QPen&            getPen           ( const Name& key ) const;
             const QBrush&          getBrush         ( const Name& key ) const;
             float                  getThreshold     ( const Name& key ) const;
      inline vector<DrawingGroup*>& getDrawingGroups ();
             DrawingStyle*          find             ( const Name& key ) const;

    // Modifiers.
             void                   inheritFrom      ( const DisplayStyle* base );
             void                   addDrawingStyle  ( const Name&   groupKey
                                                     , const Name&   key
                                                     , const string& pattern
                                                     ,       int     red
                                                     ,       int     green
                                                     ,       int     blue
                                                     ,       int     borderWidth
                                                     ,       float   threshold
                                                     );

    protected:
    // Internals - Attributes.
             const Name             _name;
             vector<DrawingGroup*>  _groups;

    // Internals - Methods.
             void                   findOrCreate     ( const Name& groupKey
                                                     , size_t&     gi );
             void                   find             ( const Name& groupKey
                                                     , const Name& key
                                                     , size_t&     gi
                                                     , size_t&     si );

  };




  // Functions.
  inline const Name&            DrawingStyle::getName          () const { return _name; }
  inline const string&          DrawingStyle::getPattern       () const { return _pattern; }
  inline const QColor&          DrawingStyle::getColor         () const { return _color; }
  inline const QPen&            DrawingStyle::getPen           () const { return _pen; }
  inline const QBrush&          DrawingStyle::getBrush         () const { return _brush; }
  inline float                  DrawingStyle::getThreshold     () const { return _threshold; }

  inline const Name&            DrawingGroup::getName          () const { return _name; }
  inline vector<DrawingStyle*>& DrawingGroup::getDrawingStyles () { return _drawingStyles; }

  inline const Name&            DisplayStyle::getName          () const { return _name; }
  inline vector<DrawingGroup*>& DisplayStyle::getDrawingGroups () { return _groups; }


} // End of Hurricane namespace.


# endif
