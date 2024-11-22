BUILD_TYPE   = release
VENV_PATH    = ${PWD}/.venv
OUT_DIR      = ${PWD}/${BUILD_TYPE}
SRC_DIR      = ${PWD}/../..
BUILD_DIR    = ${OUT_DIR}/build
PREFIX       = ${OUT_DIR}/install

venv                 = . ${VENV_PATH}/bin/activate;
pkg_python3-coriolis = /bin/bash ${PWD}/pkg_python3-coriolis.sh

venv-setup:
	python3 -m venv ${VENV_PATH}

pdm-setup: venv-setup
	$(venv) pip3 install build
	$(venv) pip3 install pdm
	$(venv) pdm plugin update
	$(venv) pdm sync -d --no-self

pdm-config: pdm-setup
	$(venv) pdm run meson setup ${SRC_DIR} ${BUILD_DIR} --prefix=${PREFIX} --buildtype=${BUILD_TYPE} -Dpython.install_env=prefix -Donly-docs=false

install: pdm-config
	$(venv) pdm run ninja -j`nproc` -C ${BUILD_DIR} install

uninstall:
	$(venv) pdm run ninja -C ${BUILD_DIR} uninstall

dist-pkg: install
	@echo "Building distribution package python3-coriolis";
	$(pkg_python3-coriolis) ${PREFIX} ${OUT_DIR}

clean:
	rm -rf ${OUT_DIR}/build/*
	rm -rf ${OUT_DIR}/install/*

nuke-everything:
	rm -rf ${VENV_PATH}
	rm -rf ${OUT_DIR}
	@echo "";
	@echo "Successfully nuked all files";
	@echo "\t'Ahhhh... much better!' - Duke Nukem";
