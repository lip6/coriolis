
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

 ISYS_ROOT = $(shell ./bootstrap/coriolisEnv.py --query-isys-root)
 INST_ROOT = $(shell ./bootstrap/coriolisEnv.py --query-inst-root)


help:
	@echo "============================================================================"; \
	 echo "Coriolis build & install top Makefile";                                        \
	 echo "";                                                                             \
	 echo "This Makefile is only a wrapper around the Coriolis & Chams Builder tool";     \
	 echo "(./boostrap/ccb.py)";                                                          \
	 echo "";                                                                             \
	 echo "To build it, simply type (in coriolis/, at the shell prompt):";                \
	 echo "    ego@home:coriolis> make install";                                          \
	 echo "To remove the build directory (and keep the software installed):";             \
	 echo "    ego@home:coriolis> make clean_build";                                      \
	 echo "To fully remove build & install:";                                             \
	 echo "    ego@home:coriolis> make uninstall";                                        \
	 echo "============================================================================"; \
	 echo "SMP_FLAGS = $(SMP_FLAGS)" ;                                                    \
	 echo "ISYS_ROOT = $(ISYS_ROOT)" ;                                                    \
	 echo "INST_ROOT = $(INST_ROOT)" ;                                                    \
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


poetry_deps: check_venv
	@$(venv); poetry install --no-root 


install: check_dir poetry_deps
	@$(venv); ./bootstrap/ccb.py $(DEVTOOLSET_OPTION) --project=coriolis \
	                                                  --make="$(SMP_FLAGS) install"
	@echo "";                                                                             \
	 echo "============================================================================"; \
	 echo "Coriolis has been successfully built";                                         \
	 echo "============================================================================"; \
	 echo "It has been installed under the directory:";                                   \
	 echo "    $(INST_ROOT)/{bin,lib,lib64,include,share,...}";                           \
	 echo "";                                                                             \
	 echo "You can start the tool by running:";                                           \
	 echo "    ego@home:~> $(INST_ROOT)/bin/coriolis";                                    \
	 echo "============================================================================";


clean_build: check_dir
	@echo "Removing the build tree only."; \
	 echo "    $(INST_ROOT)/../build";     \
	 rm -rf $(INST_ROOT)/../build


uninstall: check_dir
	@echo "Removing the whole build & install tree..."; \
	 echo "    $(ISYS_ROOT)";                           \
	 rm -rf $(ISYS_ROOT)
