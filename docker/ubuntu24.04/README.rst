.. -*- Mode: rst -*-

Docker development container
============================

For users intending to build Coriolis from sources we provide this development container environment.
It is including companions projects Alliance and Alliance Check Toolkit.

Purpose
^^^^^^^

This container is made for development in a local workspace, bounded inside the container, while
providing a clean, isolated and comprehensive environment.
All dependencies and configurations are met, in a declarative manner.

Users can both develop and generate distribution packages.

Base image
^^^^^^^^^^

Currently, it uses **Ubuntu 24 LTS** as its base image, tagged *ubuntu:24.04* in DockerHub.
Further development will decide if additional base images should be provided in order for instance
to generate other distribution specific packages.

For RPM based distributions, OpenSUSE Build System (OBS) is already in use by the project.
Check out the *packaging* folder in coriolis repo.

Developer guide
===============

Prerequisites
^^^^^^^^^^^^^

A Docker host is required, with docker and docker-compose properly installed and configured.
Current user also needs proper permissions. This is OS specific and out of this documentation scope.
Refer to suitable instructions in `installation documentation <https://docs.docker.com/engine/install/>`_.

First, choose a workspace on the Docker host. That workspace will be mounted read-write inside the development container.
Declare it using WORKSPACE environment variable, for instance:

.. code-block:: bash

    export WORKSPACE="${HOME}/coriolis-2.x"
    mkdir -p ${WORKSPACE}

Then clone the projects repositories within workspace:

.. code-block:: bash

    git clone --recurse-submodules https://github.com/lip6/coriolis.git ${WORKSPACE}/src/coriolis
    git clone https://github.com/lip6/alliance.git                      ${WORKSPACE}/src/alliance
    git clone https://github.com/lip6/alliance-check-toolkit.git        ${WORKSPACE}/src/alliance-check-toolkit

It is also advised to set workspace environment variable in an .env file next to the docker compose file:

.. code-block:: bash

    echo -e "WORKSPACE=${HOME}/coriolis-2.x" >> ${WORKSPACE}/src/coriolis/docker/ubuntu24.04/.env

Note that for the time being, an *src* folder is te be created in workspace, and that repositories are cloned inside.
This comes from compatibility reasons with existing Makefile. Build files will output along that *src* folder.
The build system may remove that requirement in the future. This documentation will be updated accordingly.

Security
^^^^^^^^

For the local binding to work with respect to discretionary access control, the current user UID and GID have to match the **Developer** user inside the container.
Those may be specified either using the .env file or environment variable.

Note: For host distributions enforcing Mandatory Access Control with for instance SELinux, the local workspace binding defines the **:Z** attribute.
This allows the binding in a private mode, e.g. only within this development container.

It is advised to set UID and GID to current user in Docker host machine. The container defaults to **USER 1000:1000**.

If those values do not match current user's, the local directory binding may fail, rendering its content unreadable.
Set their values in the .env file along docker compose:

.. code-block:: bash

    echo -e "UID=$(id -u)" >> ${WORKSPACE}/src/coriolis/docker/ubuntu24.04/.env
    echo -e "GID=$(id -g)" >> ${WORKSPACE}/src/coriolis/docker/ubuntu24.04/.env

And obviously, the container runs in **non privileged** mode.

Workspace view
^^^^^^^^^^^^^^

By default, the user is placed inside the Coriolis repo, along Alliance and Alliance Check Toolkit within workspace.
To set another folder as workspace, either change the *workspaceFolder* parameter in .devcontainer file, or add the *working_dir* in the compose file.

Then either run docker compose restart or rebuild the development container in VSCode.

Persistence
^^^^^^^^^^^

Data persistence is usually done using Docker volumes. We opted for another solution with a local folder binding.
We found this a little more flexible as it keeps the data accessible at all times, even in the event of a Docker malfunction.

The workspace remains a local folder from the Docker host. It is persistent, while the container's filesystem is transient, re-initialized upon every container restart cycle.
The advantage of binding a local folder as workspace is to retain build outputs inside the *$WORKSPACE/coriolis-2.x* folder.

With Linux hosts, there is no performance penalty. Also, the binding is secured through Mandatory Access Control like **SELinux** or **AppArmor**, if the host distribution implements it.

Starting up dev container
^^^^^^^^^^^^^^^^^^^^^^^^^

The container me be started using CLI:

.. code-block:: bash

    docker compose -f "$WORKSPACE/docker/ubuntu24.04/docker-compose-bind.yml" up -d --build

Get the CONTAINER ID, using for instance:

.. code-block:: bash

    docker ps

Then a shell can then be attached to the running container:

.. code-block:: bash

    docker exec -it $CONTAINER_ID bash

Although, the recommended workflow recommends using **VSCode** with **Docker** and **Dev Containers** extensions.
A devcontainer.json configuration exists inside coriolis repo.
Simply configure aforementioned env variable then use the 'Reopen in container' command.

Virtual Environment
^^^^^^^^^^^^^^^^^^^

Coriolis build system relies upon Python modules. Good practices recommends using a Python virtual environment.
The idea is to install recommended version for every build dependency without messing up with system wide installations.

The virtual environment is configured and activated upon container start.

Environment Variables
^^^^^^^^^^^^^^^^^^^^^

All environment variables required by Coriolis and Alliance are configured by the container.
They may then be used by Makefiles and other build tools.

For internal shells, the *developer* user bashrc is also configured to export proper variables,
for instance:

.. code-block:: bash

    cat ~/.bashrc
    [...]
    # Coriolis environment variables
    export PATH="${HOME}/coriolis-2.x/release/install/bin:${PATH}"
    export PYTHONPATH="${HOME}/coriolis-2.x/release/install/lib/python3/dist-packages:${PYTHONPATH}"
    export VIRTUAL_ENV="${HOME}/coriolis-2.x/src/coriolis/.venv"
    export ARCH=x86_64-linux-gnu
    export LD_LIBRARY_PATH="${HOME}/coriolis-2.x/release/install/lib:${HOME}/coriolis-2.x/release/install/lib/${ARCH}:${LD_LIBRARY_PATH}"
    export CORIOLIS_TOP="${HOME}/coriolis-2.x/release/install"
    export ALLIANCE_TOP="${HOME}/coriolis-2.x/release/install"
    export CELLS_TOP="${HOME}/coriolis-2.x/release/install/cells"
    [...]

Note: ARCH value depends on Docker host's architecture. It resolves to *$(gcc -dumpmachine)*.

Graphic Server Socket (Work in Progress)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Some of the Coriolis tools rely on the graphic server (Xorg or Wayland), such as **CGT**.
Running a graphic application from inside a container is technically possible.
This requires sharing the graphic server socket with the container.

Docker production container
============================

Another set of docker and docker compose file is provided in this folder.

Purpose
^^^^^^^

That second container is aimed at production usages, retrieves source codes directly from GitHub and has no binding to host filesystem.

The idea is to reproduce the context of a Coriolis user, installation and using it either from GitHub repos or using distribution packaging.

While very similar to the development container, that one is intended for package testing (installation / uninstallation) and running Coriolis tools
from an clean, isolated environment. This may latter become available in a public Docker registry, an officially supported installation method.

For those matters, this container uses a volume and grants sudo rights to the internal user.
