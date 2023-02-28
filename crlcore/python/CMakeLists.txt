
 install( FILES   __init__.py                       DESTINATION ${Python_CORIOLISLIB} ) 

 install( FILES   helpers/__init__.py               DESTINATION ${Python_CORIOLISLIB}/helpers ) 
 install( FILES   helpers/io.py                     DESTINATION ${Python_CORIOLISLIB}/helpers ) 
 install( FILES   helpers/utils.py                  DESTINATION ${Python_CORIOLISLIB}/helpers ) 
 install( FILES   helpers/overlay.py                DESTINATION ${Python_CORIOLISLIB}/helpers ) 
 install( FILES   helpers/analogtechno.py           DESTINATION ${Python_CORIOLISLIB}/helpers ) 
 install( FILES   helpers/technology.py             DESTINATION ${Python_CORIOLISLIB}/helpers ) 

 install( FILES     technos/__init__.py             DESTINATION ${Python_CORIOLISLIB}/technos ) 
 install( DIRECTORY technos/common                  DESTINATION ${Python_CORIOLISLIB}/technos ) 
 install( DIRECTORY technos/symbolic                DESTINATION ${Python_CORIOLISLIB}/technos ) 
 install( DIRECTORY technos/node600                 DESTINATION ${Python_CORIOLISLIB}/technos ) 
 install( DIRECTORY technos/node180                 DESTINATION ${Python_CORIOLISLIB}/technos ) 
 install( DIRECTORY technos/node45                  DESTINATION ${Python_CORIOLISLIB}/technos ) 
