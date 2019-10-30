#!/usr/bin/env python
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2014-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/AboutWindow.py"                 |
# +-----------------------------------------------------------------+



try:
  import sys
  import traceback
  try:
    from PyQt4.QtCore import Qt
    from PyQt4.QtCore import QEventLoop
    from PyQt4.QtGui  import QDialog
    from PyQt4.QtGui  import QPalette
    from PyQt4.QtGui  import QColor
    from PyQt4.QtGui  import QFont
    from PyQt4.QtGui  import QWidget
    from PyQt4.QtGui  import QFrame
    from PyQt4.QtGui  import QLabel
    from PyQt4.QtGui  import QVBoxLayout
    from PyQt4.QtGui  import QAction
    from PyQt4.QtGui  import QKeySequence
    from PyQt4.QtGui  import QApplication
  except:
    try:
      from PyQt5.QtCore    import Qt
      from PyQt5.QtCore    import QEventLoop
      from PyQt5.QtWidgets import QDialog
      from PyQt5.QtGui     import QPalette
      from PyQt5.QtGui     import QColor
      from PyQt5.QtGui     import QFont
      from PyQt5.QtWidgets import QWidget
      from PyQt5.QtWidgets import QFrame
      from PyQt5.QtWidgets import QLabel
      from PyQt5.QtWidgets import QVBoxLayout
      from PyQt5.QtWidgets import QAction
      from PyQt5.QtGui     import QKeySequence
      from PyQt5.QtWidgets import QApplication
    except:
      print '[ERROR] AboutWindow: Neither PyQt4 nor PyQt5 is available.'
      sys.exit( 1 )
  import Viewer
  import helpers
  from   helpers.io   import ErrorMessage
  from   helpers.io   import WarningMessage
  import plugins
  import chip.Chip
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


# --------------------------------------------------------------------
# Class : "AboutWidget".


class AboutWidget ( QDialog ):

  def __init__ ( self, parent=None ):
    QWidget.__init__ ( self, parent )
   #self.setFixedSize( 500, 400 )
   #self.setStyleSheet( 'background-color: #ffffdd;' )

    topLine = QFrame()
    topLine.setFrameShape( QFrame.HLine )
    topLine.setLineWidth ( 2 )
    botLine = QFrame()
    botLine.setFrameShape( QFrame.HLine )
    botLine.setLineWidth ( 2 )

    title = QLabel( 'CCB' )
    title.setAlignment( Qt.AlignCenter )
    font = title.font()
    font.setPointSize( 72 )
    font.setWeight   ( QFont.Bold )
    title.setFont( font )

    subTitle = QLabel( 'Coriolis & Chams Builder for the Dummies' )
    subTitle.setAlignment( Qt.AlignCenter )
    subTitle.setFont( QFont('Courier',10,QFont.Bold) )

    authors  = QLabel( 'Coriolis CAD System 1.0 . . . . . . . .  ccb 1.0\n'
                       'Copyright (c) 2008-2016 . . . . . . . . . . UPMC\n'
                       'Authors . . . . . . . . . . . . .  Damien Dupuis\n'
                       '        . . . . . . . . . . . . Jean-Paul Chaput\n'
                       'E-Mail  . . . . . . . . Jean-Paul.Chaput@lip6.fr'
                     )
    authors.setAlignment( Qt.AlignCenter )
    authors.setFont( QFont('Courier',10,QFont.Bold) )

    vLayout = QVBoxLayout()
    vLayout.addStretch(10)
    vLayout.addWidget( topLine )
    vLayout.addWidget( title )
    vLayout.addStretch(1)
    vLayout.addWidget( subTitle )
    vLayout.addWidget( authors )
    vLayout.addStretch(1)
    vLayout.addWidget( botLine )
    vLayout.addStretch(10)

    frame = QFrame()
    frame.setFrameShape ( QFrame.Box )
    frame.setFrameShadow( QFrame.Sunken )
    frame.setLayout     ( vLayout )
    frame.setLineWidth  ( 1 )

    vLayout = QVBoxLayout()
    vLayout.addWidget( frame )
    
    self.setLayout( vLayout )

    self._exitAction = QAction( '&Exit', self )
    self._exitAction.setStatusTip( 'Exit CCB (settings are saved)' )
    self._exitAction.setShortcut ( QKeySequence('CTRL+Q') )
   #self._exitAction.triggered.connect( QApplication.closeAllWindows )
    self._exitAction.triggered.connect( self.reject )
    self.addAction( self._exitAction )

    self._closeAction = QAction( '&Close', self )
    self._closeAction.setStatusTip( 'Close the About Window' )
    self._closeAction.setShortcut ( QKeySequence('CTRL+A') )
   #self._closeAction.triggered.connect( self.close )
    self._closeAction.triggered.connect( self.accept )
    self.addAction( self._closeAction )
    
    return


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'file.About'
                         , 'About Coriolis'
                         , 'Informations about the Coriolis Software'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def ScriptMain ( **kw ):
  rvalue = True
  try:
   #helpers.setTraceLevel( 550 )

    aboutWidget = AboutWidget()
    answer = aboutWidget.exec_()
    print 'answer:', answer
    if not answer: return True

  except Exception, e:
    helpers.io.catch( e )

  sys.stdout.flush()
  sys.stderr.flush()
      
  return rvalue
