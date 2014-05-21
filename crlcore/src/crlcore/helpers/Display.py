
# -*- Mode:Python -*-

import os
import os.path
import sys
import string
import Cfg
import Viewer
from   helpers          import ErrorMessage
from   helpers          import WarningMessage
from   helpers          import Debug
from   helpers.Patterns import patternsLUT


displayFile = '<displayFile has not been set>'


stdColors = \
    { 'Black'       : '0,0,0'
    , 'Gray50'      : '050,050,050'
    , 'Gray238'     : '238,238,238'
    , 'White'       : '255,255,255'
    , 'Seashell4'   : '139,134,130'
    , 'Red'         : '255,0,0'      # Shade of Reds.
    , 'OrangeRed'   : "255,69,0"
    , 'DarkOrange'  : "255,140,0"
    , 'Orange'      : "255,165,0"
    , 'Wheat'       : "245,222,179"
    , 'LightPink'   : '255,182,193'
    , 'Green'       : '0,255,0'      # Shade of Green.
    , 'LawnGreen'   : '124,252,0'
    , 'Blue'        : '0,0,255'      # Shade of Blue.
    , 'Aqua'        : '0,255,255'
    , 'BlueViolet'  : '138,43,226'
    , 'Violet'      : '238;130;238'  # Shade of Violets.
    , 'Magenta'     : '255,0,255'
    , 'Magenta4'    : '139,0,139'
    , 'Tan'         : '210,180,140'  # Shade of Browns
    , 'Yellow'      : '255,255,0'    # Shade of Yellows.
    , 'LightYellow' : '255,255,224'
    }


def checkAttribute ( options, attribute, goodType ):
    if goodType == float and isinstance(options[attribute],int): return True

    if not isinstance(options[attribute],goodType):
        raise ErrorMessage( 1,
            ['The <%s> attribute of Drawing must be %s, not %s.' \
                 % (attribute,str(goodType).split("'")[1],helpers.stype(options[attribute]))
             ,str(options[attribute])])
    return True


def checkColor ( color ):
    if not isinstance(color,str):
        raise ErrorMessage( 1,
            ['The <color> attribute of Drawing must be str, not %s.' % (helpers.stype(color))
            ,str(options[attribute])])

    # Try a predefined color lookup.
    if stdColors.has_key(color): return stdColors[color]

    # Try a RGB hexa: #RRGGBB.
    if color[0] == '#':
        for digit in color[1:]:
            if not digit in string.hexdigits:
                raise ErrorMessage( 1,
                    ['Non-hexadecimal digit \'%c\' in <color> attribute of Drawing.' % digit
                    ,color])
        if len(color) != 7:
            raise ErrorMessage( 1,
                ['<color> attribute of Drawing do not have exactly six digits.' % digit
                ,color])
        return color

    # Try a RGB tuple (R,G,B).
    rgb = color.split(',')
    if len(rgb) != 3:
        raise ErrorMessage( 1,
            ['<color> attribute of Drawing is neither hexa nor RGB nor predefined.'
            ,color])

    for component in rgb:
        for digit in component:
            if not digit in string.digits:
                raise ErrorMessage( 1,
                    ['Bad RGB <color> component \'%s\' of Drawing, not an integer.' % component
                    ,color])
        value = int(component)
        if value < 0 or value > 255:
            raise ErrorMessage( 1,
                ['Bad RGB <color> component \'%s\' of Drawing, not between 0 and 255.' % component
                ,color])
    return color


class StyleType ( object ):

    Style       = 1
    Inherit     = 2
    Darkening   = 3
    Group       = 4
    Drawing     = 5

    def __init__ ( self, code ):
        self._code = code
        return

    def __int__ ( self ):
        return self._code

    def __str__ ( self ):
        if self._code == StyleType.Style:     return 'Style'
        if self._code == StyleType.Inherit:   return 'Inherit'
        if self._code == StyleType.Darkening: return 'Darkening'
        if self._code == StyleType.Group:     return 'Group'
        if self._code == StyleType.Drawing:   return 'Drawing'
        return 'Unknown (%d)', self._code

    def __repr__ ( self ):
        return str(self)


Style     = StyleType(StyleType.Style)
Inherit   = StyleType(StyleType.Inherit)
Darkening = StyleType(StyleType.Darkening)
Group     = StyleType(StyleType.Group)
Drawing   = StyleType(StyleType.Drawing)


