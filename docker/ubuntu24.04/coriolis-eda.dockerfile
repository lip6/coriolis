# Image for vscode container development using local workspace

FROM ubuntu:24.04

ARG WORKSPACE=

RUN apt-get update -y && \
    apt-get install -y sudo vim git locales build-essential ccache cmake graphviz \
    python3 python3-pip python3-venv python3-doit \
    doxygen pelican texlive-latex-recommended \
    bison flex \
    qtbase5-dev libqt5svg5-dev libqwt-qt5-dev libbz2-dev \
    rapidjson-dev libboost-all-dev libeigen3-dev libxml2-dev libcairo2-dev \
    libmotif-dev libxpm-dev

RUN export LC_ALL=en_US.UTF-8 && \
    export LANG=en_US.UTF-8 && \
    locale-gen en_US.UTF-8

RUN adduser developer && \
    echo "developer:developer" | chpasswd && \
    usermod -aG sudo developer

USER developer

VOLUME /home/developer/coriolis-2.x

WORKDIR /home/developer/coriolis-2.x/src

CMD ["/bin/bash"]
