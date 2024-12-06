#!/bin/bash

if [[ ! -d ${HOME}/coriolis-2.x/src/coriolis/.git ]]; then
    echo "Coriolis Git repo not found, cloning..."
    git clone -b ${CORIOLIS_BRANCH} --recurse-submodules ${CORIOLIS_URL} ${HOME}/coriolis-2.x/src/coriolis
else
    echo "Coriolis Git repo already exists, skip cloning."
fi

if [[ ! -d ${HOME}/coriolis-2.x/src/alliance/.git ]]; then
    echo "Alliance Git repo not found, cloning..."
    git clone -b ${ALLIANCE_BRANCH} ${ALLIANCE_URL} ${HOME}/coriolis-2.x/src/alliance
else
    echo "Alliance Git repo already exists, skip cloning."
fi

if [[ ! -d ${HOME}/coriolis-2.x/src/alliance-check-toolkit/.git ]]; then
    echo "Alliance-check-toolkit Git repo not found, cloning..."
    git clone -b ${ALLIANCE_CT_BRANCH} ${ALLIANCE_CT_URL} ${HOME}/coriolis-2.x/src/alliance-check-toolkit
else
    echo "Alliance-check-toolkit Git repo already exists, skip cloning."
fi

if [[ -d "${VENV_PATH}" ]]; then
    echo "PDM project and virtual env are already configured, skip configuration."
else
    echo "PDM project and virtual env not configured, configuring..."
    make -C ${HOME}/coriolis-2.x/src/coriolis -f docker/ubuntu24.04/Makefile pdm-config
fi

cat <<EOF
Development container ready
    Repositories:          cloned
    PDM project:           configured
EOF
