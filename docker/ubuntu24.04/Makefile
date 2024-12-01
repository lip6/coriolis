VIRTUAL_ENV  := $(if $(VIRTUAL_ENV),$(VIRTUAL_ENV),${PWD})

BUILD_TYPE   := $(if $(BUILD_TYPE),$(BUILD_TYPE),release)

SRC_DIR      := $(if $(SRC_DIR),$(SRC_DIR),${PWD}/../..)

OUT_DIR      := $(if $(OUT_DIR),$(OUT_DIR),${PWD}/${BUILD_TYPE})

ALLIANCE_SRC = ${SRC_DIR}/../alliance/alliance/src

BUILD_DIR    = ${OUT_DIR}/build

PREFIX       = ${OUT_DIR}/install

venv         = . ${VIRTUAL_ENV}/bin/activate;

pkg_python3-coriolis-eda = /bin/bash ${SRC_DIR}/packaging/ubuntu24.04/pkg_python3-coriolis-eda.sh

venv-setup:
	python3 -m venv ${VIRTUAL_ENV}

pdm-setup: venv-setup
	$(venv) pip3 install build
	$(venv) pip3 install pdm
	$(venv) pdm plugin update
	$(venv) pdm sync -d --no-self

pdm-config: pdm-setup
	$(venv) pdm run meson setup ${SRC_DIR} ${BUILD_DIR} --prefix=${PREFIX} --buildtype=${BUILD_TYPE} -Dpython.install_env=prefix -Donly-docs=false

install: pdm-config
	$(venv) pdm run ninja -j`nproc` -C ${BUILD_DIR} install

install_alliance:
	cd ${ALLIANCE_SRC}; \
	sed -i 's,dirs="\\$$newdirs documentation",dirs="$$newdirs",' ./autostuff; \
	./autostuff clean; \
	./autostuff; \
	mkdir -p ${BUILD_DIR}; \
	cd ${BUILD_DIR}; \
	${ALLIANCE_SRC}/configure --prefix=${PREFIX} --enable-alc-shared; \
	make -j`nproc` install

install_docs:
	$(venv) meson configure ${BUILD_DIR} --prefix=${PREFIX} -Ddocs=true
	$(venv) meson install -C ${BUILD_DIR}
	@echo "Documentation generated and installed."

uninstall:
	$(venv) pdm run ninja -C ${BUILD_DIR} uninstall

dist-pkg: install install_docs
	@echo "Building distribution package for Ubuntu 24.04";
	$(pkg_python3-coriolis-eda) ${SRC_DIR} ${PREFIX} ${OUT_DIR}

clean:
	rm -rf ${OUT_DIR}/build/*
	rm -rf ${OUT_DIR}/install/*

nuke-everything:
	rm -rf ${VIRTUAL_ENV}
	rm -rf ${OUT_DIR}
	@echo "";
	@echo "Successfully nuked all files";
	@echo "\t'Ahhhh... much better!' - Duke Nukem";
