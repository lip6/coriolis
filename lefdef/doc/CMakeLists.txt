# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<lefdef/doc>" -*-

     set ( htmlInstallDir  share/doc/coriolis2/en/html )
     set ( latexInstallDir share/doc/coriolis2/en/pdf )
     set ( pdfInstallDir   share/doc/coriolis2/en/pdf )

 install ( DIRECTORY support
                     lefapi
                     defapi
                     lefdefref               DESTINATION ${htmlInstallDir}/lefdef )
 install ( FILES     lefapi/lefapi.pdf
                     defapi/defapi.pdf
                     lefdefref/lefdefref.pdf DESTINATION ${pdfInstallDir}/lefdef  )

