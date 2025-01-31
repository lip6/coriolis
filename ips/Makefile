## IPs wheel packages build Makefile

### Environment variables and venv setup
PYTHON3			:= python3
DIST_PATH  		:= ../dist
VENV_PATH  		:= ../.venv

venv        	:= . ${VENV_PATH}/bin/activate;
pip_install 	:= $(venv) pip3 install --upgrade
venv_python		:= $(venv) ${PYTHON3}

PACKAGES    	:= pip setuptools build wheel cibuildwheel

venv_setup:
	${PYTHON3} -m venv ${VENV_PATH}
	$(foreach pkg,$(PACKAGES),$(pip_install) $(pkg);)

### IPs
IPS = lsxram

define build_ip
$(1): venv_setup
	$(venv_python) -m build -w -o ${DIST_PATH} $(1) 2>&1 | tee -a $(1)_build.log
endef

# Generate build targets
$(foreach ip,$(IPS),$(eval $(call build_ip,$(ip))))

### All target
all: $(IPS)
	@echo "### Finished building all cells wheel packages"

## Uninstall target
define uninstall_ips
uninstall-$(1):
	$(venv) pip3 uninstall -y ips_$(1)
endef

$(foreach ip,$(IPS),$(eval $(call uninstall_ips,$(ip))))

uninstall: $(foreach ip,$(IPS),uninstall-$(ip))
	@echo "### Uninstalled all IPs wheel packages"

### Clean target
define clean_target
clean-$(1):
	@rm -rf $(1)/{build,*.egg-info}
	@rm -f $(1)_build.log
endef

# Generate clean targets
$(foreach ip,$(IPS),$(eval $(call clean_target,$(ip))))

clean: $(foreach ip,$(IPS),clean-$(ip))
	@find . -type f -name "*.log" -delete
	@echo "### Cleaned up all IPs wheel packages"
