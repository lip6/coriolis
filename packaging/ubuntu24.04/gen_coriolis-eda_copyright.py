#!/usr/bin/env python3

from datetime import datetime
import sys
import os
from pathlib import Path
import re

from jinja2 import Environment, FileSystemLoader

# Arguments
pkg_path = sys.argv[1]
print(f"Search for pkg files in {pkg_path}")

def search_pattern(path, pattern, in_name=True):
    base_path = Path(path)

    results = []
    for file in base_path.rglob('*'):
        if not file.is_file():
            continue

        if in_name:
            search_in = file.name
        else:
            search_in = str(file.resolve)

        if pattern.search(search_in):
            results.append(str(file).replace(path, ''))
    
    return results

def search_binaries(path):
    base_path = Path(path)
    pattern = re.compile(r"^.*.so$", re.I)

    results = []
    for file in base_path.rglob('*'):
        if file.is_file() and os.access(file, os.X_OK) and not pattern.search(file.name):
            results.append(str(file).replace(path, ''))

    return results

def search_doc(path, pattern):
    base_path = Path(path)

    results = []
    for file in base_path.rglob('*'):
        if file.is_dir() and pattern.search(str(file.absolute)):
            results.append(f"{file}/*".replace(path, ''))
    
    return results

template_params = {
    "current_year": datetime.now().year
}

## LefDef: C++

pattern = re.compile(r"^.*liblefdef.*.so$", re.I)
template_params["lefdef_so"] = search_pattern(pkg_path, pattern)

## Flute: C++

pattern = re.compile(r"^.*libflute.*.so$", re.I)
template_params["flute_so"] = search_pattern(pkg_path, pattern)

## Hurricane: C++

pattern = re.compile(r"^.*libhurricane.*.so$", re.I)
template_params["hurricane_so"] = search_pattern(pkg_path, pattern)

## Coriolis: includes

pattern = re.compile(r"^.*/include/.*$", re.I)
template_params["coriolis_includes"] = search_pattern(pkg_path, pattern, in_name=False)

## Coriolis: C++

pattern = re.compile(r"^.*lib.*.so$", re.I)
coriolis_so = search_pattern(pkg_path, pattern)

for so in template_params["lefdef_so"]:
  coriolis_so.remove(so)

for so in template_params["flute_so"]:
  coriolis_so.remove(so)

for so in template_params["hurricane_so"]:
  coriolis_so.remove(so)

template_params["coriolis_so"] = coriolis_so

## Coriolis: Python bindings

pattern = re.compile(r"^.*cpython.*.so$", re.I)
template_params["coriolis_bindings"] = search_pattern(pkg_path, pattern)

## Coriolis: Doc

pattern = re.compile(r"^.*/doc/coriolis/.*$", re.I)
template_params["coriolis_docs"] = search_doc(pkg_path, pattern)

## Coriolis: bin

template_params["coriolis_binaries"] = search_binaries(pkg_path)

## Generate copyright file using template

current_dir = os.path.dirname(os.path.abspath(__file__))
jinja2_env = Environment(loader = FileSystemLoader(current_dir + '/templates'))

copyright = jinja2_env.get_template('copyright.jinja2')
copyright_contents = copyright.render(template_params)

with open(f"{pkg_path}/DEBIAN/copyright", 'w') as copyright_file:
    copyright_file.write(copyright_contents)
