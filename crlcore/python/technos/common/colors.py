
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./etc/common/colors.py"                   |
# +-----------------------------------------------------------------+


import string
from   coriolis.helpers.io import ErrorMessage


stdColors = \
    { 'Black'         : '0,0,0'
    , 'Gray50'        : '050,050,050'
    , 'Gray238'       : '238,238,238'
    , 'White'         : '255,255,255'
    , 'Seashell4'     : '139,134,130'
    , 'Ivory'         : '255,255,240'
    , 'Linen'         : '250,240,230'
    , 'Red'           : '255,0,0'      # Shade of Reds.
    , 'OrangeRed'     : "255,69,0"
    , 'DarkOrange'    : "255,140,0"
    , 'Orange'        : "255,165,0"
    , 'Wheat'         : "245,222,179"
    , 'wheat1'        : "255,231,186"
    , 'Pink'          : '255,192,203'
    , 'LightPink'     : '255,182,193'
    , 'Green'         : '0,255,0'      # Shade of Green.
    , 'LawnGreen'     : '124,252,0'
    , 'Blue'          : '0,0,255'      # Shade of Blue.
    , 'Aqua'          : '0,255,255'
    , 'DarkTurquoise' : '0,206,209'
    , 'BlueViolet'    : '138,43,226'
    , 'Violet'        : '238;130;238'  # Shade of Violets.
    , 'Magenta'       : '255,0,255'
    , 'Magenta4'      : '139,0,139'
    , 'Tan'           : '210,180,140'  # Shade of Browns
    , 'Yellow'        : '255,255,0'    # Shade of Yellows.
    , 'LightYellow'   : '255,255,224'
    }


def toRGB ( color ):
    if not isinstance(color,str):
      raise ErrorMessage( 1, [ 'The <color> attribute of Drawing must be str, not %s.' % (helpers.stype(color))
                               , str(options[attribute])] )

    # Try a predefined color lookup.
    if color in stdColors: return stdColors[color]

    # Try a RGB hexa: #RRGGBB.
    if color[0] == '#':
      for digit in color[1:]:
        if not digit in string.hexdigits:
          raise ErrorMessage( 1, ['Non-hexadecimal digit \'%c\' in <color> attribute of Drawing.' % digit, color] )
      if len(color) != 7:
        raise ErrorMessage( 1, ['<color> attribute of Drawing do not have exactly six digits.' % digit, color])
      return color

    # Try a RGB tuple (R,G,B).
    rgb = color.split(',')
    if len(rgb) != 3:
      raise ErrorMessage( 1, ['<color> attribute of Drawing is neither hexa nor RGB nor predefined.', color] )

    for component in rgb:
      for digit in component:
        if not digit in string.digits:
          raise ErrorMessage( 1, ['Bad RGB <color> component \'%s\' of Drawing, not an integer.' % component ,color] )
      value = int(component)
      if value < 0 or value > 255:
        raise ErrorMessage( 1, ['Bad RGB <color> component \'%s\' of Drawing, not between 0 and 255.' % component ,color] )
    return color
