## IP wheel packages build Makefile

### Environment variables and venv setup
LOG_FILE   		:= ip-build.log
DIST_PATH  		:= ../dist

venv        	= . ${VENV_PATH}/bin/activate;

PIP_INSTALL 	= $(venv) pip3 install --upgrade
PACKAGES    	= pip setuptools build wheel cibuildwheel

venv-setup:
	${PYTHON3} -m venv ${VENV_PATH} > $(LOG_FILE) 2>&1
	$(foreach pkg,$(PACKAGES),$(PIP_INSTALL) $(pkg);)

### IPs
IPS = lsxram

define build_ip
$(1): venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} $(1) >> $(LOG_FILE) 2>&1
endef

# Generate build targets
$(foreach ip,$(IPS),$(eval $(call build_ip,$(ip))))

### All target
all: $(IPS)
	@echo "Finished building all cells wheel packages" >> $(LOG_FILE) 2>&1

## Uninstall target
define uninstall_ips
uninstall-$(1):
	$(venv) pip3 uninstall -y coriolis_ip_$(1)
endef

$(foreach ip,$(IPS),$(eval $(call uninstall_ips,$(ip))))

uninstall: $(foreach ip,$(IPS),uninstall-$(ip))
	@echo "Uninstalled all IPs wheel packages"

### Clean target
define clean_target
clean-$(1):
	rm -rf $(1)/{build,*.egg-info}
endef

# Generate clean targets
$(foreach ip,$(IPS),$(eval $(call clean_target,$(ip))))

clean: $(foreach ip,$(IPS),clean-$(ip))
	rm -f $(LOG_FILE)
