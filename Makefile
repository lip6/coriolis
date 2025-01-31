## Main Coriolis EDA Makefile

### Environment variables and venv setup
LOG_FILE   		:= coriolis_eda-build.log
DIST_PATH  		:= ./dist
VENV_PATH       := .venv

venv        	:= . ${VENV_PATH}/bin/activate;
PIP_INSTALL 	:= $(venv) pip3 install --upgrade
PACKAGES    	:= pip setuptools build wheel cibuildwheel

.PHONY: init venv-setup coriolis_eda

init:
	@if [ ! -f .env ]; then \
		echo "Missing .env file. Copying .env.example to .env"; \
		cp .env.example .env; \
	fi
	@if [ -f .env ]; then \
		set -a; \
		. ./.env; \
		set +a; \
	fi

venv-setup:
	${PYTHON3} -m venv ${VENV_PATH} 										2>&1 | tee -a  $(LOG_FILE)
	$(foreach pkg,$(PACKAGES),$(PIP_INSTALL) $(pkg);)

### Coriolis EDA target
coriolis_eda: venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} 							2>&1 | tee -a  $(LOG_FILE)

### PDKs target
pdks: coriolis_eda
	@make -C ./pdks all
	@echo "Built all PDKs wheel packages" 									2>&1 | tee -a  $(LOG_FILE)

### IPs target
ips: coriolis_eda
	@make -C ./ips all
	@echo "Built all IPs wheel packages" 									2>&1 | tee -a  $(LOG_FILE)

### All target
all: pdks ips
	@echo "Built all wheel packages" 										2>&1 | tee -a  $(LOG_FILE)

### Install target
install: all
	@echo "Installing all Coriolis wheel packages in ${VENV_PATH} ..." 		2>&1 | tee -a  $(LOG_FILE)
	$(venv) pip3 install --upgrade --force-reinstall ${DIST_PATH}/*.whl
	@echo "Installed all Coriolis wheel packages" 							2>&1 | tee -a  $(LOG_FILE)

## Uninstall target
uninstall:
	@make -C ./pdks uninstall
	$(venv) pip3 uninstall -y coriolis_eda
	@echo "Uninstalled all Coriolis wheel packages" 						2>&1 | tee -a  $(LOG_FILE)

### Clean target
clean:
	@echo "Cleaning up..."
	@rm -rf ./{build,*.egg-info}
	@rm -f $(LOG_FILE)

### MrProper target
mrproper: clean
	@rm -rf ${DIST_PATH}
	@make -C ./pdks clean
	@make -C ./ips clean
