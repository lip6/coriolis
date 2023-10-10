
 UNAME_S            = $(shell uname -s)
 UNAME_R            = $(shell uname -r)
 DEVTOOLSET_VERSION = 7

# We must use devtoolset only under RHEL6 & clones.
 DEVTOOLSET_OPTION =
 ifeq ($(UNAME_S),Linux)
   ifneq ($(findstring .el7.,$(UNAME_R)),)
     DEVTOOLSET_OPTION = --devtoolset=$(DEVTOOLSET_VERSION)
   endif
   ifneq ($(findstring .slsoc6.,$(UNAME_R)),)
     DEVTOOLSET_OPTION = --devtoolset=$(DEVTOOLSET_VERSION)
   endif
 endif

 venv = . venv/bin/activate

 SMP_FLAGS = 
 ifneq ($(shell which nproc 2> /dev/null),)
   SMP_FLAGS = -j$(shell nproc)
 endif

 ISYS_ROOT         = $(shell ./bootstrap/coriolisEnv.py --query-inst-root)
 SRC_ROOT          = $(shell ./bootstrap/coriolisEnv.py --query-src-root)
 BUILD_ROOT        = $(ISYS_ROOT)/build
 INST_ROOT         = $(ISYS_ROOT)/install
 SRC_ALLIANCE_ROOT = $(SRC_ROOT)/alliance/alliance/src


help:
	@echo "============================================================================"; \
	 echo "Coriolis build & install top Makefile";                                        \
	 echo "";                                                                             \
	 echo "This Makefile is only a wrapper around virtualenv, pdm & meson.";              \
	 echo "";                                                                             \
	 echo "To build it, simply type (in coriolis/, at the shell prompt):";                \
	 echo "    ego@home:coriolis> make install";                                          \
	 echo "To remove the build directory (and keep the software installed):";             \
	 echo "    ego@home:coriolis> make clean_build";                                      \
	 echo "To fully remove build & install:";                                             \
	 echo "    ego@home:coriolis> make uninstall";                                        \
	 echo "To build Alliance";                                                            \
	 echo "    ego@home:coriolis> make install_alliance";                                 \
	 echo "    ";                                                                         \
	 echo "    Note: The Alliance repository must have been cloned beforehand";           \
	 echo "============================================================================"; \
	 echo "SMP_FLAGS  = $(SMP_FLAGS)" ;                                                   \
	 echo "BUILD_ROOT = $(BUILD_ROOT)" ;                                                  \
	 echo "INST_ROOT  = $(INST_ROOT)" ;                                                   \
	 echo "SRC_ROOT   = $(SRC_ROOT)" ;                                                    \
	 echo "============================================================================";


check_dir:
	@if [ "`pwd`" != "${HOME}/coriolis-2.x/src/coriolis" ]; then      \
	   echo "Coriolis uses a fixed directory from the user's root.";  \
	   echo "You must put in:";                                       \
	   echo "    <${HOME}/coriolis-2.x/src/coriolis>";                \
	   echo "Instead of:";                                            \
	   echo "    <`pwd`>";                                            \
	   echo "Stopping build.";                                        \
	   exit 1;                                                        \
	 fi


check_venv:
	@if [ ! -d "./venv" ]; then python3 -m venv venv; fi


pip_pdm: check_venv
	@$(venv); pip install pdm


pdm_deps: pip_pdm
	@$(venv); pdm install --no-self 


install: check_dir pdm_deps
	@meson setup $(BUILD_ROOT)
	@meson configure $(BUILD_ROOT) --prefix $(INST_ROOT)
	@ninja -v -C $(BUILD_ROOT) install
	@echo "";                                                                             \
	 echo "============================================================================"; \
	 echo "Coriolis has been successfully built";                                         \
	 echo "============================================================================"; \
	 echo "It has been installed under the directory:";                                   \
	 echo "    $(INST_ROOT)/{bin,lib,lib64,include,share,...}";                           \
	 echo "============================================================================";


clean_build: check_dir
	@echo "Removing the build tree only."; \
	 echo "    $(BUILD_ROOT)";             \
	 rm -rf $(BUILD_ROOT)


uninstall: check_dir
	@echo "Removing the whole build & install tree..."; \
	 echo "    $(INST_ROOT)";                           \
	 rm -rf $(INST_ROOT)


check_alliance_dir:
	@if [ ! -d "$(SRC_ROOT)/alliance" ]; then            \
	   echo "Alliance source repository not found in:";  \
	   echo "    $(SRC_ROOT)/alliance";                  \
	   echo "Stopping build.";                           \
	   exit 1;                                           \
	 fi


install_alliance:
	@export ALLIANCE_TOP=$(INST_ROOT);                                          \
	 export LD_LIBRARY_PATH=$(INST_ROOT)/lib:${LD_LIBRARY_PATH};                \
	 cd $(SRC_ALLIANCE_ROOT);                                                   \
	 sed -i 's,dirs="\\$$newdirs documentation",dirs="$$newdirs",' ./autostuff; \
	 ./autostuff clean; ./autostuff;                                            \
	 mkdir -p $(BUILD_ROOT); cd $(BUILD_ROOT);                                  \
	 $(SRC_ALLIANCE_ROOT)/configure --prefix=$(INST_ROOT) --enable-alc-shared;  \
	 make -j1 install


