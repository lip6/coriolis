# Copyright 2022 GlobalFoundries PDK Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#=========================================================================
# ---------------------------------- DRC ---------------------------------
#=========================================================================

SHELL        := /bin/bash
Testing_DIR  ?= $(shell pwd)
run_folder   := $(shell date +'run_%Y_%m_%d_%H_%M')


.DEFAULT_GOAL := all

all : test-DRC-pcell

test-DRC-pcell: Add_run-dir  test-DRC-bjt  test-DRC-diode  test-DRC-MIM  test-DRC-MOSCAP  test-DRC-MOS  test-DRC-RES

#=================================
# --------- RUN FOLDER ----------
#=================================

.ONESHELL:
Add_run-dir:
	@cd $(Testing_DIR)
	@ mkdir -p $(run_folder)/bjt_drc  $(run_folder)/diode_drc  $(run_folder)/mimcap_drc  $(run_folder)/mosfet_drc  $(run_folder)/moscap_drc  $(run_folder)/res_drc


#=================================
# ---------- test-DRC-BJT --------
#=================================

.ONESHELL:
test-DRC-bjt: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for BJT pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/bjt_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/bjt_drc/bjt.log
	@mv -f ../testcases/bjt_pcells*.lyrdb $(run_folder)/bjt_drc


#=================================
# -------- test-DRC-diode -------
#=================================

.ONESHELL:
test-DRC-diode: test-DRC-np_diode  test-DRC-pn_diode test-DRC-np_dw_diode  test-DRC-pn_dw_diode  test-DRC-nwp_diode  test-DRC-dnwpw_diode  test-DRC-dnwps_diode  test-DRC-sc_diode

.ONESHELL:
test-DRC-np_diode: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for np_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/np_diode_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/np_diode.log
	@mv -f ../testcases/np_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-pn_diode: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pn_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pn_diode_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/pn_diode.log
	@mv -f ../testcases/pn_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-np_dw_diode: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for np_dw_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/np_dw_diode_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/np_dw_diode.log
	@mv -f ../testcases/np_dw_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-pn_dw_diode: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pn_dw_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pn_dw_diode_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/pn_dw_diode.log
	@mv -f ../testcases/pn_dw_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-nwp_diode: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nwp_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nwp_diode_pcells.gds   --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/nwp_diode.log
	@mv -f ../testcases/nwp_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-dnwpw_diode: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for dnwpw_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/dnwpw_diode_pcells.gds --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/dnwpw_diode.log 
	@mv -f ../testcases/dnwpw_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-dnwps_diode: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for dnwps_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/dnwps_diode_pcells.gds --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/dnwps_diode.log
	@mv -f ../testcases/dnwps_diode_pcells*.lyrdb $(run_folder)/diode_drc


.ONESHELL:
test-DRC-sc_diode: 	Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for sc_diode pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/sc_diode_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/diode_drc/sc_diode.log
	@mv -f ../testcases/sc_diode_pcells*.lyrdb $(run_folder)/diode_drc


#=================================
# --------- test-DRC-MIM ---------
#=================================

.ONESHELL:
test-DRC-MIM: test-DRC-MIM-A  test-DRC-MIM-B

.ONESHELL:
test-DRC-MIM-A: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for MIM-A pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/MIM-A_pcells.gds        --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mimcap_drc/mim_a_gfA.log
	@mv -f ../testcases/MIM-A_pcells*.lyrdb $(run_folder)/mimcap_drc


.ONESHELL:
test-DRC-MIM-B: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for MIM-B pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/MIM-B_gfB_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mimcap_drc/mim_b_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/MIM-B_gfC_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mimcap_drc/mim_b_gfC.log
	@mv -f ../testcases/MIM-B_gfB_pcells*.lyrdb $(run_folder)/mimcap_drc



#=================================
# --------- test-DRC-MOS ---------
#=================================

.ONESHELL:
test-DRC-MOS: test-DRC-nmos_3p3  test-DRC-nmos_5p0  test-DRC-nmos_6p0  test-DRC-pmos_3p3  test-DRC-pmos_5p0  test-DRC-pmos_6p0 test-DRC-nmos_3p3_dw  test-DRC-nmos_5p0_dw  test-DRC-nmos_6p0_dw  test-DRC-pmos_3p3_dw  test-DRC-pmos_5p0_dw  test-DRC-pmos_6p0_dw test-DRC-nmos_6p0_nat  test-DRC-nmos_10p0_asym  test-DRC-pmos_10p0_asym


