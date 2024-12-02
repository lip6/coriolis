# Image for vscode container development using local workspace

FROM ubuntu:24.04

ARG CORIOLIS_URL=https://github.com/lip6/coriolis.git
ARG CORIOLIS_BRANCH=main
ARG ALLIANCE_URL=https://github.com/lip6/alliance.git
ARG ALLIANCE_BRANCH=main
ARG ALLIANCE_CT_URL=https://github.com/lip6/alliance-check-toolkit.git
ARG ALLIANCE_CT_BRANCH=main

RUN apt-get update -y && \
    apt-get install -y vim git locales build-essential ccache cmake \
    python3 python3-pip python3-venv \
    bison flex \
    qtbase5-dev libqt5svg5-dev libqwt-qt5-dev libbz2-dev \
    rapidjson-dev libboost-all-dev libeigen3-dev libxml2-dev libcairo2-dev \
    libmotif-dev libxpm-dev \
    doxygen pelican texlive-latex-recommended graphviz yosys

RUN export LC_ALL=en_US.UTF-8 && \
    export LANG=en_US.UTF-8 && \
    locale-gen en_US.UTF-8

RUN adduser developer && \
    echo "developer:developer" | chpasswd

VOLUME /home/developer/coriolis-2.x

USER developer

RUN cat <<EOF >> /home/developer/setup-coriolis-workspace.sh
#!/bin/bash

if [[ ! -d \${HOME}/coriolis-2.x/src/coriolis ]]; then
    git clone -b \${CORIOLIS_BRANCH} --recurse-submodules \${CORIOLIS_URL} \${HOME}/coriolis-2.x/src/coriolis
fi

if [[ ! -d \${HOME}/coriolis-2.x/src/alliance ]]; then
    git clone -b \${ALLIANCE_BRANCH} \${ALLIANCE_URL} \${HOME}/coriolis-2.x/src/alliance
fi

if [[ ! -d \${HOME}/coriolis-2.x/src/alliance-check-toolkit ]]; then
    git clone -b \${ALLIANCE_CT_BRANCH} \${ALLIANCE_CT_URL} \${HOME}/coriolis-2.x/src/alliance-check-toolkit
fi

if [[ -f \${HOME}/coriolis-2.x/src/coriolis/docker/ubuntu24.04/Makefile ]]; then
    make -C \${HOME}/coriolis-2.x/src/coriolis -f docker/ubuntu24.04/Makefile pdm-config
fi
EOF

RUN chmod u+x /home/developer/setup-coriolis-workspace.sh

RUN cat <<EOF >> /home/developer/.bashrc

# Coriolis environment variables
export PATH="\${HOME}/coriolis-2.x/release/install/bin:\${PATH}"
export PYTHONPATH="\${HOME}/coriolis-2.x/release/install/lib/python3/dist-packages:\${PYTHONPATH}"
export VIRTUAL_ENV="\${HOME}/coriolis-2.x/src/coriolis/.venv"
export ARCH=$(gcc -dumpmachine)
export LD_LIBRARY_PATH="\${HOME}/coriolis-2.x/release/install/lib:\${HOME}/coriolis-2.x/release/install/lib/\${ARCH}:\${LD_LIBRARY_PATH}"
export CORIOLIS_TOP="\${HOME}/coriolis-2.x/release/install"
export ALLIANCE_TOP="\${HOME}/coriolis-2.x/release/install"
export CELLS_TOP="\${HOME}/coriolis-2.x/release/install/cells"
EOF

WORKDIR /home/developer/coriolis-2.x

CMD ["/bin/bash"]
