
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                       Damien Dupuis              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/Highlighter.py"                     |
# +-----------------------------------------------------------------+


import re
from   PyQt4.QtCore import Qt
from   PyQt4.QtGui  import QFont
from   PyQt4.QtGui  import QColor
from   PyQt4.QtGui  import QTextCharFormat
from   PyQt4.QtGui  import QSyntaxHighlighter


class Highlighter ( QSyntaxHighlighter ):

  Normal         = 0x0001
  Bold           = 0x0002
  Italic         = 0x0004

  ttyBackground  = QColor.fromRgb( 255, 255, 221 )  # #ffffdd
  ttyBlack       = QColor.fromRgb(  46,  52,  54 )  # #2e3436
  ttyRed         = QColor.fromRgb( 204,   0,   0 )  # #cc0000
  ttyGreen       = QColor.fromRgb(  78, 154,   6 )  # #4e9a06
  ttyYellow      = QColor.fromRgb( 196, 160,   0 )  # #c4a000
  ttyBlue        = QColor.fromRgb(  52, 101, 164 )  # #3465a4
  ttyViolet      = QColor.fromRgb( 117,  80, 123 )  # #75507b
  ttyCyan        = QColor.fromRgb(   6, 152, 154 )  # #06989a
  ttyGrey        = QColor.fromRgb( 211, 215, 207 )  # #d3d7cf
  ttyLightBlack  = QColor.fromRgb(  85,  87,  83 )  # #555753
  ttyLightRed    = QColor.fromRgb( 239,  41,  41 )  # #ef2929
  ttyLightGreen  = QColor.fromRgb( 138, 226,  52 )  # #8ae234
  ttyLightYellow = QColor.fromRgb( 252, 233,  79 )  # #fce94f
  ttyLightBlue   = QColor.fromRgb( 114, 159, 207 )  # #729fcf
  ttyLightViolet = QColor.fromRgb( 173, 127, 168 )  # #ad7fa8
  ttyLightCyan   = QColor.fromRgb(  52, 226, 226 )  # #34e2e2
  ttyLightGrey   = QColor.fromRgb( 238, 238, 236 )  # #eeeeec

  Rules = [ [ttyLightViolet, Bold       , re.compile(r'^Scanning.*'), None]
          , [ttyLightRed   , Bold       , re.compile(r'^Linking.*'), None]
          , [ttyLightGreen , Normal     , re.compile(r'^\[(?P<percent>\s*\d+)%\]\s*(?P<message>Building.*)'), None]
          , [ttyLightGreen , Bold       , re.compile(r'^\[(?P<percent>\s*\d+)%\]\s*(?P<message>Built target.*)'), None]
          , [ttyLightBlue  , Normal     , re.compile(r'^\[(?P<percent>\s*\d+)%\]\s*(?P<message>Generating.*moc_.*)'), None]
          , [ttyLightBlue  , Bold       , re.compile(r'^Generating.*'), None]
          , [ttyLightCyan  , Normal     , re.compile(r'^Install the project.*'), None]
          , [ttyCyan       , Bold       , re.compile(r'^-- Install.*'), None]
          , [ttyCyan       , Bold|Italic, re.compile(r'^-- Up-to-date.*'), None]
          ]

  def __init__ ( self, parent=None ):
    QSyntaxHighlighter.__init__ ( self, parent )
    for rule in Highlighter.Rules:
      if not rule[3]:
        rule[3] = QTextCharFormat()
        rule[3].setForeground( rule[0] )
        if rule[1] & Highlighter.Normal: rule[3].setFontWeight( QFont.Normal )
        if rule[1] & Highlighter.Bold:   rule[3].setFontWeight( QFont.Bold )
        if rule[1] & Highlighter.Italic: rule[3].setFontItalic( True )
    return

  def highlightBlock ( self, line ):
    for rule in Highlighter.Rules:
      m = rule[2].match(line)
      if m:
        if m.groupdict().has_key('percent'):
          self.setFormat( 7, len(line), rule[3] )
        else:
          self.setFormat( 0, len(line), rule[3] )
    return
