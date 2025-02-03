#!/usr/bin/env avt_shell

#############################################################
# Timing Database Generation                                #
#############################################################

set VthHigh 0.8
set VthLow  0.2
set Slop    10e-12
set Temperature 25.0
set VddName  vdd
set VssName  vss

set osdidll {}
set spimodel {}
set vddvolt 1.8

set idx 0
if {$argc == 0} {
    puts "calcCPath -Target taget_name -SpiceModel spice_model -SpiceType spice_type -VddVoltage vdd_voltage -ClockSignal clock_signal_name -VddName vdd_name -VssName vss_name -Temperature temperature -OsdiDll osdi_dllname"
    puts "if you need multiple spice models, put your models as a list \" model1 model2 ..\" it will feed to avt_LoadFile in the order"
    exit 1
}
while {$idx < $argc} {
  set flag [lindex $argv $idx]
  incr idx
  switch -exact -- $flag {
  -Target -
  -t  {
     if {$idx < $argc} {
     set target [lindex $argv $idx]
     incr idx
   } }
  -OsdiDll -
  -o  {
     if {$idx < $argc} {
     set odsidll [lindex $argv $idx]
     incr idx
   } }
  -SpiceModel -
  -m  {
     if {$idx < $argc} {
     set spimodel [lindex $argv $idx]
     incr idx
   } }
  -SpiceType -
  -p  {
     if {$idx < $argc} {
     set spitype [lindex $argv $idx]
     incr idx
   } }
  -VddVoltage -
  -v  {
     if {$idx < $argc} {
     set vddvolt [lindex $argv $idx]
     incr idx
   } }
  -ClockSignal -
  -c  {
     if {$idx < $argc} {
     set clksig [lindex $argv $idx]
     incr idx
   } }
  -VddName -
  -V  {
     if {$idx < $argc} {
     set VddName [lindex $argv $idx]
     incr idx
   } }
  -VssName -
  -S  {
     if {$idx < $argc} {
     set VssName [lindex $argv $idx]
     incr idx
   } }
  -Temperature -
  -T  {
     if {$idx < $argc} {
     set Temperature [lindex $argv $idx]
     incr idx
   } }
  default {
    puts "calcCPath -Target taget_name -SpiceModel spice_model -SpiceType spice_type -VddVoltage vdd_voltage -ClockSignal clock_signal_name -VddName vdd_name -VssName vss_name -Temperature temperature -OsdiDll osdi_dllname"
    exit 1
    }
 }
}

# General config
avt_config avtLibraryDirs .

avt_config tasGenerateConeFile yes
avt_config avtVerboseConeFile yes 
avt_config yagSetResetDetection yes 

avt_config simVthHigh $VthHigh
avt_config simVthLow $VthLow
avt_config simSlope $Slop
avt_config simTemperature $Temperature

avt_config simToolModel $spitype
avt_config tasGenerateDetailTimingFile yes
avt_config avtVddName $VddName
avt_config avtVssName $VssName
avt_config simPowerSupply $vddvolt

# Files of transistor model of the technology, that may require modifications
#
# nfet_01v8
foreach i $odsidll {
	avt_LoadFile $i osdi
}
foreach i $spimodel {
	avt_LoadFile $i spice
}
avt_LoadFile $target.spi spice

# File decribing the netlist with power supply and temperature

# Database generation
set fig [hitas $target]

set temp   [ttv_GetTimingFigureProperty $fig TEMP]
set supply [ttv_GetTimingFigureProperty $fig DEF_SUPPLY]

set slope  [ttv_GetTimingFigureProperty $fig DEF_SLOPE]
set cload   [ttv_GetTimingFigureProperty $fig DEF_LOAD]
set date   [ttv_GetTimingFigureProperty $fig DATE]

puts ""
puts "Power supply: [ttv_GetTimingFigureProperty $fig DEF_SUPPLY]V"
puts "Temperature: [ttv_GetTimingFigureProperty $fig TEMP] deg C"
#set sig [ttv_GetTimingSignal $fig out]
#set sign2 [ttv_GetTimingSignal $fig n2]
#set sign1 [ttv_GetTimingSignal $fig n1]

puts ""
#puts "out signal capacitance: [ttv_GetTimingSignalProperty $sig CAPA]F"
#puts "n2 signal capacitance: [ttv_GetTimingSignalProperty $sign2 CAPA]F"
#puts "n1 signal capacitance: [ttv_GetTimingSignalProperty $sign1 CAPA]F"


puts "input slope     : $slope s"
puts "generation date : $date"

puts "output load     : $cload"

#############################################################
# Stability Analysis                                        #
#############################################################

inf_SetFigureName $target

create_clock -name ck -period 10000 -waveform {5000 0} $clksig

set_input_delay -min 2000 -clock ck -clock_fall [all_inputs]
set_input_delay -max 3000 -clock ck -clock_fall [all_inputs]

# OCV
#inf_DefinePathDelayMargin any "*" 1 1e-9 datapath

set fig [ttv_LoadSpecifiedTimingFigure $target]
set clist [ttv_GetPaths $fig * * uu 5 critic path max]

ttv_DisplayPathListDetail [fopen $target.cpath.rep w]  $clist

set stbfig [stb $fig]

stb_DisplaySlackReport [fopen $target.slack.rep w] $stbfig * * ?? 10  all 10000
