## Cells wheel packages build Makefile

### Environment variables and venv setup
LOG_FILE   		:= cells-build.log
DIST_PATH  		:= ../dist

venv        	= . ${VENV_PATH}/bin/activate;

PIP_INSTALL 	= $(venv) pip3 install --upgrade
PACKAGES    	= pip setuptools build wheel cibuildwheel

venv-setup:
	${PYTHON3} -m venv ${VENV_PATH} > $(LOG_FILE) 2>&1
	$(foreach pkg,$(PACKAGES),$(PIP_INSTALL) $(pkg);)

### Cells libraries CELLS_LIBS
CELLS_LIBS = hibikino \
	lsxlib \
	mpxlib msplib msxlib \
	niolib nramlib nrf2lib \
	nsxlib nsxlib2 \
	phlib phlib80 \
	sky130 \
	vsclib

define build_target
$(1): venv-setup
	$(venv) ${PYTHON3} -m build -w -o ${DIST_PATH} $(1) >> $(LOG_FILE) 2>&1
endef

# Generate build targets
$(foreach target,$(CELLS_LIBS),$(eval $(call build_target,$(target))))

### All target
all: $(CELLS_LIBS)
	@echo "Finished building all cells wheel packages" >> $(LOG_FILE) 2>&1

## Uninstall target
define uninstall_cells_lib
uninstall-$(1):
	$(venv) pip3 uninstall -y coriolis_cells_$(1)
endef

$(foreach target,$(CELLS_LIBS),$(eval $(call uninstall_cells_lib,$(target))))

uninstall: $(foreach target,$(CELLS_LIBS),uninstall-$(target))
	@echo "Uninstalled all cells wheel packages"

### Clean target
define clean_target
clean-$(1):
	rm -rf $(1)/{build,*.egg-info}
endef

# Generate clean targets
$(foreach target,$(CELLS_LIBS),$(eval $(call clean_target,$(target))))

clean: $(foreach target,$(CELLS_LIBS),clean-$(target))
	rm -f $(LOG_FILE)