def loadStyleTuple ( styleTuple ):
    style = None
    group = 'NoGroup'

    entryNo = 0
    for styleEntry in styleTuple:
        entryNo += 1
    
        try:
            if styleEntry[0] == Style:
                if len(styleEntry) != 3:
                    raise ErrorMessage(1,['Malformed entry in <styleTable>.'
                                         ,'Style must have exactly three fields: (Style,id,description).'
                                         ,str(styleEntry)
                                         ])
                styleType, styleId, description = styleEntry

                if not isinstance(styleId,str):
                    raise ErrorMessage(1,['The second Style field, <id> must be a string, not a %s.' % helpers.stype(styleId)
                                         ,str(styleEntry)])
                if not isinstance(description,str):
                    raise ErrorMessage(1,['The third Style field, <description> must be a string, not a %s.' % helpers.stype(styleId)
                                         ,str(styleEntry)])

                style = Viewer.DisplayStyle(styleId)
                style.setDescription(description)
    
            if not style:
                raise ErrorMessage(1,['Bad entry order in <styleTable>.'
                                     ,'First entry must be of Style type.'
                                     ])
    
            if styleEntry[0] == Inherit:
                if len(styleEntry) != 2:
                    raise ErrorMessage(1,['Malformed entry in <didplayTable>.'
                                         ,'Inherit must have exactly two fields: (Inherit,id).'
                                         ,str(styleEntry)
                                         ])
                styleType, styleId = styleEntry
                if not isinstance(styleId,str):
                    raise ErrorMessage(1,['The second Inherit field, <id> must be a string, not a %s.' % helpers.stype(styleId)
                                         ,str(styleEntry)])
                style.inheritFrom(styleId)
    
            if styleEntry[0] == Darkening:
                if len(styleEntry) != 4:
                    raise ErrorMessage(1,['Malformed entry in <didplayTable>.'
                                         ,'Darkening (HSV color) must have exactly four fields: (Darkening,hue,value,saturation).'
                                         ,str(styleEntry)
                                         ])
                styleType, hue, value, saturation = styleEntry
                if not isinstance(hue       ,float) or \
                   not isinstance(value     ,float) or \
                   not isinstance(saturation,float) :
                    raise ErrorMessage(1,['<hue>, <val> & <sat> fields of Darkening must be floats.'
                                         ,'%s is (Type, %s, %s, %s)' % (str(styleEntry)
                                                                       ,helpers.stype(hue)
                                                                       ,helpers.stype(value)
                                                                       ,helpers.stype(saturation))
                                         ])
                style.setDarkening(Viewer.DisplayStyle.HSVr( hue, value, saturation ))
    
            elif styleEntry[0] == Group:
                if len(styleEntry) != 2:
                    raise ErrorMessage(1,['Malformed entry in <styleTable>.'
                                         ,'Group must have exactly two fields: (Group,group).'
                                         ,str(styleEntry)
                                         ])
                styleType, group = styleEntry
                if not isinstance(group,str):
                    raise ErrorMessage(1,['The second Group field, <group> must be a string, not a %s.' % helpers.stype(group)
                                         ,str(styleEntry)])
    
            elif styleEntry[0] == Drawing:
                if len(styleEntry) != 3:
                    raise ErrorMessage(1,['Malformed entry in <styleTable>.'
                                         ,'Rule must have exactly three fields: (Drawing,dname,{dict}).'
                                         ,str(styleEntry)
                                         ])
                styleType, name, options = styleEntry
                arguments = { 'group':group, 'name':name }
                for key in options.keys():
                    if key == 'color':
                        arguments[key] = checkColor(options[key])
                        continue
                    elif key == 'border': checkAttribute(options,'border',int)
                    elif key == 'pattern':
                        if patternsLUT.has_key(options[key]):
                            arguments[key] = patternsLUT[ options[key] ].hexa
                            continue
                        pass
                    elif key == 'threshold': checkAttribute(options,'threshold',float)
                    elif key == 'goMatched': checkAttribute(options,'goMatched',bool)
                    else:
                        w = WarningMessage( ['Unknown Drawing option: <%s>' % key
                                            ,'In %s:<styleTable>:"%s" at index %d.' \
                                                 % (displayFile,style.getName(),entryNo)] )
                        print w
                        continue
                    arguments[key] = options[key]
                style.addDrawingStyle( **arguments )
    
        except Exception, e:
            if style:
                footer = 'In %s:<styleTable>:"%s" at index %d.' % (displayFile,style.getName(),entryNo)
            else:
                footer = 'In %s:<styleTable> at index %d.' % (displayFile,entryNo)
            ErrorMessage.wrapPrint(e,footer)
            if not style: break

    if style != None:
        Viewer.Graphics.addStyle( style )
    return


def loadStyles ( stylesTable, fromFile ):
    global displayFile
    displayFile = fromFile

    try:
        # Check for three levels of tuple for a correct <styleTable>.
        if not isinstance(stylesTable,tuple):
            raise ErrorMessage(1,['Malformed <styleTable>, not even a one level deep tuple.'])
        if not isinstance(stylesTable[0],tuple):
            raise ErrorMessage(1,['Malformed <styleTable>, only two level deep tuple.'])

        for styleTuple in stylesTable:
            loadStyleTuple( styleTuple )
    except Exception, e:
        styleTableExample = [ 'Should be a three level deep tuple:'
                            ,'  ( ( (Style, id, description)'
                            ,'    , (Entry1, ...)'
                            ,'    , (Entry2, ...)'
                            ,'    , (EntryN, ...)'
                            ,'    )'
                            ,'  ,)'
                            ,'If only one style is defined, do not forget the last extra comma.'
                            ,'(putting a level of parenthesis do not create a tuple for one item)'
                            ,'In %s:<styleTable>.' % displayFile
                            ]
        ErrorMessage.wrapPrint(e, styleTableExample)
    return


def loadDefaultStyle ( styleName, fromFile ):
    global displayFile
    displayFile = fromFile

    Viewer.Graphics.setStyle( styleName )
    return