.ONESHELL:
test-DRC-nmos_3p3: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_3p3 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_gfC.log
	@mv -f ../testcases/nmos_3p3_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_5p0: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_5p0 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_gfC.log
	@mv -f ../testcases/nmos_5p0_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_6p0: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_6p0 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_gfC.log
	@mv -f ../testcases/nmos_6p0_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_3p3: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_3p3 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_gfC.log
	@mv -f ../testcases/pmos_3p3_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_5p0: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_5p0 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_gfC.log
	@mv -f ../testcases/pmos_5p0_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_6p0: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_6p0 pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_gfC.log
	@mv -f ../testcases/pmos_6p0_pcells*.lyrdb $(run_folder)/mosfet_drc

.ONESHELL:
test-DRC-nmos_3p3_dw: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_3p3_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_3p3_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_3p3_dw_gfC.log
	@mv -f ../testcases/nmos_3p3_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_5p0_dw: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_5p0_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_5p0_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_5p0_dw_gfC.log
	@mv -f ../testcases/nmos_5p0_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_6p0_dw: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_6p0_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_dw_gfC.log
	@mv -f ../testcases/nmos_6p0_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_3p3_dw: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_3p3_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_3p3_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_3p3_dw_gfC.log
	@mv -f ../testcases/pmos_3p3_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_5p0_dw: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_5p0_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_5p0_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_5p0_dw_gfC.log
	@mv -f ../testcases/pmos_5p0_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_6p0_dw: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_6p0_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_dw_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_dw_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_6p0_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_6p0_dw_gfC.log
	@mv -f ../testcases/pmos_6p0_dw_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_6p0_nat: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_6p0_nat pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_nat_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_nat_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_nat_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_nat_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_6p0_nat_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_6p0_nat_gfC.log
	@mv -f ../testcases/nmos_6p0_nat_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-nmos_10p0_asym: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmos_10p0_asym pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_10p0_asym_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_10p0_asym_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_10p0_asym_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_10p0_asym_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmos_10p0_asym_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/nmos_10p0_asym_gfC.log
	@mv -f ../testcases/nmos_10p0_asym_pcells*.lyrdb $(run_folder)/mosfet_drc


.ONESHELL:
test-DRC-pmos_10p0_asym: Add_run-dir	
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmos_10p0_asym pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_10p0_asym_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_10p0_asym_gfA.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_10p0_asym_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_10p0_asym_gfB.log
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmos_10p0_asym_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/mosfet_drc/pmos_10p0_asym_gfC.log
	@mv -f ../testcases/pmos_10p0_asym_pcells*.lyrdb $(run_folder)/mosfet_drc


#=================================
# ------- test-DRC-MOSCAP --------
#=================================

.ONESHELL:
test-DRC-MOSCAP: test-DRC-nmoscap  test-DRC-pmoscap test-DRC-nmoscap_dw  test-DRC-pmoscap_dw  test-DRC-nmoscap_b  test-DRC-pmoscap

.ONESHELL:
test-DRC-nmoscap: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmoscap pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_gfC.log 
	@mv -f ../testcases/nmoscap_pcells*.lyrdb $(run_folder)/moscap_drc


.ONESHELL:
test-DRC-pmoscap: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmoscap pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_gfC.log 
	@mv -f ../testcases/pmoscap_pcells*.lyrdb $(run_folder)/moscap_drc


.ONESHELL:
test-DRC-nmoscap_dw: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmoscap_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_dw_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_dw_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_dw_gfC.log 
	@mv -f ../testcases/nmoscap_dw_pcells*.lyrdb $(run_folder)/moscap_drc


.ONESHELL:
test-DRC-pmoscap_dw: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmoscap_dw pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_dw_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_dw_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_dw_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_dw_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_dw_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_dw_gfC.log 
	@mv -f ../testcases/pmoscap_dw_pcells*.lyrdb $(run_folder)/moscap_drc


.ONESHELL:
test-DRC-nmoscap_b: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nmoscap_b pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_b_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_b_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_b_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_b_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nmoscap_b_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/nmoscap_b_gfC.log 
	@mv -f ../testcases/nmoscap_b_pcells*.lyrdb $(run_folder)/moscap_drc


.ONESHELL:
test-DRC-pmoscap_b: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pmoscap_b pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_b_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_b_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_b_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_b_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pmoscap_b_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/moscap_drc/pmoscap_b_gfC.log 
	@mv -f ../testcases/pmoscap_b_pcells*.lyrdb $(run_folder)/moscap_drc


#=================================
# --------- test-DRC-RES ---------
#=================================

.ONESHELL:
test-DRC-RES: test-DRC-nplus_u_res  test-DRC-nplus_s_res  test-DRC-pplus_u_res  test-DRC-pplus_s_res  test-DRC-npolyf_u_res  test-DRC-npolyf_s_res  test-DRC-ppolyf_u_res \
			  test-DRC-ppolyf_s_res  test-DRC-ppolyf_u_high_Rs_res  test-DRC-nwell_res  test-DRC-pwell_res  test-DRC-metal_res

