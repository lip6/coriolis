#!/usr/bin/env python3

from pathlib import Path

def get_version():
    moduleDir   = Path( __file__ ).parent
    versionFile = moduleDir / 'gitversion.txt'
    try:
        from dunamai import Version
        version = Version.from_git()
        
        if version.distance > 0:
            version.stage    = "a"
            version.revision = version.distance
            version.distance = 0
        with versionFile.open( 'w' ) as fd:
            fd.write( version.serialize() )
        return version.serialize()
    except RuntimeError:
        if versionFile.is_file():
            with versionFile.open( 'r' ) as fd:
                version = fd.readline()
                return version
        return '2.5.5'


if __name__ == "__main__":
    print( get_version() )
