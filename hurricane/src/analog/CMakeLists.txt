# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<hurricane/src/analog>" -*-

#              include ( ${QT_USE_FILE} )

   include_directories ( ${HURRICANE_SOURCE_DIR}/src/hurricane
                         ${HURRICANE_SOURCE_DIR}/src/isobar
                         ${HURRICANE_SOURCE_DIR}/src/viewer
                         ${HURRICANE_SOURCE_DIR}/src/analog
                         ${HURRICANE_SOURCE_DIR}/src/configuration3
                         ${QtX_INCLUDE_DIRS}
                         ${Boost_INCLUDE_DIRS}
                         ${Python_INCLUDE_DIRS}
                       )

                   set( cpps        AnalogCellExtension.cpp
                                    Parameter.cpp
                                    CapacitiesParameter.cpp
                                    FormFactorParameter.cpp
                                    ChoiceParameter.cpp
                                    FloatParameter.cpp
                                    MatrixParameter.cpp
                                    MCheckBoxParameter.cpp
                                    SpinBoxParameter.cpp
                                    StepParameter.cpp
                                    StringParameter.cpp
                                    BJT.cpp
                                    BJTFamily.cpp
                                    BJTFamilyNames.cpp
                                    CapacitorFamily.cpp
                                    CapacitorFamilyNames.cpp
                                    Cascode.cpp
                                    ChoiceParameter.cpp
                                    Choices.cpp
                                    CommonDrain.cpp
                                    CommonGatePair.cpp
                                    CommonSourcePair.cpp
                                    CrossCoupledPair.cpp
                                    Device.cpp
                                    DifferentialPair.cpp
                                    LayoutGenerator.cpp
                                    LevelShifter.cpp
                                    MetaCapacitor.cpp
                                    MetaTransistor.cpp
                                    MultiCapacitor.cpp
                                    SimpleCurrentMirror.cpp
                                    Transistor.cpp
                                    TransistorFamily.cpp
                                    TransistorFamilyNames.cpp
                                    TransistorPair.cpp
                                    MultiCapacitor.cpp
				    MetaResistor.cpp
				    ResistorFamily.cpp
				    Resistor.cpp
                      )
                   set( pyCpps      PyAnalog.cpp
                                    PyCapacitorFamily.cpp
                                    PyCapacitorParameter.cpp
                                    PyCascode.cpp
                                    PyChoiceParameter.cpp
                                    PyCommonDrain.cpp
                                    PyCommonGatePair.cpp
                                    PyCommonSourcePair.cpp
                                    PyCrossCoupledPair.cpp
                                    PyDevice.cpp
                                    PyDifferentialPair.cpp
                                    PyFormFactorParameter.cpp
                                    PyLayoutGenerator.cpp
                                    PyLevelShifter.cpp
                                    PyMCheckBoxParameter.cpp
                                    PyMultiCapacitor.cpp
                                    PyMatrix.cpp
                                    PyParameter.cpp
                                    PySimpleCurrentMirror.cpp
                                    PySpinBoxParameter.cpp
                                    PyStepParameter.cpp
                                    PyStringParameter.cpp
                                    PyFloatParameter.cpp
                                    PyTransistor.cpp
                                    PyTransistorFamily.cpp
                                    PyTransistorPair.cpp
                                    PyMatrixParameter.cpp
                                    PyCapacitiesParameter.cpp
                                    PyMultiCapacitor.cpp
                                    PyResistorFamily.cpp
                                    PyResistor.cpp
                      )
                   set( includes    hurricane/analog/AnalogCellExtension.h
                                    hurricane/analog/BJTFamily.h
                                    hurricane/analog/BJTFamilyNames.h
                                    hurricane/analog/BJT.h
                                    hurricane/analog/CapacitorFamily.h
                                    hurricane/analog/CapacitorFamilyNames.h
                                    hurricane/analog/CapacitorParameter.h
                                    hurricane/analog/Cascode.h
                                    hurricane/analog/ChoiceParameter.h
                                    hurricane/analog/Choices.h
                                    hurricane/analog/CommonDrain.h
                                    hurricane/analog/CommonGatePair.h
                                    hurricane/analog/CommonSourcePair.h
                                    hurricane/analog/CrossCoupledPair.h
                                    hurricane/analog/Device.h
                                    hurricane/analog/DifferentialPair.h
                                    hurricane/analog/FormFactorParameter.h
                                    hurricane/analog/LayoutGenerator.h
                                    hurricane/analog/LevelShifter.h
                                    hurricane/analog/MCheckBoxParameter.h
                                    hurricane/analog/MetaCapacitor.h
                                    hurricane/analog/MetaTransistor.h
                                    hurricane/analog/MultiCapacitor.h
                                    hurricane/analog/Parameter.h
                                    hurricane/analog/SimpleCurrentMirror.h
                                    hurricane/analog/SpinBoxParameter.h
                                    hurricane/analog/StepParameter.h
                                    hurricane/analog/StringParameter.h
                                    hurricane/analog/FloatParameter.h
                                    hurricane/analog/TransistorFamily.h
                                    hurricane/analog/TransistorFamilyNames.h
                                    hurricane/analog/Transistor.h
                                    hurricane/analog/TransistorPair.h
                                    hurricane/analog/MatrixParameter.h
                                    hurricane/analog/CapacitiesParameter.h
                                    hurricane/analog/MultiCapacitor.h
                                    hurricane/analog/Matrix.h
				    hurricane/analog/MetaResistor.h
				    hurricane/analog/ResistorFamily.h
				    hurricane/analog/Resistor.h
                      )
                   set( pyIncludes  hurricane/analog/PyCapacitorFamily.h
                                    hurricane/analog/PyCapacitorParameter.h
                                    hurricane/analog/PyCascode.h
                                    hurricane/analog/PyChoiceParameter.h
                                    hurricane/analog/PyCommonDrain.h
                                    hurricane/analog/PyCommonGatePair.h
                                    hurricane/analog/PyCommonSourcePair.h
                                    hurricane/analog/PyCrossCoupledPair.h
                                    hurricane/analog/PyDevice.h
                                    hurricane/analog/PyDifferentialPair.h
                                    hurricane/analog/PyFormFactorParameter.h
                                    hurricane/analog/PyLayoutGenerator.h
                                    hurricane/analog/PyLevelShifter.h
                                    hurricane/analog/PyMCheckBoxParameter.h
                                    hurricane/analog/PyMultiCapacitor.h
                                    hurricane/analog/PyMatrix.h
                                    hurricane/analog/PyParameter.h
                                    hurricane/analog/PySimpleCurrentMirror.h
                                    hurricane/analog/PySpinBoxParameter.h
                                    hurricane/analog/PyStepParameter.h
                                    hurricane/analog/PyStringParameter.h
                                    hurricane/analog/PyFloatParameter.h
                                    hurricane/analog/PyTransistorFamily.h
                                    hurricane/analog/PyTransistor.h
                                    hurricane/analog/PyTransistorPair.h
                                    hurricane/analog/PyMatrixParameter.h
                                    hurricane/analog/PyCapacitiesParameter.h
                                    hurricane/analog/PyResistorFamily.h
                                    hurricane/analog/PyResistor.h
                      )

                   set( depLibs     viewer
	                            isobar
	                            hurricane
                                    ${UTILITIES_LIBRARY}
                                    ${QtX_LIBRARIES}
                      )

           add_library( analog      ${cpps} ${pyCpps} )
 set_target_properties( analog      PROPERTIES VERSION 1.0 SOVERSION 1 )
 target_link_libraries( analog      ${depLibs} )
 set_has_unresolved_symbols( analog )

     add_python_module( "${pyCpps}"
                        "${pyIncludes}"
                        "Do_not_generate_C_library"
                        Analog
                        "analog;${depLibs}"
                        include/coriolis2/hurricane/analog
                      )

               install( FILES       ${includes}    DESTINATION include/coriolis2/hurricane/analog ) 
               install( TARGETS     analog         DESTINATION lib${LIB_SUFFIX} )
