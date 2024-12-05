# Image for vscode container development using local workspace

FROM ubuntu:24.04

RUN apt-get update -y && \
    apt-get install -y vim git locales build-essential ccache cmake \
    python3 python3-pip python3-venv \
    bison flex \
    qtbase5-dev libqt5svg5-dev libqwt-qt5-dev libbz2-dev \
    rapidjson-dev libboost-all-dev libeigen3-dev libxml2-dev libcairo2-dev \
    libmotif-dev libxpm-dev \
    doxygen pelican texlive-latex-recommended graphviz yosys klayout && \
    export LC_ALL=en_US.UTF-8 && \
    export LANG=en_US.UTF-8 && \
    locale-gen en_US.UTF-8

RUN adduser --disabled-password --gecos "" developer

VOLUME /home/developer/coriolis-2.x

USER developer

COPY setup-coriolis-workspace.sh /home/developer/setup-coriolis-workspace.sh

WORKDIR /home/developer/coriolis-2.x

CMD ["/bin/bash"]
