#!/usr/bin/env avt_shell

set VthHigh     0.8
set VthLow      0.2
set Slop        10e-12
set Temperature 27.0
set VddName     vdd
set VssName     vss
set vddvolt     1.8


#set techno      [lindex $argv 0]

set osdidll  {}
set spimodel {}
set MasterFF sff_m
set SlaveFF  sff_s
set FFCell   sff*

set idx 0

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
  -Cell -
  -C  {
     if {$idx < $argc} {
     set cell [lindex $argv $idx]
     incr idx
   } }
  -OsdiDll -
  -o  {
     if {$idx < $argc} {
     set osdidll [lindex $argv $idx]
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
     set spiceFormat [lindex $argv $idx]
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
  -MasterFlipFlop -
  -M {
     if {$idx < $argc} {
     set MasterFF [lindex $argv $idx]
     incr idx
   } }
  -SlaveFlipFlop - 
  -s {
     if {$idx < $argc} {
     set SlaveFF [lindex $argv $idx]
     incr idx
   } }
  -FlipFlopCell -
  -c {
     if {$idx < $argc} {
     set FFCell [lindex $argv $idx]
     incr idx
   } }
  default {
    puts [concat "Unknown parameter" $flag]
    exit 1
    }
 }
}


puts "Using technology:        <$spimodel>"
puts "Extracting boolean cell: <$cell.spi>"

if ([string match $FFCell $cell]) {
  puts "Cell is a D flip-flop."

  inf_SetFigureName   $cell
  inf_MarkSignal      $MasterFF "FLIPFLOP+MASTER"
  inf_MarkSignal      $SlaveFF SLAVE
}

avt_config   yagSetResetDetection yes
avt_config   simToolModel  $spiceFormat
foreach i $osdidll {
	avt_LoadFile $i osdi
}
foreach i $spimodel {
	avt_LoadFile $i spice
}
avt_config simVthHigh $VthHigh
avt_config simVthLow $VthLow
avt_config simSlope $Slop
avt_config simTemperature $Temperature
avt_config simPowerSupply $vddvolt
#avt_LoadFile $techno       spice
avt_LoadFile $cell.spi     spice
avt_config   avtVddName    $VddName
avt_config   avtVssName    $VssName
yagle $cell

