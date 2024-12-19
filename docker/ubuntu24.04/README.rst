.. -*- Mode: rst -*-

Docker development container
============================

For users intending to build Coriolis from sources we provide a development container environment.
It is including companions projects Alliance and Alliance Check Toolkit.

This folder provides two Docker compose deployments:
    - A standard, volume based one
    - A second one using local folder binding for the Coriolis workspace

They mainly differ in the way the persistence of the Coriolis workspace folder is achieved.
The default one (as used by VSCode's devcontainer extension) retrieves all repos from GitHub before configuring the workspace.
While the bounded version requires the user to get the source code by himself previously. See guide bellow.

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

**Classic container**

User may adjust the Coriolis and Alliance GitHub repos URIs and branches being checked out in an .env file placed next the docker compose file.

.. code-block:: bash

    echo "CORIOLIS_URL=https://github.com/my_fork/coriolis.git"                  >> docker/ubuntu24.04/.env
    echo "CORIOLIS_BRANCH=my_branch"                                             >> docker/ubuntu24.04/.env
    echo "ALLIANCE_URL=https://github.com/my_fork/alliance.git"                  >> docker/ubuntu24.04/.env
    echo "ALLIANCE_BRANCH=my_branch"                                             >> docker/ubuntu24.04/.env
    echo "ALLIANCE_CT_URL=https://github.com/my_fork/alliance-check-toolkit.git" >> docker/ubuntu24.04/.env
    echo "ALLIANCE_CT_BRANCH=my_branch"                                          >> docker/ubuntu24.04/.env

From now on, simply "docker compose up" the deployment or use VSCode command 'Reopen in container'.
At start, the repos will be cloned if needed and PDM project configured.

    **Important note:** As the changes made to project repositories are kept inside a Docker volume, they persist even when restarting or rebuilding the container.
    Which is intended through Docker container lifecycle. Nevertheless, they are not accessible from host filesystem.
    **Do not** delete underlying volume without pushing your work toward your repos remotes, or it will be lost.

**Bounded Workspace container**

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

Note that for the time being, legacy Coriolis build system requires an *src* folder is to be created in workspace, and that repositories are cloned inside.
Build files will output along that *src* folder.
The build system may remove that requirement in the future. This documentation will be updated accordingly.

Security
^^^^^^^^

Obviously, neither of the provided containers run in **privileged** mode.

In addition, container's internal user is denied sudo permissions, as it exposes to potential root-level exploitation. Remember Docker is daemon based.
Compromising host security through volumes or local filesystem binding is a common attack pattern if the container is breached.

All dependencies and requirements are already installed from Dockerfile. Feel free to add or modify them before rebuilding the container.
For generated distribution package testing purposes, they are either available in bounded folder or can be copied over from volume.

**On bind version**

For the local folder binding to work with respect to discretionary access control, the current user UID and GID have to match the internal user inside the container.

Note: For host distributions enforcing Mandatory Access Control with for instance SELinux, the local workspace binding defines the **:Z** attribute.
This allows the binding in a private mode, e.g. only within this development container.

It is advised to set UID and GID to current user in Docker host machine. The container defaults to **USER 1000:1000**.

If those values do not match current user's, the local directory binding may fail, rendering its content unreadable.
Set their values in the .env file along docker compose:

.. code-block:: bash

    echo -e "UID=$(id -u)" >> ${WORKSPACE}/src/coriolis/docker/ubuntu24.04/.env
    echo -e "GID=$(id -g)" >> ${WORKSPACE}/src/coriolis/docker/ubuntu24.04/.env

**On volume version**

Previous remarks do not apply to the volume version, as it keeps complete isolation from host filesystem.

Workspace view
^^^^^^^^^^^^^^

By default, the user is placed inside the Coriolis repo, along Alliance and Alliance Check Toolkit within workspace.
To set another folder as workspace, either change the *workspaceFolder* parameter in .devcontainer file, or add the *working_dir* in the compose file.

Then either run docker compose restart or rebuild the development container in VSCode.

Persistence
^^^^^^^^^^^

Data persistence is usually done using Docker volumes, that is what the default development containers uses.
We also provided a second one with a local folder binding. Feel free to used the one best suited and flexible regarding your preferences.

The workspace is either a volume or a local folder from the Docker host.
In both cases it is persistent, while the container's filesystem is transient, re-initialized upon every container restart cycle.
Any changes made in the workspace like code edits and build outputs are retained inside the *$WORKSPACE/coriolis-2.x* folder.

With Linux hosts, there is no performance penalty when using either volume or local binding. Also, the binding is secured through Mandatory Access Control like **SELinux** or **AppArmor**, if the host distribution implements it.
That may render the binding variant a little less portable than volume though.
That is why we recommend using the latter as a good standard practice.

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

Coriolis build system relies upon Python modules, managed in a PDM project. It follows good practices by using a Python virtual environment.
The idea is to install recommended version for every build dependency without messing up with system wide installations.

The virtual environment like the PDM project are configured and activated upon container start.

Workspace configuration
^^^^^^^^^^^^^^^^^^^^^^^

An initial configuration is to be done at container start, using provided *setup-coriolis-workspace.sh* script.
That includes cloning the Coriolis, Alliance and Alliance Check Toolkit repositories, and setting up the PDM project.
This operation is done only once, at the first container start or if the volume was recreated.

All environment variables required by Coriolis and Alliance are configured by the container.
They may then be used by Makefiles and other build tools.

The internal user's bashrc is also configured to export proper variables:

.. code-block:: bash

    cat ~/.bashrc
    [...]
    # Coriolis environment variables
    export PATH="${HOME}/coriolis-2.x/release/install/bin:${PATH}"
    export PYTHONPATH="${HOME}/coriolis-2.x/release/install/lib/python3/dist-packages:${PYTHONPATH}"
    export VENV_PATH="${HOME}/coriolis-2.x/src/coriolis/.venv"
    export ARCH=x86_64-linux-gnu
    export LD_LIBRARY_PATH="${HOME}/coriolis-2.x/release/install/lib:${HOME}/coriolis-2.x/release/install/lib/${ARCH}:${LD_LIBRARY_PATH}"
    export CORIOLIS_TOP="${HOME}/coriolis-2.x/release/install"
    export ALLIANCE_TOP="${HOME}/coriolis-2.x/release/install"
    export CELLS_TOP="${HOME}/coriolis-2.x/release/install/cells"
    [...]

Note: ARCH value depends on Docker host's architecture. It resolves to *$(gcc -dumpmachine)*.

Graphic Server Socket
^^^^^^^^^^^^^^^^^^^^^

Some of the Coriolis tools rely on the graphic server (Xorg or Wayland), such as **CGT**.

Running a graphic application from inside a container requires sharing the graphic server socket and configuration with the container.
In compose file, for X11 we use the environment variable:
    - DISPLAY
    - XDG_SESSION_TYPE
    - XDG_RUNTIME_DIR
and X11 socket within /tmp/.X11-unix, along with Wayland counterparts:
    - WAYLAND_DISPLAY
and Wayland socket at: \${XDG_RUNTIME_DIR}/\${WAYLAND_DISPLAY}

Be sure that your host has a graphic server running and those environment variables are properly set.
