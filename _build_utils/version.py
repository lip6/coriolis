#!/usr/bin/env python3

def get_version():
    from dunamai import Version
    version = Version.from_git()

    if version.distance > 0:
        version.stage = "a"
        version.revision = version.distance
        version.distance = 0
    return version.serialize()


if __name__ == "__main__":
    print(get_version())
