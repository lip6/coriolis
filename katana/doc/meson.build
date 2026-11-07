
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

katana_docs = custom_target( 'katana-docs'
                           , input:        doxyfile
                           , output:       'doc'
                           , command:      [doxygen, doxyfile]
                           , depend_files: [katana_hpps, katana_cpps, style_confs]
                           , install:      true
                           , install_dir:  htmldir
                           )

