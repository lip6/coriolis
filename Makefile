## Main Coriolis EDA Makefile

### Environment variables and venv setup
LOG_FILE   		:= coriolis-eda-build.log
DIST_PATH  		:= ./dist

venv        	= . ${VENV_PATH}/bin/activate;

ifneq (,$(wildcard .env))
    include .env
    export $(shell sed 's/=.*//' .env)
endif

PIP_INSTALL 	= $(venv) pip3 install --upgrade
PACKAGES    	= pip setuptools build wheel cibuildwheel

venv-setup:
	${PYTHON3} -m venv ${VENV_PATH} > $(LOG_FILE) 2>&1
	$(foreach pkg,$(PACKAGES),$(PIP_INSTALL) $(pkg);)

### Coriolis EDA target
coriolis-eda: venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} >> $(LOG_FILE) 2>&1

### All target
all: coriolis-eda
	@echo "Finished building coriolis-eda wheel package" >> $(LOG_FILE) 2>&1

### Install target
install: all
	make -C ./cells all
	make -C ./pdk all
	make -C ./ips all
	$(venv) pip3 install --upgrade ${DIST_PATH}/*.whl
	@echo "Installed coriolis-eda wheel package"

## Uninstall target
uninstall:
	$(venv) pip3 uninstall -y coriolis-eda
	make -C ./cells uninstall
	make -C ./pdk uninstall
	make -C ./ips uninstall
	@echo "Uninstalled coriolis-eda wheel package"

### Clean target
clean:
	@echo "Cleaning up..."
	rm -rf ./{build,*.egg-info}
	rm -f $(LOG_FILE)
	make -C ./cells clean
	make -C ./pdk clean
	make -C ./ips clean

### MrProper target
mrproper: clean
	rm -rf ${DIST_PATH}
