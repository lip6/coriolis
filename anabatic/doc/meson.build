
cdata = configuration_data()
cdata.set('VERSION', meson.project_version())

cdata.set('TOP_SRCDIR' , meson.project_source_root())
cdata.set('DOCDIR'     , meson.current_source_dir ())
cdata.set('DOCBUILDDIR', meson.current_build_dir  () / 'doc')
cdata.set('TAGDIR'     , meson.project_build_root ())

doxyfile = configure_file( input:         'Doxyfile.in'
                         , output:        'Doxyfile'
                         , configuration: cdata
                         , install:       false
                         )

style_confs = files([ 'SoC.css'
                    , 'custom_tabs.css'
                    , 'header.html'
                    , 'footer.html'
                    , 'dynsections.js'
                    , 'jquery.js'
                    , 'socbook.cls'
                    , 'header.tex'
                    ])

anabatic_docs = custom_target( 'anabatic-docs'
                             , input:       doxyfile
                             , output:      'doc'
                             , depend_files: [anabatic_hpps, anabatic_cpps, style_confs]
                             , command:     [doxygen, doxyfile]
                             , install:     true
                             , install_dir: htmldir
                             )

