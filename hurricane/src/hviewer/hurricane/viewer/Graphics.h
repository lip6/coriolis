
// -*- C++ -*-


# ifndef  __GRAPHICS_H__
#   define  __GRAPHICS_H__


# include  <string>
# include  <vector>

# include  "hurricane/viewer/DisplayStyle.h"

class QFont;
class QColor;
class QPen;
class QBrush;




namespace Hurricane {


  class Name;


  class Graphics {

    public:
    // Accessors.
      static  Graphics*             getGraphics   ();
      static  const QFont           getFixedFont  ( bool bold=false, bool underline=false );
      static  const Name&           getGroup      ( const Name& key );
      static  const QColor&         getColor      ( const Name& key );
      static  const QPen&           getPen        ( const Name& key );
      static  const QBrush&         getBrush      ( const Name& key );
      static  const string&         getPattern    ( const Name& key );
      static  float                 getThreshold  ( const Name& key );

    // Modifiers.
       static void                  addStyle      ( DisplayStyle* displayStyle );
       static void                  setStyle      ( const Name& key );
       static DisplayStyle*         getStyle      ( const Name& key );
       static DisplayStyle*         getStyle      ();

    // Internals - Attributes.
    protected:
      static Graphics*              _singleton;
             vector<DisplayStyle*>  _styles;
             DisplayStyle*          _active;

    // Internals - Constructors & Destructors.
                                    Graphics      ();
                                    Graphics      ( const Graphics& );
              Graphics&             operator=     ( const Graphics& );
                                   ~Graphics      ();

    // Internals - Methods.
              size_t                _findStyle    ( const Name& key ) const;
              void                  _addStyle     ( DisplayStyle* displayStyle );
              void                  _setStyle     ( const Name& key );
              DisplayStyle*         _getStyle     ( const Name& key );
              DisplayStyle*         _getStyle     () const;
      inline  const Name&           _getGroup     ( const Name& key ) const;
      inline  const QColor&         _getColor     ( const Name& key ) const;
      inline  const QPen&           _getPen       ( const Name& key ) const;
      inline  const QBrush&         _getBrush     ( const Name& key ) const;
      inline  const string&         _getPattern   ( const Name& key ) const;
      inline  float                 _getThreshold ( const Name& key ) const;

  };


  inline const Name&     Graphics::_getGroup     ( const Name& name ) const
  { return _active->getGroup(name); }

  inline const QColor&   Graphics::_getColor     ( const Name& name ) const
  { return _active->getColor(name); }

  inline const QPen&     Graphics::_getPen       ( const Name& name ) const
  { return _active->getPen(name); }

  inline const QBrush&   Graphics::_getBrush     ( const Name& name ) const
  { return _active->getBrush(name); }

  inline const string&   Graphics::_getPattern   ( const Name& name ) const
  { return _active->getPattern(name); }

  inline float           Graphics::_getThreshold ( const Name& name ) const
  { return _active->getThreshold(name); }

  inline DisplayStyle*   Graphics::_getStyle     () const
  { return _active; }




} // End of Hurricane namespace.


# endif
