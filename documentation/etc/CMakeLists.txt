
               set ( htmlInstallDir  share/doc/coriolis2/en/html/etc )

           install ( FILES     Pygments.css SoC.css SoC-ReST.css
                               DESTINATION ${htmlInstallDir} )
           install ( DIRECTORY images/
                               DESTINATION ${htmlInstallDir}/images
                               FILES_MATCHING PATTERN "*.png" )
