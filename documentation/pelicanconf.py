#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from   __future__ import unicode_literals
import os
import os.path
import socket

pelicanDir = os.path.abspath( os.getcwd() )
hostname   = socket.gethostname()

if pelicanDir.endswith('coriolis/documentation'):
  pluginsDir = '/dsk/l1/pelican/pelican-plugins'
  outputDir  = 'output/'
  siteUrl    = ''
elif hostname.startswith('lepka'):
  pluginsDir = '/dsk/l1/pelican/pelican-plugins'
  outputDir  = '/dsk/l1/httpd/coriolis'
  siteUrl    = 'http://localhost/coriolis'
else:
  pluginsDir = '/data/git/pelican-plugins'
  outputDir  = 'output/'
  siteUrl    = ''

PLUGIN_PATHS = [ pluginsDir ]
PLUGINS      = [ 'bootstrap-rst' ]

STATIC_PATHS = [ 'pages/users-guide'
               , 'pages/python-tutorial'
               , 'pages/python-cpp'
               , 'pages/stratus'
               , 'pages/rds'
               , 'images'
               , 'pdfs'
               ]


AUTHOR   = u'Jean-Paul Chaput'
SITENAME = u'Coriolis VLSI CAD Tools'
SITEURL  = siteUrl

THEME = 'nest-coriolis'
#THEME = 'nest'
#THEME = 'dev-random'
#THEME = 'brutalist'

DELETE_OUTPUT_DIRECTORY = True
ARTICLE_EXCLUDES        = [ ]

PATH       = 'content'
PAGE_PATHS = [ 'pages' ]

TIMEZONE     = 'Europe/Paris'
DEFAULT_LANG = u'en'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM         = None
CATEGORY_FEED_ATOM    = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM      = None
AUTHOR_FEED_RSS       = None
DISPLAY_PAGES_ON_MENU = True

# Blogroll
#SOCIAL = (('Pelican'   , 'http://getpelican.com/' ),
#          ('Python.org', 'http://python.org/'     ),
#          ('Jinja2'    , 'http://jinja.pocoo.org/'),)
SOCIAL = None

# Social widget
LINKS  = (('CIAN Team Website'      , 'https://www-soc.lip6.fr/'),
          ('Free Silicon Foundation', 'https://f-si.org'        ),)

DEFAULT_PAGINATION = 10

IGNORE_FILES = [ 'UsersGuide.rst'       # For User's Guide.
               , 'definitions.rst'
               , 'Abstract.rst'
               , 'DesignFlow.rst'
               , 'HTML_defs.rst'
               , 'Pelican_defs.rst'
               , 'Installation.rst'
               , 'LicenseCredits.rst'
               , 'Releases.rst'
               , 'ScriptsPlugins.rst'
               , 'ViewerTools.rst'

               , 'PythonTutorial.rst' # For Python Tutorial
               , 'Introduction.rst'
               , 'Environment.rst'
               , 'CellNetComponent.rst'
               , 'Collections.rst'
               , 'CgtScript.rst'
               , 'Netlist.rst'
               , 'RealDesigns.rst'
               , 'ToolEngines.rst'
               , 'AdvancedTopics.rst'
               , 'AdvancedTopics.rst'
               , 'pdfHeader.rst'

               , 'PythonCpp.rst'      # For Python/C++ Tutorial
               , 'Introduction.rst'
               , 'Configuration.rst'
               , 'DBoStandalone.rst'
               , 'DBoHierarchy.rst'
               , 'NonDBo.rst'
               , 'DbU.rst'
               , 'Name.rst'

               , 'Stratus.rst'        # For Stratus.
               , 'Developper.rst'
               , 'DpGen.rst'
               , 'Language.rst'
               , 'Patterns.rst'

               , 'RDS.rst'            # For RDS.
               , 'RDSpage.rst'
               ]

# Uncomment following line if you want document-relative URLs when developing
RELATIVE_URLS = True

NEST_INDEX_HEADER_TITLE   = u'Alliance/Coriolis VLSI CAD Tools'
NEST_HEADER_LOGO          =  '/images/common/Coriolis-logo-white-4-small.png'
NEST_HEADER_IMAGES        =          'common/layout-motif-faded-4.png'
NEST_LINKS_COLUMN_TITLE   = u'Links'
NEST_SOCIAL_COLUMN_TITLE  = u'Social'
NEST_SITEMAP_COLUMN_TITLE = u'Social'
NEST_COPYRIGHT            = u'Copyright © 2020-2020 Sorbonne Universite'

MENUITEMS = [ ('Git'          , '/pages/gitlab.html' )
            , ('Documentation', '/pages/documentation.html' ) ]
