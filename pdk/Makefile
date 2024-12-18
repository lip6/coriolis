## PDK wheel packages build Makefile

### Environment variables and venv setup
LOG_FILE   		:= pdk-build.log
DIST_PATH  		:= ../dist

venv        	= . ${VENV_PATH}/bin/activate;

PIP_INSTALL 	= $(venv) pip3 install --upgrade
PACKAGES    	= pip setuptools build wheel cibuildwheel

venv-setup:
	${PYTHON3} -m venv ${VENV_PATH} > $(LOG_FILE) 2>&1
	$(foreach pkg,$(PACKAGES),$(PIP_INSTALL) $(pkg);)

### PDK
PDKS = common sky130_vsc

define build_pdk
$(1): venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} $(1) >> $(LOG_FILE) 2>&1
endef

# Generate build targets
$(foreach pdk,$(PDKS),$(eval $(call build_pdk,$(pdk))))

### All target
all: $(PDKS)
	@echo "Finished building all PDK wheel packages" >> $(LOG_FILE) 2>&1

## Uninstall target
define uninstall_pdk
uninstall-$(1):
	$(venv) pip3 uninstall -y coriolis_pdk_$(1)
endef

$(foreach pdk,$(PDKS),$(eval $(call uninstall_pdk,$(pdk))))

uninstall: $(foreach pdk,$(PDKS),uninstall-$(pdk))
	@echo "Uninstalled all PDK wheel packages"

### Clean target
define clean_target
clean-$(1):
	rm -rf $(1)/{build,*.egg-info}
endef

# Generate clean targets
$(foreach pdk,$(PDKS),$(eval $(call clean_target,$(pdk))))

clean: $(foreach pdk,$(PDKS),clean-$(pdk))
	rm -f $(LOG_FILE)
