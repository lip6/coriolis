#!/usr/bin/env python3

import sys

def flush ():
    sys.stdout.flush()
    sys.stderr.flush()
    

import Cfg

print( "" )
print( "Test 7" )
print( "========================================" )
print( type(Cfg) )
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "dir(Cfg) first test" )
for item in dir(Cfg):
    print( item )
    flush()
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "Cfg.__dict__" )
print( Cfg.__dict__ )
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "str(Cfg)" )
print( str(Cfg) )
flush()
print( "repr(Cfg)" )
print( repr(Cfg) )
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "dir(Cfg), second test" )
for item in dir(Cfg):
    print( item )
    flush()
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "Cfg.__dict__" )
print( Cfg.__dict__ )
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "dir(Cfg.parameter)" )
print( type(Cfg.Parameter) )
for item in dir(Cfg.Parameter):
    print( item )
    flush()
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "dir(Cfg), third test" )
for item in dir(Cfg):
    print( item )
    flush()
flush()
print( Cfg.__dict__ )

print( "" )
print( "Test 1" )
print( "========================================" )
cfg = Cfg.Configuration.get()
p1 = cfg.addParameter( "test.param1", Cfg.Parameter.Type.String, "my_value_1", Cfg.Parameter.Priority.ApplicationBuiltin )
print( 'p1=', p1 )
print( '(before) p1.flags=', p1.flags )
p1.flags = Cfg.Parameter.Flags.HasMin|Cfg.Parameter.Flags.HasMax
print( '(after) p1.flags=', p1.flags )

print( "" )
print( "Test 2" )
print( "========================================" )
p2 = cfg.addParameter( "test.param2", Cfg.Parameter.Type.String, "my_value_2" )
print( p2 )
dir(Cfg)

cfg.pushDefaultPriority( Cfg.Parameter.Priority.ConfigurationFile )
print( "priority=%d" % cfg.getDefaultPriority()  )
flush()

print( "" )
print( "Test 3" )
print( "========================================" )
hasP1 = Cfg.hasParameter( 'test.param1' )
print( 'hasP1=%s' % hasP1  )
hasP3 = Cfg.hasParameter( 'test.param3' )
print( 'hasP3=%s' % hasP3  )
dir(Cfg.Parameter)
flush()

print( "" )
print( "Test 4" )
print( "========================================" )
try:
    p3 = cfg.addParameter( "test.param3", Cfg.Parameter.Type.String )
except Exception as e:
    print( e )
dir(Cfg.Parameter)
flush()

print( "" )
print( "Test 5" )
print( "========================================" )
try:
    p4 = cfg.addParameter( 1, Cfg.Parameter.Type.String, "my_value4" )
except Exception as e:
    print( e )
dir(Cfg.Parameter)
flush()

print( "" )
print( "Test 6" )
print( "========================================" )
pX = Cfg.getParamString( 'test.param1' )
print( 'test.param1=%s' % pX )
print( 'test.param1.isFile(): %s' % pX.isFile() )
dir(Cfg.Parameter)
flush()

print( "" )
print( "Test 8" )
print( "========================================" )
master = cfg.addParameter( "test.master", Cfg.Parameter.Type.String, "master value" )
slave1 = cfg.addParameter( "test.slave1", Cfg.Parameter.Type.String, "slave1 value" )
slave2 = cfg.addParameter( "test.slave2", Cfg.Parameter.Type.String, "slave2 value" )
slave3 = cfg.addParameter( "test.slave3", Cfg.Parameter.Type.String, "slave3 value" )
master.addSlave( 'test.slave1' )
master.addSlave( 'test.slave2' )
master.addSlave( 'test.slave3' )
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "Walkthrough the vector using indexes" )
slaves = master.getSlaves()
for i in range(len(slaves)):
    print( '[%d] %s' % (i,slaves[i]) )
    flush()
flush()
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "Walkthrough the vector using iterator" )
for slave in slaves:
    print( '===> %s' % slave )
    flush()
flush()
print( '"test.slave2" in slaves = {}'.format('test.slave2' in slaves) )
print( '"test.slaveX" in slaves = {}'.format('test.slaveX' in slaves) )

print( "" )
print( "Test 9" )
print( "========================================" )
testDict = { 'machin':1, 'truc':2 }
for key in testDict:
    print( '==>', key )
    flush()
flush()
print( 'has key "machin":{}'.format('machin' in testDict) )
params = cfg.getParameters()
print( 'params["test.master"] = %s' % params['test.master'] )
try:
    print( 'params.has_key("test.master") = {}'.format('test.master' in params) )
   #print( 'params.has_key("test.master") = {}'.format(params.has_key('test.master') ))
    print( 'params.has_key("test.slave6") = {}'.format('test.slave6' in params) )
    print( 'params["test.slave6"] = %s' % params['test.slave6'] )
except KeyError as ke:
    print( ke )
print( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" )
print( "Walkthrough the map using iterator" )
for k, v in params:
    print( '===> %s %s' % (k,v)  )
    flush()
flush()

print( "" )
print( "Test 10" )
print( "========================================" )
print( 'getParamString("test.master")=', Cfg.getParamString( "test.master" ) )
flush()

pint = cfg.addParameter( "test.paramInt", Cfg.Parameter.Type.Int, "12" )
print( 'pint=', pint )
flush()

pdouble = cfg.addParameter( "test.paramDouble", Cfg.Parameter.Type.Double, "3.14" )
print( 'pdouble=', pdouble )
flush()
