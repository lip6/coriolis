
# -*- Mode:Python -*-

import os
import os.path
import sys
import Cfg
import helpers
from   helpers   import ErrorMessage
from   helpers   import Debug


confFile = '<confFile has not been set>'


class ParameterType ( object ):

    Bool       = 1
    Int        = 2
    Enumerate  = 3
    Percentage = 4
    Double     = 5
    String     = 6
    Tab        = 10
    Title      = 11
    Section    = 12
    Rule       = 13
    Option     = 14

    def __init__ ( self, code ):
        self._code = code
        return

    def __int__ ( self ):
        return self._code

    def __str__ ( self ):
        if self._code == ParameterType.Bool:       return 'TypeBool'
        if self._code == ParameterType.Int:        return 'TypeInt'
        if self._code == ParameterType.Enumerate:  return 'TypeEnumerate'
        if self._code == ParameterType.Percentage: return 'TypePercentage'
        if self._code == ParameterType.Double:     return 'TypeDouble'
        if self._code == ParameterType.String:     return 'TypeString'
        if self._code == ParameterType.Tab:        return 'TypeTab'
        if self._code == ParameterType.Title:      return 'TypeTitle'
        if self._code == ParameterType.Section:    return 'TypeSection'
        if self._code == ParameterType.Rule:       return 'TypeRule'
        if self._code == ParameterType.Option:     return 'TypeOption'
        return 'TypeUnknown (%d)', self._code

    def __repr__ ( self ):
        return str(self)


TypeBool       = ParameterType(ParameterType.Bool)
TypeInt        = ParameterType(ParameterType.Int)
TypeEnumerate  = ParameterType(ParameterType.Enumerate)
TypePercentage = ParameterType(ParameterType.Percentage)
TypeDouble     = ParameterType(ParameterType.Double)
TypeString     = ParameterType(ParameterType.String)
TypeTab        = ParameterType(ParameterType.Tab)
TypeTitle      = ParameterType(ParameterType.Title)
TypeSection    = ParameterType(ParameterType.Section)
TypeRule       = ParameterType(ParameterType.Rule)
TypeOption     = ParameterType(ParameterType.Option)


def loadParameters ( parametersData, fromFile ):
    global confFile
    confFile = fromFile

    entryNo = 0
    for entry in parametersData:
        entryNo += 1

        try:
            if len(entry) < 3:
                raise ErrorMessage(['Malformed entry in <parametersTable>.'
                                   ,'Less than three fields: (path_name, Type, default_value [,options]).'
                                   ,str(entry)
                                   ])
            if len(entry) == 3:
                pathName, paramType, value = entry
                options = None
            elif len(entry) == 4:
                pathName, paramType, value, options = entry
            else:
                raise ErrorMessage(['Malformed entry in <parametersTable>.'
                                   ,'More than four fields: (path_name, Type, default_value [,options]).'
                                   ,str(entry)
                                   ])

            if paramType == TypeBool:
                param = Cfg.getParamBool(pathName)
                param.setBool(value)
            if paramType == TypeInt:
                param = Cfg.getParamInt(pathName)
                param.setInt(value)
            if paramType == TypeEnumerate:
                param = Cfg.getParamEnumerate(pathName)
                param.setInt(value)
            if paramType == TypePercentage:
                param = Cfg.getParamPercentage(pathName)
                param.setPercentage(value)
            if paramType == TypeDouble:
                param = Cfg.getParamDouble(pathName)
                param.setDouble(value)
            if paramType == TypeString:
                param = Cfg.getParamString(pathName)
                param.setString(value)

            if options and param:
                for key in options.keys():
                    if   key == 'min': param.setMin(options[key])
                    elif key == 'max': param.setMax(options[key])
                    elif key == 'values':
                        for name, value in options[key]:
                            param.addValue(name, value)
                    elif key == 'slaves':
                        for slave in options[key]:
                            param.addSlave(slave)
                    elif key == 'flags':
                        param.flags = options[key]

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<parametersTable> at index %d.' % (confFile,entryNo))
    return


def loadLayout ( layoutData, fromFile ):
    global confFile
    confFile = fromFile

    layout  = Cfg.Configuration.get().getLayout()
    tabName = None

    entryNo = 0
    for entry in layoutData:
        entryNo += 1

        try:
            if not isinstance(entry,tuple):
                raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                   ,'Not a tuple (a, b, c, ...) or (a,).'
                                   ,str(entry)
                                   ])
            if entry[0] == TypeTab:
                if len(entry) != 3:
                    raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                       ,'Tab must have exactly three fields: (TypeTab, tab_name, tag).'
                                       ,str(entry)
                                       ])
                layoutType, tabName, tagName = entry
                layout.addTab  ( tabName, tagName )

            if not tabName:
                raise ErrorMessage(['Bad entry order in <layoutTable>.'
                                   ,'First entry must be of Tab type.'
                                   ])

            if entry[0] == TypeTitle:
                if len(entry) != 2:
                    raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                       ,'Title must have exactly two fields: (TypeTitle, title).'
                                       ,str(entry)
                                       ])
                layoutType, title = entry
                layout.addTitle ( tabName, title )
            elif entry[0] == TypeSection:
                if len(entry) != 3:
                    raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                       ,'Section must have exactly three fields: (TypeSection, section, column).'
                                       ,str(entry)
                                       ])
                layoutType, section, column = entry
                layout.addSection ( tabName, section, column )
            elif entry[0] == TypeRule:
                if len(entry) != 1:
                    raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                       ,'Rule must have exactly one fields: (TypeRule,).'
                                       ,str(entry)
                                       ])
                layout.addRule ( tabName )
            elif entry[0] == TypeOption:
                if len(entry) < 4:
                    raise ErrorMessage(['Malformed entry in <layoutTable>.'
                                       ,'Option must have at least four fields: (TypeOption,path_name,text,column[,span,flags]).'
                                       ,str(entry)
                                       ])
                if len(entry) == 4:
                    layoutType, pathName, text, column = entry
                    layout.addParameter ( tabName, pathName, text, column )
                elif len(entry) == 5:
                    layoutType, pathName, text, column, span = entry
                    layout.addParameter ( tabName, pathName, text, column, span )
                elif len(entry) == 6:
                    layoutType, pathName, text, column, span, flags = entry
                    layout.addParameter ( tabName, pathName, text, column, span, flags )

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<layoutTable> at index %d.' % (confFile,entryNo))
    return


def loadCompatXml ():
    xmlConf = helpers.sysConfDir+'/tools.configuration.xml'
    print 'Load XML', xmlConf
    Cfg.Configuration.get().readFromFile(xmlConf)
    return
