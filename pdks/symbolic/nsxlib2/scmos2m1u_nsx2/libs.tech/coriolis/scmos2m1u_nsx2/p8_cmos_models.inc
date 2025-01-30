**********************************************************************
* SPICE MOSFET Models used in Analog Integrated Circuit Design  by Johns &
* Martin 
* 
* Usage: 
*	.include cmos_models.inc
**********************************************************************

.MODEL nmos NMOS   LEVEL=8        TOX=1.8E-8       LD=0.08U 
+                  UO=500         VMAX=2.0E5       PHI=0.6        
+                  GAMMA=0.5      NSUB=2.5E16      VTO=0.8 
+                  NFS=8.2E11     CGSO=2.5E-10     CGBO=2.5E-10 
+                  CJSW=2.5E-10   CGDO=2.5E-10     MJ=0.5 
+                  CJ=2.5E-4      PB=0.9           IS=1.0E-16 
+                  JS=1.0E-4      KF=600E-27       AF=0.8
+                  RS=600           RD=600
+                  ETA=0.05       KAPPA=0.007      THETA=0.06
+                  XJ=2.7E-7        DELTA=0.7

.MODEL pmos PMOS   LEVEL=8        TOX=1.8E-8       LD=0.08U 
+                  UO=165         VMAX=2.7E5       PHI=0.80        
+                  GAMMA=0.75     NSUB=5.5E16      VTO=-0.9 
+                  NFS=7.6E11     CGSO=2.5E-10     CGBO=2.75E-10 
+                  CJSW=3.4E-10   CGDO=2.5E-10     MJ=0.5 
+                  CJ=3.7E-4      PB=0.8           IS=1.0E-16 
+                  JS=1.0E-4      KF=400E-27       AF=1.0
+                  RS=1200          RD=1200
+                  ETA=0.12       KAPPA=1.5        THETA=0.135
+                  XJ=2.3E-7        DELTA=0.3

