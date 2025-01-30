# MOSIS_SCMOS_for_KLayout

## KLayout (version 0.27 or higher) technology files for MOSIS SCMOS generic process

 * MOSIS_SCMOS.lyt   : technology and connections description  
 * MOSIS_SCMOS.lyp   : layers color and shape description  
 * drc/drc_MOSIS_SCMOS.lydrc : DRC script  

Within KLayout, create the technolgy MOSIS_SCMOS by the menu : **[Tools]-[Manage Technologies]**  
Then press + at the bottom left, you will add a new technology that you can call : _MOSIS_SCMOS_

Then, you can copy the file **MOSIS_SCMOS.lyp**, **MOSIS_SCMOS.lyt** and the 2 directories **drc** and **lvs** of that repository in your directory :  
`$HOME/.klayout/tech/MOSIS_SCMOS  (under Linux)`  
`#HOMEDATA#/klayout/tech/MOSIS_SCMOS  (under Windows)`  

In the the drc/drc_MOSIS_SCMOS.lydrc , set the 4 parameters, by default I set them to :
 * LAMBDA = 0.2
 * SUBM = true
 * DEEP = false
 * NBR_OF_METALS = 6
