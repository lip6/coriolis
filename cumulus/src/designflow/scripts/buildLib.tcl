
set VthHigh     0.8
set VthLow      0.2
set Slop        10e-12
set Temperature 27.0
set VddName     vdd
set VssName     vss

set vddvolt     1.8
set osdidll     {}
set spimodel    {}

set MasterFF    sff_m
set SlaveFF     sff_s
set FFCell      sff*

set idx 0
while {$idx < $argc} {
  set flag [lindex $argv $idx]
  incr idx
  switch -exact -- $flag {
  -LibName -
  -L  {
     if {$idx < $argc} {
     set libName [lindex $argv $idx]
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
  -F  {
     if {$idx < $argc} {
     set spiceType [lindex $argv $idx]
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
 puts "Characterizing library:  <$libName>"

 avt_config   yagSetResetDetection yes
 avt_config   simToolModel  $spiceType
 foreach i $osdidll {
        puts "OSDI loading"
        puts $i
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
 avt_config   avtVddName    $VddName
 avt_config   avtVssName    $VssName

 set cells [glob *.spi]

 foreach cell $cells {
   set cellName [string map { .spi "" }  $cell]
 
   if ([string match ts_*  $cellName]) continue
   if ([string match nts_* $cellName]) continue
 
   avt_LoadFile $cell spice
   avt_config   tasBefig             yes
   # avt_config   tmaFunctionalityMode w
   avt_config   tmaDriveCapaout      yes
   avt_config   avtPowerCalculation  yes

   if ([string match $FFCell $cell]) {
     puts "Cell is a D flip-flop."

     inf_SetFigureName   $cell
     inf_MarkSignal      $MasterFF "FLIPFLOP+MASTER"
     inf_MarkSignal      $SlaveFF SLAVE
     create_clock -period 3000 ck
   }

   set fig      [hitas $cellName]
   set name     $cellName
   set beh_fig  [avt_LoadBehavior $cellName.vhd vhdl]
   set abs_fig  [tma_abstract $fig $beh_fig]
 
   lappend fig_list $abs_fig
   lappend beh_list $beh_fig
 }
 
 lib_drivefile $fig_list $beh_list "$libName.lib" max
