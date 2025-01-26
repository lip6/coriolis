pdfcontent = [
  'content/pages/users-guide/UsersGuide.rst',
  'content/pages/python-tutorial/PythonTutorial.rst',
  'content/pages/python-cpp/PythonCpp.rst',
  'content/pages/design-flow/DesignFlow.rst',
  'content/pages/stratus/Stratus.rst',
  'content/pages/check-toolkit/CheckToolkit.rst',
  'content/pages/rds/RDS.rst',
]


rstcontent = files(
  'etc/definitions.rst',
  'content/pages/design-flow/DesignFlow_HTML.rst',
  'content/pages/design-flow/QuickStart.rst',
  'content/pages/documentation.rst',
  'content/pages/gitlab.rst',
  'content/pages/homepage.rst',
  'content/pages/python-cpp/Configuration.rst',
  'content/pages/python-cpp/DBoHierarchy.rst',
  'content/pages/python-cpp/DBoStandalone.rst',
  'content/pages/python-cpp/DbU.rst',
  'content/pages/python-cpp/Introduction.rst',
  'content/pages/python-cpp/Name.rst',
  'content/pages/python-cpp/NonDBo.rst',
  'content/pages/python-cpp/PythonCpp_HTML.rst',
  'content/pages/python-tutorial/AdvancedTopics.rst',
  'content/pages/python-tutorial/CellNetComponent.rst',
  'content/pages/python-tutorial/CgtScript.rst',
  'content/pages/python-tutorial/Collections.rst',
  'content/pages/python-tutorial/Environment.rst',
  'content/pages/python-tutorial/Introduction.rst',
  'content/pages/python-tutorial/Netlist.rst',
  'content/pages/python-tutorial/PythonTutorial_HTML.rst',
  'content/pages/python-tutorial/RealDesigns.rst',
  'content/pages/python-tutorial/ToolEngines.rst',
  'content/pages/rds/RDS_HTML.rst',
  'content/pages/presentations/PresentationsPage.rst',
  'content/pages/presentations/Presentations_HTML.rst',
  'content/pages/rds/RDSpage.rst',
  'content/pages/stratus/Developper.rst',
  'content/pages/stratus/DpGen.rst',
  'content/pages/stratus/Language.rst',
  'content/pages/stratus/Patterns.rst',
  'content/pages/stratus/Stratus_HTML.rst',
  'content/pages/symbolic-layout.rst',
  'content/pages/packaging/OBS.rst',
  'content/pages/packaging/Packaging_HTML.rst',
  'content/pages/technical-log/TechnicalLog_HTML.rst',
  'content/pages/technical-log/December_2023.rst',
  'content/pages/users-guide/Configuration.rst',
  'content/pages/users-guide/DesignFlow.rst',
  'content/pages/users-guide/Installation.rst',
  'content/pages/users-guide/LicenseCredits.rst',
  'content/pages/users-guide/Releases.rst',
  'content/pages/users-guide/ScriptsPlugins.rst',
  'content/pages/users-guide/UsersGuide_HTML.rst',
  'content/pages/users-guide/ViewerTools.rst',
)

css = files(
  'themes/nest-coriolis/static/css/nest.css',
  'themes/nest-coriolis/static/css/pygment.css',
  'themes/nest-coriolis/static/css/coriolis.css',
)

plugins = files(
  'pelican-plugins/bootstrap-rst/140x140.png',
  'pelican-plugins/bootstrap-rst/171x180.png',
  'pelican-plugins/bootstrap-rst/300x200.png',
  'pelican-plugins/bootstrap-rst/__init__.py',
  'pelican-plugins/bootstrap-rst/bootstrap.py',
  'pelican-plugins/bootstrap-rst/custom.css',
  'pelican-plugins/bootstrap-rst/directives.py',
  'pelican-plugins/bootstrap-rst/docs-min.css',
  'pelican-plugins/bootstrap-rst/make-glyphicons.py',
  'pelican-plugins/bootstrap-rst/page.tmpl',
  'pelican-plugins/bootstrap-rst/pygments.css',
  'pelican-plugins/bootstrap-rst/roles.py',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap-theme.css',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap-theme.css.map',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap-theme.min.css',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap.css',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap.css.map',
  'pelican-plugins/bootstrap-rst/bootstrap/css/bootstrap.min.css',
  'pelican-plugins/bootstrap-rst/bootstrap/fonts/glyphicons-halflings-regular.eot',
  'pelican-plugins/bootstrap-rst/bootstrap/fonts/glyphicons-halflings-regular.svg',
  'pelican-plugins/bootstrap-rst/bootstrap/fonts/glyphicons-halflings-regular.ttf',
  'pelican-plugins/bootstrap-rst/bootstrap/fonts/glyphicons-halflings-regular.woff',
  'pelican-plugins/bootstrap-rst/bootstrap/js/bootstrap.js',
  'pelican-plugins/bootstrap-rst/bootstrap/js/bootstrap.min.js',
)

dopdf = find_program('etc/doPdf.sh')

pdf_documents = []
foreach document : pdfcontent
    basename = document.split( '/' )[ -1 ].replace( '.rst', '' )
    pdf_documents += custom_target( basename + '_pdf'
                                  , command: [ dopdf
    			                 , '--buildDir=@0@'.format( meson.current_build_dir() )
    			                 , '--sourceDir=@0@'.format( meson.current_source_dir() )
                                         , '--stylesheet=etc/SoC-ReST.tex'
    			                 , '--rst2latex=@0@'.format( rst2latex.full_path() )
    			                 , '--pdflatex=@0@'.format( pdflatex.full_path() )
    			                 , document ]
                                  , input:       document
                                  , output:      basename + '.pdf'
                                  , install:     true
                                  , install_dir: pdfdir
                                  )
endforeach


cdata = configuration_data()
cdata.set('SOURCEDIR'  , meson.current_source_dir())
cdata.set('OUTPUT_PATH', meson.current_build_dir())
cdata.set('SITEURL'    , docsiteurl )
pelicanconf = configure_file( input:        'pelicanconf.py.in'
                            , output:       'pelicanconf.py'
                            , configuration: cdata
                            , install:       false
                            )

htmldoc = custom_target( 'htmldoc'
                       , output: [ 'archives.html'
                                 , 'authors.html'
                                 , 'categories.html'
                                 , 'images'
                                 , 'index.html'
                                 , 'pages'
                                 , 'pdfs'
                                 , 'tags.html'
                                 , 'theme'
                                 ]
                       , depend_files: [ rstcontent, css, plugins, pelicanconf ]
		       , depends: [ pdf_documents ]
                       , command: [ pelican
                                  , '-vD'
                                  , '--ignore-cache'
                                  , '-s'
                                  , pelicanconf
                                  , meson.current_source_dir()/'content'
				  ]
                       , install: true
                       , install_dir: htmldir
                       )

docs = run_target( 'docs'
                 , command: 'true'
                 , depends: [ htmldoc
		            , crlcore_docs
			    , analog_docs
			    , hurricane_docs
			    , viewer_docs
			    , oroshi_docs
			    , unicorn_docs
			    ]
                 )

install_data( install_dir: pdfdir
            , sources:     files('content/pdfs/SAL.Bootcamp.pdf') )