.ONESHELL:
test-DRC-nplus_u_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nplus_u_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_u_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_u_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_u_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_u_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_u_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_u_res_gfC.log 
	@mv -f ../testcases/nplus_u_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-nplus_s_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nplus_s_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_s_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_s_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_s_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_s_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nplus_s_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nplus_s_res_gfC.log 
	@mv -f ../testcases/nplus_s_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-pplus_u_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pplus_u_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_u_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_u_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_u_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_u_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_u_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_u_res_gfC.log 
	@mv -f ../testcases/pplus_u_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-pplus_s_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pplus_s_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_s_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_s_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_s_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_s_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pplus_s_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pplus_s_res_gfC.log 
	@mv -f ../testcases/pplus_s_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-npolyf_u_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for npolyf_u_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_u_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_u_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_u_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_u_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_u_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_u_res_gfC.log 
	@mv -f ../testcases/npolyf_u_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-npolyf_s_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for npolyf_s_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_s_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_s_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_s_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_s_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/npolyf_s_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/npolyf_s_res_gfC.log 
	@mv -f ../testcases/npolyf_s_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-ppolyf_u_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for ppolyf_u_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_res_gfC.log 
	@mv -f ../testcases/ppolyf_u_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-ppolyf_s_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for ppolyf_s_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_s_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_s_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_s_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_s_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_s_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_s_res_gfC.log 
	@mv -f ../testcases/ppolyf_s_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-ppolyf_u_high_Rs_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for ppolyf_u_high_Rs_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_high_Rs_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_high_Rs_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_high_Rs_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_high_Rs_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/ppolyf_u_high_Rs_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/ppolyf_u_high_Rs_res_gfC.log 
	@mv -f ../testcases/ppolyf_u_high_Rs_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-nwell_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for nwell_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nwell_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nwell_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nwell_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nwell_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/nwell_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/nwell_res_gfC.log 
	@mv -f ../testcases/nwell_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-pwell_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for pwell_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pwell_resistor_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pwell_res_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pwell_resistor_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pwell_res_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/pwell_resistor_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/pwell_res_gfC.log 
	@mv -f ../testcases/pwell_resistor_pcells*.lyrdb $(run_folder)/res_drc


.ONESHELL:
test-DRC-metal_res: Add_run-dir
	@cd $(Testing_DIR)
	@echo "===== test-DRC for metal_res pcells ====="
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/metal_resistor_rm1_pcells.gds      --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/metal_res_rm1_gfC.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/metal_resistor_rm2_3_pcells.gds    --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/metal_res_rm2_3_gfC.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/metal_resistor_tm30k_pcells.gds    --gf180mcu=A --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/metal_res_tm30k_gfA.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/metal_resistor_tm11k_pcells.gds    --gf180mcu=B --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/metal_res_tm11k_gfB.log 
	@python3 $(PDK_ROOT)/$(PDK)/run_drc.py --path=../testcases/metal_resistor_tm9k_pcells.gds     --gf180mcu=C --antenna  --no_offgrid  |& tee $(run_folder)/res_drc/metal_res_tm9k_gfC.log 
	@mv -f ../testcases/metal_resistor_rm1_pcells*.lyrdb $(run_folder)/res_drc
	@mv -f ../testcases/metal_resistor_rm2_3_pcells*.lyrdb $(run_folder)/res_drc
	@mv -f ../testcases/metal_resistor_tm30k_pcells*.lyrdb $(run_folder)/res_drc
	@mv -f ../testcases/metal_resistor_tm11k_pcells*.lyrdb $(run_folder)/res_drc
	@mv -f ../testcases/metal_resistor_tm9k_pcells*.lyrdb $(run_folder)/res_drc


#==========================
# --------- HELP ----------
#==========================

# Help Target
help:
	@echo "\n ==== The following are some of the valid targets for this Makefile ====\n"
	@echo "... all                        (the default if no target is provided             )"
	@echo "... test-DRC-pcell             (To run DRC for on all pcells                     )"
	@echo "... test-DRC-bjt               (To run DRC for on bjt pcells                     )"
	@echo "... test-DRC-diode             (To run DRC for on diode pcells                   )"
	@echo "... test-DRC-MIM               (To run DRC for on MIM pcells                     )"
	@echo "... test-DRC-MOS               (To run DRC for on MOS pcells                     )"
	@echo "... test-DRC-MOSCAP            (To run DRC for on MOSCAP pcells                  )"
	@echo "... test-DRC-RES               (To run DRC for on RES pcells                     )"

