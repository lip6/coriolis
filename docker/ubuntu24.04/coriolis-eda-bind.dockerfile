# Image for vscode container development using local workspace

FROM ubuntu:24.04

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

ARG UID=1000
ARG GID=1000

# Clame back all user ids and create developer user
RUN userdel ubuntu && \
    adduser --uid $UID developer && \
    echo "developer:developer" | chpasswd developer

USER $UID:$GID

VOLUME /home/developer/coriolis-2.x

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

WORKDIR /home/developer/coriolis-2.x/src/coriolis

CMD ["/bin/bash"]
