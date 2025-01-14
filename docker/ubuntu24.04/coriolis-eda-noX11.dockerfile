# Image for vscode container development using local workspace

FROM ubuntu:24.04

RUN apt-get update -y && \
    apt-get install -y vim git locales build-essential ccache cmake \
    python3 python3-pip python3-venv \
    bison flex \
    qtbase5-dev libqt5svg5-dev libqwt-qt5-dev libbz2-dev \
    rapidjson-dev libboost-all-dev libeigen3-dev libxml2-dev libcairo2-dev \
    libmotif-dev libxpm-dev \
    doxygen pelican texlive-latex-recommended graphviz yosys klayout \
    binutils-dev libiberty-dev && \
    export LC_ALL=en_US.UTF-8 && \
    export LANG=en_US.UTF-8 && \
    locale-gen en_US.UTF-8

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN userdel $(getent passwd ${USER_ID} | cut -d':' -f1) && \
    groupadd -g ${GROUP_ID} developer && \
    adduser --uid ${USER_ID} --gid ${GROUP_ID} --disabled-password --gecos "" --shell /bin/bash developer

USER developer

COPY setup-coriolis-workspace.sh /home/developer/.local/bin/setup-coriolis-workspace.sh

COPY .bashrc.d/10_coriolis_env.sh /home/developer/.bashrc.d/10_coriolis_env.sh

RUN cat <<EOF >> /home/developer/.bashrc

# Source all scripts in ~/.bashrc.d
if [[ -d "\${HOME}/.bashrc.d" ]]; then
  for file in "\${HOME}/.bashrc.d"/*; do
    [ -r "\$file" ] && [ -f "\$file" ] && source "\$file"
  done
fi

# Check if Coriolis workspace is setup
if [[ ! -d "\${HOME}/coriolis-2.x/src/coriolis/.git" ]]; then
  echo "Coriolis workspace is not setup yet, you may run 'setup-coriolis-workspace.sh'"
fi
EOF

VOLUME /home/developer/coriolis-2.x

WORKDIR /home/developer/coriolis-2.x

CMD ["/bin/bash"]
