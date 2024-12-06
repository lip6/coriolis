#!/bin/bash

export PATH="${HOME}/coriolis-2.x/release/install/bin:${PATH}"
export PYTHONPATH="${HOME}/coriolis-2.x/release/install/lib/python3/dist-packages:${PYTHONPATH}"
export VENV_PATH="${HOME}/coriolis-2.x/src/coriolis/.venv"

export ARCH=$(gcc -dumpmachine)
export LD_LIBRARY_PATH="${HOME}/coriolis-2.x/release/install/lib:${HOME}/coriolis-2.x/release/install/lib/${ARCH}:${LD_LIBRARY_PATH}"

export CORIOLIS_TOP="${HOME}/coriolis-2.x/release/install"
export ALLIANCE_TOP="${HOME}/coriolis-2.x/release/install"
export CELLS_TOP="${HOME}/coriolis-2.x/release/install/cells"

if [[ -z "$VIRTUAL_ENV" ]]; then
    source ${VENV_PATH}/bin/activate
fi
