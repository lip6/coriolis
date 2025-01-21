## PDKs wheel packages build Makefile

### PDKs
PDKS = common

### Environment variables and venv setup
PYTHON3			:= python3
DIST_PATH  		:= ../dist
VENV_PATH  		:= ../.venv

venv        	= . ${VENV_PATH}/bin/activate;
pip_install 	= $(venv) pip3 install --upgrade
venv_python		= $(venv) ${PYTHON3}

PACKAGES    	= pip setuptools build wheel cibuildwheel

venv_setup:
	${PYTHON3} -m venv ${VENV_PATH}
	$(foreach pkg,$(PACKAGES),$(pip_install) $(pkg);)

define build_pdk
pdk_$(1): venv_setup
	@echo -e "\n\t### Building PDK $(1)" 			2>&1 | tee -a $(1)_build.log
	$(venv_python) -m build -w -o ${DIST_PATH} $(1) 2>&1 | tee -a $(1)_build.log
endef

# Generate build targets
$(foreach pdk,$(PDKS),$(eval $(call build_pdk,$(pdk))))

### All target
all: $(foreach pdk,$(PDKS),pdk_$(pdk))
	@echo "### Finished building all PDK wheel packages"

### Install target
define install_pdk
install_$(1): pdk_$(1)
	$(venv) pip3 install --upgrade --force-reinstall ${DIST_PATH}/pdk_$(1)*.whl
endef

$(foreach pdk,$(PDKS),$(eval $(call install_pdk,$(pdk))))

install: $(foreach pdk,$(PDKS),install_$(pdk))
	@echo "### Installed all PDK wheel packages"

## Uninstall target
define uninstall_pdk
uninstall_$(1):
	$(venv) pip3 uninstall -y pdk_$(1)
endef

$(foreach pdk,$(PDKS),$(eval $(call uninstall_pdk,$(pdk))))

uninstall: $(foreach pdk,$(PDKS),uninstall_$(pdk))
	@echo "### Uninstalled all PDK wheel packages"

### Clean target
define clean
clean_$(1):
	rm -rf $(1)/{build,*.egg-info}
	rm -f $(1)_build.log
endef

# Generate clean targets
$(foreach pdk,$(PDKS),$(eval $(call clean,$(pdk))))

clean: $(foreach pdk,$(PDKS),clean_$(pdk))
	@echo "### Cleaned all PDK wheel packages"
