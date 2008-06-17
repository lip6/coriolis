
// -*- C++ -*-


# include  <QBitmap>

# include  "hurricane/BasicLayer.h"

# include  "hurricane/viewer/ScreenUtilities.h"




namespace {


  inline bool charToBits ( uchar& b, char c, bool low=true )
  {
    bool   isValid = true;
    uchar  value   = 0xF;

    if ( (c >= '0') && (c <= '9') ) value = (uchar)( c - '0' );
    else {
      if ( (c >= 'a') && (c <= 'f') ) value = (uchar)( c - 'a' + 10 );
      else {
        if ( (c >= 'A') && (c <= 'F') ) value = (uchar)( c - 'A' + 10 );
        else
          isValid = false;
      }
    }

    b &= (low) ? 0xF0  : 0x0F;
    b += (low) ? value : (value<<4);

    return isValid;
  } 


  bool  getPattern ( uchar bits[], const std::string& pattern )
  {
    bool isValid = true;

    for ( size_t i=0 ; i<pattern.size() ; i++ ) {
      if ( i > 15 ) { isValid = false; break; }
      isValid &= charToBits ( bits[i/2], pattern[i], i%2 );
    }
    return isValid;
  }


}




namespace Hurricane {


QBrush  getBrush ( const string& pattern, int red, int green, int blue )
{
  if ( pattern != "FFFFFFFFFFFFFFFF" ) {
    uchar  bits[8];
    
    if ( !getPattern(bits,pattern) )
      cerr << "[WARNING] Invalid stipple pattern: \"0x" << pattern << "\"." << endl;
    
    return QBrush ( QColor(red,green,blue), QBitmap::fromData(QSize(8,8),bits,QImage::Format_Mono) );
  }

  return QBrush ( QColor(red,green,blue), Qt::SolidPattern );
}


} // End of Hurricane namespace.
