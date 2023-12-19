#!/usr/bin/env python3

import os
import os.path
import sys
import time
import shutil
import subprocess
import logging
import optparse


def log ( message ):
    print( message )
    logging.info( message )
    return


class Configuration ( object ):

    def __init__ ( self ):
        self.pluginsDir = 'pelican-plugins'
        self.logDir = 'logs'
        self.target = 'coriolis-d'


class Command ( object ):

    def __init__ ( self, command ):
        self.command = command
        self.outlog  = None
        self.errlog  = None

    def asString ( self ):
        s = ''
        for i in range(len(self.command)):
            if i: s += ' '
            if ' ' in self.command[i]: s += '"'+self.command[i]+'"'
            else:                      s +=     self.command[i]
        return s

    def execute ( self ):
        sys.stdout.flush()
        sys.stderr.flush()
        log( self.asString() )
        child = subprocess.Popen( self.command, stdin =subprocess.PIPE
                                              , stdout=subprocess.PIPE
                                              , stderr=subprocess.PIPE )
        self.outlog, self.errlog = child.communicate()
        if len(self.outlog): logging.info ( self.outlog.decode('utf-8') )
        if len(self.errlog): logging.error( self.errlog.decode('utf-8'))
        status   = child.returncode
        status >>= 8
        return status



class Document ( object ):

    def __init__ ( self, conf, document ):
        self.conf     = conf
        self.document = os.path.basename( document )
        self.rdir     = os.path.dirname ( document )

    def toPdf ( self ):
        pdfDir         = 'content/pdfs'
        stylesheet     = 'etc/SoC-ReST.tex'
        documentPdf    = '{}.pdf'.format( self.document )
        documentRst    = '{}.rst'.format( self.document )
        documentTex    = '{}.tex'.format( self.document )
        documentTmps   = [ documentTex
                         , '{}.log'.format( self.document )
                         , '{}.out'.format( self.document )
                         , '{}.aux'.format( self.document )
                         , '{}.toc'.format( self.document )
                         ]
        targetPdf      = os.path.join( pdfDir, documentPdf )

        cwd = os.getcwd()
        os.chdir(self.rdir)
        os.environ[ 'TEXINPUTS' ] = './etc/images//:./images//:'
        Command( [ 'rst2latex', '--use-latex-toc'
                              , '--stylesheet={}'.format( stylesheet )
                              , documentRst
                 ] ).execute()
        Command( [ 'pdflatex', '-halt-on-error', documentTex ] ).execute()
        Command( [ 'pdflatex', '-halt-on-error', documentTex ] ).execute()
        os.chdir(cwd)
        for file in documentTmps:
            if os.path.isfile( file ):
                os.unlink( file )
        if not os.path.isdir( pdfDir ):
            log( 'mkdir {}'.format( pdfDir ))
            os.mkdir( pdfDir )
        if os.path.exists( targetPdf ): os.unlink( targetPdf )
        log( 'mv {} {}'.format( documentPdf, pdfDir ))
        shutil.move( documentPdf, pdfDir )
        os.chdir( cwd )


if __name__ == '__main__':
    usage = \
      '\n'  \
      '\nThe "build.py" script rebuild the "coriolis.lip6.fr" website hosted'   \
      '\nat the Sorbonne Universite/LIP6. It can work in the following modes:'  \
      '\n'                                                                      \
      '\n1. Fully install the website on a freshly generated docker image.'     \
      '\n2. Update the "coriolis.lip6.fr" website.'                             \
      '\n3. Generate a static website on my laptop ("lepka").'                  \
      '\n4. Generate the static documentation shipped with coriolis (in git).'  \
      '\n'                                                                      \
      '\nNOTE: About the "<-C|--coriolis> remote rebuild option."'              \
      '\n      Your ${HOME}/.ssh/config file must contain an alias for the'     \
      '\n      "coriolis-d" host, as shown below:'                              \
      '\n'                                                                      \
      '\n          Host coriolis-d'                                             \
      '\n              User          pelican'                                   \
      '\n              HostName      localhost'                                 \
      '\n              HostKeyAlias  coriolis-d'                                \
      '\n              Port          2250'                                      \
      '\n              CheckHostIP   no'                                        \
      '\n              ForwardAgent  yes'                                       \
      '\n              ForwardX11    no'                                        \
      '\n              RemoteForward 2227 localhost:2227'                       \
      '\n'                                                                      \
      '\n      It logs as user "pelican" using the ssh port 2250 and creates'   \
      '\n      a tunnel between port 2227 on the remote host (the web server)'  \
      '\n      and port 2227 on the local host (your machine). You *must* have' \
      '\n      an ssh tunnel between your machine and "bop" on the local port'  \
      '\n      2227 for this to work. The "2227" tunnel between the web server' \
      '\n      and "bop" is used to perform "git pull".'                        \
      '\n'                                                                      \
      '\n./build.py <options>'
    parser = optparse.OptionParser( usage )
    parser.add_option( '-p', '--pelican' , action='store_true', dest='doPelican' , help='Run pelican.' )
    parser.add_option( '-P', '--pdfs'    , action='store_true', dest='doPdfs'    , help='Generate the PDFs.' )
    parser.add_option( '-C', '--coriolis', action='store_true', dest='doCoriolis', help='Build/update the web site on the server (docker).' )
    (options, args) = parser.parse_args()
    conf = Configuration()
    if not os.path.isdir( conf.logDir ):
        os.mkdir( conf.logDir )
    logging.basicConfig( filename='logs/build-{}.log'.format(time.strftime( '%Y%m%d-%H%M' ))
                       , encoding='utf-8'
                       ,   format='%(asctime)s:%(levelname)s| %(message)s'
                       ,    level=logging.INFO
                       ,    force=True
                       )
    documents = [ Document( conf, 'content/pages/users-guide/UsersGuide' )
                , Document( conf, 'content/pages/python-tutorial/PythonTutorial' )
                , Document( conf, 'content/pages/python-cpp/PythonCpp' )
                , Document( conf, 'content/pages/design-flow/DesignFlow' )
                , Document( conf, 'content/pages/stratus/Stratus' )
                , Document( conf, 'content/pages/check-toolkit/CheckToolkit' )
                , Document( conf, 'content/pages/rds/RDS' )
                ]
    if options.doPdfs:
        for document in documents: document.toPdf()
    if options.doPelican:
        Command( [ 'pelican', '-vD', '--ignore-cache', 'content' ] ).execute()
    sys.exit( 0 )
