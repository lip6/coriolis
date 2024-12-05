#!/bin/bash

if [[ ! -d ${HOME}/coriolis-2.x/src/coriolis/.git ]]; then
    git clone -b ${CORIOLIS_BRANCH} --recurse-submodules ${CORIOLIS_URL} ${HOME}/coriolis-2.x/src/coriolis
else
    echo "Coriolis Git repo already exists, skip cloning."
fi

if [[ ! -d ${HOME}/coriolis-2.x/src/alliance/.git ]]; then
    git clone -b ${ALLIANCE_BRANCH} ${ALLIANCE_URL} ${HOME}/coriolis-2.x/src/alliance
else
    echo "Alliance Git repo already exists, skip cloning."
fi

if [[ ! -d ${HOME}/coriolis-2.x/src/alliance-check-toolkit/.git ]]; then
    git clone -b ${ALLIANCE_CT_BRANCH} ${ALLIANCE_CT_URL} ${HOME}/coriolis-2.x/src/alliance-check-toolkit
else
    echo "Alliance-check-toolkit Git repo already exists, skip cloning."
fi

if [[ -f ${HOME}/coriolis-2.x/src/coriolis/docker/ubuntu24.04/Makefile ]]; then
    make -C ${HOME}/coriolis-2.x/src/coriolis -f docker/ubuntu24.04/Makefile pdm-config
fi

# Function to add a line to ~/.bashrc if it is not already present
add_line_to_bashrc() {
  local line="$1"
  local bashrc="$HOME/.bashrc"

  # Check if the line is already present in ~/.bashrc
  if ! grep -Fxq "$line" "$bashrc"; then
    echo "$line" >> "$bashrc"
    echo "Added line to $bashrc: $line"
  else
    echo "Line already present in $bashrc: $line"
  fi
}

add_line_to_bashrc 'export PATH="${HOME}/coriolis-2.x/release/install/bin:${PATH}"'
add_line_to_bashrc 'export PYTHONPATH="${HOME}/coriolis-2.x/release/install/lib/python3/dist-packages:${PYTHONPATH}"'
add_line_to_bashrc 'export VIRTUAL_ENV="${HOME}/coriolis-2.x/src/coriolis/.venv"'

add_line_to_bashrc 'export ARCH=$(gcc -dumpmachine)'
add_line_to_bashrc 'export LD_LIBRARY_PATH="${HOME}/coriolis-2.x/release/install/lib:${HOME}/coriolis-2.x/release/install/lib/${ARCH}:${LD_LIBRARY_PATH}"'

add_line_to_bashrc 'export CORIOLIS_TOP="${HOME}/coriolis-2.x/release/install"'
add_line_to_bashrc 'export ALLIANCE_TOP="${HOME}/coriolis-2.x/release/install"'
add_line_to_bashrc 'export CELLS_TOP="${HOME}/coriolis-2.x/release/install/cells"'

add_line_to_bashrc 'if [[ -z "$VIRTUAL_ENV" ]]; then source $VIRTUAL_ENV/bin/activate; fi'

cat <<EOF
Development container ready
    Repositories: cloned
    PDM project: configured
    Environment variables: set
EOF
