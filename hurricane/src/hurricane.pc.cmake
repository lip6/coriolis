prefix=@DEST_DIR@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${exec_prefix}/include
 
Name: hurricane
Description: Hurricane Libs
Version: 2.0.b
Libs: -L${libdir} -lanalogic -lhurricanefigs -lhurricane
Cflags: -I${includedir}/hurricane
