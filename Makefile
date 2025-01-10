## Main Coriolis EDA Makefile

### Environment variables and venv setup
LOG_FILE   		:= coriolis_eda-build.log
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
coriolis_eda: venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} >> $(LOG_FILE) 2>&1

### PDKs target
pdk: coriolis_eda
	make -C ./pdk all
	@echo "Built all PDKs wheel packages" >> $(LOG_FILE) 2>&1

### IPs target
ips: coriolis_eda
	make -C ./ips all
	@echo "Built all IPs wheel packages" >> $(LOG_FILE) 2>&1

### All target
all: pdk ips
	@echo "Built all wheel packages" >> $(LOG_FILE) 2>&1

### Install target
install: all
	@echo "Installing all Coriolis wheel packages in ${VENV_PATH} ..." >> $(LOG_FILE) 2>&1
	$(venv) pip3 install --upgrade --force-reinstall ${DIST_PATH}/*.whl
	@echo "Installed all Coriolis wheel packages" >> $(LOG_FILE) 2>&1

## Uninstall target
uninstall:
	make -C ./pdk uninstall
	$(venv) pip3 uninstall -y coriolis_eda
	@echo "Uninstalled all Coriolis wheel packages" >> $(LOG_FILE) 2>&1

### Clean target
clean:
	@echo "Cleaning up..."
	rm -rf ./{build,*.egg-info}
	rm -f $(LOG_FILE)

### MrProper target
mrproper: clean
	rm -rf ${DIST_PATH}
	make -C ./pdk clean
	make -C ./ips clean
