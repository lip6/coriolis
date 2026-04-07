import os
import glob
import re
import sys
import time
import datetime
import math
import argparse
import subprocess
###############################################################################
#get from vst file all top level module signals (name, size and index
# of the first bit)
#vdd and vss signals are not considered
def get_signals_hurricane(entity):
 from coriolis.designflow.technos import setupSky130_c4m
 from coriolis import CRL
 setupSky130_c4m( '../../..', '../../../pdkmaster/C4M.Sky130' )
 af = CRL.AllianceFramework.get()
 cell  = CRL.Blif.load(f'{entity}.blif')
 #get supply pins not used in placement
 supply_pins = [j.getName() for j in cell.getSupplyNets()]
 vectors = {}
 for i in cell.getExternalNets():
     #vector_name
     a=i.getName()
     vector = a.split('(')[0]
     if vector in vectors:
      #vector size
      vectors[vector][1] += 1
      #first bit of vector, it is 0 in most cases
      vectors[vector][2]=min(vectors[vector][2],int(a.split(')')[0].split('(')[1]))
     else:
         supply = False
         for s in supply_pins:
             if vector == s:
                 supply = True
         if supply == False:
             if '(' in a:
              bit = int(a.split(')')[0].split('(')[1])
             else: 
              bit = 0   
             vectors[vector] = [vector.lower(),1,bit]
 signals_sorted={}
 #In order to have a dictionnary with a number as key
 #usefull for sorting signals in other functions
 for i, key in enumerate(vectors):
  signals_sorted[i] = vectors[key]
 return signals_sorted
def get_signals(file,entity):
 # get all the signals --> lines between 'entity' and end 'entity'
 command=f"awk -F: '/entity {entity}/,/end {entity}/ {{print $0}}' {file}"
 l= subprocess.getoutput(command)
 lines = l.split('\n')
 all_signals = {}
 signals_index = 0
 for line in lines:
    # Delete useless spacing
    cleaned_line = line.strip()
    words = cleaned_line.split()
    if ":" in words:
        index = words.index(":")
        signal_name = words[index - 1]
        if (words[index+2]=='bit'):
            size  = 1
            end   = 0
            start = 0
        else:
        #vector of more than one bit
        #works for little endian representation
            end   = int(words[index+2].split('(')[1])
            start = int(words[index+4].split(')')[0])
            size   = end-start+1
        #if (('vss' and 'in' and 'bit') or ('vdd' and 'in' and 'bit')) not in words:    
        all_signals[signals_index]= [signal_name,size,start]
        signals_index = signals_index+1
 return all_signals        

#Given the list of connectors, it outputs the number of them.
def number_of_connectors(L):
    return len(L)
#It outputs the total number of bits for connectors
def number_of_bits(D):
    s= 0
    for i in range (number_of_connectors(D)):
        s=s+D[i][1]
    return s

#sorts indexs from the highest number of bits to the lower
#It takes as input a dictionnary containing the signals 
#(name, size and index) of the first bit)
def order_of_placement(dico):
    N = number_of_connectors(dico)
    L=[]
    sorted_indexes=[]
    for i in range(N):
        L.append(dico[i][1])
        sorted_indexes.append(i)
    #sorts by insertion the connectors indexes (sorted_indexes[n]) 
    #according to the size of the connector (L)    
    for n in range(1,N):
        cle = L[n]
        tmp = sorted_indexes[n]
        j = n-1
        while j>=0 and L[j] < cle:
            L[j+1] = L[j]
            sorted_indexes[j+1]=sorted_indexes[j]
            j = j-1
        L[j+1] = cle
        sorted_indexes[j+1]=tmp
    return sorted_indexes 


#Outputs the number_of_connectors of each side and the spacing between connectors
#Horizontal and veritcal sizes are in µm
def connectors_number_per_side(horizontal_size,vertical_size,horizontal_pitch,vertical_pitch,N):
    # we take a marge of one pitch at each corner if wanted
    north_side_capacity = horizontal_size# -(2*horizontal_pitch)
    south_side_capacity = horizontal_size# -(2*horizontal_pitch)
    east_side_capacity  = vertical_size  # -(2*vertical_pitch)
    west_side_capacity  = vertical_size  # -(2*vertical_pitch)
    percentage_of_vertical_pins = (north_side_capacity/(north_side_capacity+west_side_capacity))
    number_of_pins_in_vertical = int((percentage_of_vertical_pins * N)/2)
    north_side_pins_number = south_side_pins_number = number_of_pins_in_vertical
    east_side_pins_number  = int((N-(2*number_of_pins_in_vertical))/2)
    west_side_pins_number  = N - north_side_pins_number - south_side_pins_number - east_side_pins_number
    north_spacing = int(north_side_capacity /(north_side_pins_number * horizontal_pitch))
    south_spacing = int(south_side_capacity /(south_side_pins_number * horizontal_pitch))
    east_spacing  = int(east_side_capacity  /(east_side_pins_number  * vertical_pitch)  )
    west_spacing  = int(west_side_capacity  /(west_side_pins_number  * vertical_pitch)  )
    sides_connectors_number = [west_side_pins_number, north_side_pins_number, east_side_pins_number, south_side_pins_number]
    sides_connectors_spacing= [west_spacing,north_spacing,east_spacing,south_spacing]
    #Order: West, North, East, South
    return sides_connectors_number,sides_connectors_spacing

#check according to the size if the connector is a bit or a vector of more than 1bit
def vector_or_bit(i):
    if i == 1:
        return 'bit'
    else:
        return 'vector'

#Define which connector is placed in which size in order to place uniformely the connectors between the 4 sides
#The inputs are a dictionary containing signals from vst, sides and pitches sizes
def signals_per_side(dico,horizontal_size,vertical_size,horizontal_pitch,vertical_pitch):
  N = number_of_bits(dico)
  sorted_indexes_dico =   order_of_placement(dico)
  sides_connectors_number = connectors_number_per_side(horizontal_size,vertical_size,horizontal_pitch,vertical_pitch,N)[0]
  sides_connectors_pitches = connectors_number_per_side(horizontal_size,vertical_size,horizontal_pitch,vertical_pitch,N)[1]
  north=[]
  south=[]
  east =[]
  west =[]
  north_pin_placed = 0
  south_pin_placed = 0
  east_pin_placed  = 0
  west_pin_placed  = 0
  i                = 0
  bits_left        = 0
  last_bit_placed  = 0
  while west_pin_placed < sides_connectors_number[0]:
          name_of_vector_placed,size_placed_vector,start_b = dico[sorted_indexes_dico[i]]
          bits_placed_number = min(sides_connectors_number[0]-west_pin_placed,dico[sorted_indexes_dico[i]][1])
          west.append([name_of_vector_placed , start_b,start_b+bits_placed_number-1,vector_or_bit(size_placed_vector)])
          west_pin_placed += bits_placed_number
          last_bit_placed = bits_placed_number -1
          if bits_placed_number == dico[sorted_indexes_dico[i]][1]:
            #all the bits of the vector are placed  
            i=i+1
            bits_left        = 0
          else:
            #there is no enough space in this side to place of all vector bits
            #the missing bits will be placed on the other sides starting by the north
            bits_left = dico[sorted_indexes_dico[i]][1]- bits_placed_number

  while north_pin_placed < sides_connectors_number[1]:
          name_of_vector_placed,size_placed_vector,start_b = dico[sorted_indexes_dico[i]]
          if bits_left != 0:
          #place the last west vector missing bits (if they are not all placed)     
           bits_placed_number = min(sides_connectors_number[1],bits_left)  
           north.append([name_of_vector_placed ,start_b+last_bit_placed +1 ,start_b+last_bit_placed+ bits_placed_number,vector_or_bit(size_placed_vector)])
           north_pin_placed += bits_placed_number
           last_bit_placed=  last_bit_placed+bits_placed_number
           if bits_placed_number == bits_left:
            #all the bits of the last west vector are placed  
            i=i+1
            bits_left=0
           else:
            bits_left = bits_left- bits_placed_number
            #not all bits are placed, missing ones will be placed on the other sides
          else:
          #all bits from vectors in the west are placed, we will place new vectors in this side  
           bits_placed_number = min(sides_connectors_number[1]-north_pin_placed,dico[sorted_indexes_dico[i]][1])
           north.append([name_of_vector_placed , start_b, start_b+bits_placed_number-1,vector_or_bit(size_placed_vector)])
           north_pin_placed += bits_placed_number
           last_bit_placed = bits_placed_number -1
           if bits_placed_number == dico[sorted_indexes_dico[i]][1]:
            #all the bits of the vector are placed  
            i=i+1
            bits_left=0
           else:
            #missing bits will be placed in the next side   
            bits_left = dico[sorted_indexes_dico[i]][1]- bits_placed_number

  while east_pin_placed < sides_connectors_number[2]:
          name_of_vector_placed,size_placed_vector,start_b = dico[sorted_indexes_dico[i]]
          if bits_left != 0:
           bits_placed_number = min(sides_connectors_number[2],bits_left)  
           east.append([name_of_vector_placed ,start_b+last_bit_placed +1 ,start_b+last_bit_placed+ bits_placed_number,vector_or_bit(size_placed_vector)])
           east_pin_placed += bits_placed_number
           last_bit_placed=  last_bit_placed+bits_placed_number
           if bits_placed_number == bits_left:
            #all the bits of the vector are placed  
            i=i+1
            bits_left=0
           else:
            bits_left = bits_left- bits_placed_number
          else:
           bits_placed_number = min(sides_connectors_number[2]-east_pin_placed,dico[sorted_indexes_dico[i]][1])
           east.append([name_of_vector_placed ,start_b, start_b+bits_placed_number-1,vector_or_bit(size_placed_vector)])
           east_pin_placed += bits_placed_number
           last_bit_placed = bits_placed_number -1
           if bits_placed_number == dico[sorted_indexes_dico[i]][1]:
            #all the bits of the vector are placed  
            i=i+1
            bits_left=0
           else:
            bits_left = dico[sorted_indexes_dico[i]][1]- bits_placed_number

  while south_pin_placed < sides_connectors_number[3]:
          name_of_vector_placed,size_placed_vector,start_b = dico[sorted_indexes_dico[i]]
          if bits_left != 0:
           bits_placed_number = min(sides_connectors_number[3],bits_left)  
           south.append([name_of_vector_placed ,start_b+last_bit_placed +1 ,start_b+last_bit_placed+ bits_placed_number,vector_or_bit(size_placed_vector)])
           south_pin_placed += bits_placed_number
           last_bit_placed=  last_bit_placed+bits_placed_number
           if bits_placed_number == bits_left:
            #all the bits of the vector are placed  
            i=i+1
            bits_left=0
           else:
            bits_left = bits_left- bits_placed_number
          else:
           bits_placed_number = min(sides_connectors_number[3]-south_pin_placed,dico[sorted_indexes_dico[i]][1])
           south.append([name_of_vector_placed ,start_b,start_b+ bits_placed_number-1,vector_or_bit(size_placed_vector)])
           south_pin_placed += bits_placed_number
           last_bit_placed = bits_placed_number -1
           if bits_placed_number == dico[sorted_indexes_dico[i]][1]:
            #all the bits of the vector are placed  
            i=i+1
            bits_left=0
           else:
            bits_left = dico[sorted_indexes_dico[i]][1]- bits_placed_number
  #reverse the order of pins for south and east to place the pins starting by reverse          
  east.reverse()          
  south.reverse()          
  return sides_connectors_pitches,[west,north,east,south]
    
#same function than before but written as a function to be called
def generate_ioPinsSpec_list(pitch_id,dico,horizontal_size,vertical_size,horizontal_pitch,vertical_pitch):
    spacings,connectors =  signals_per_side(dico,horizontal_size,vertical_size,horizontal_pitch,vertical_pitch)
    print(horizontal_size)
    ioPinsSpec = []
    side=['WEST','NORTH','EAST','SOUTH']
    #pitch_id=['m1pitch','m2pitch','m1pitch','m2pitch']
    #pitch_id=['1','1','1','1']
    for s in range (4):
     pitch_shift =0   
     side_name =   f'IoPin.{side[s]} |IoPin.A_BEGIN' 
     for i in range (len(connectors[s])):
        if  connectors[s][i][2] != 0: ##one bit to place
            spacing_between_bits = f'{spacings[s]}*{pitch_id[s]}'
        else:
            spacing_between_bits = '0'
        if (side[s] =='WEST') or (side[s] =='NORTH') :     
         range_pin =  f'range({connectors[s][i][1]},{connectors[s][i][2]+1})'
        elif  (side[s] =='SOUTH') or (side[s] =='EAST'):     
         range_pin =  f'range({connectors[s][i][2]},{connectors[s][i][1]-1},-1)'
        else:
         print('ERROR in side name')

        if connectors[s][i][3] == 'bit':
            name =f'{connectors[s][i][0]}'
        elif  connectors[s][i][3] == 'vector':
            name =f'{connectors[s][i][0]}'+'({})'

        if i==0:
            #find a multiple of pitch close spacing/2
            start = f'{int((spacings[s])/2)}*{pitch_id[s]}'
            pitch_shift=0
        else:
           number_of_bits_placed_per_pin_in_side= f'{connectors[s][i-1][2]+1-connectors[s][i-1][1]}'
           pitch_shift=int (pitch_shift)+ int(number_of_bits_placed_per_pin_in_side)
           start =f'{pitch_shift}*{spacings[s]}*{pitch_id[s]}+ {int(spacings[s]/2)}*{pitch_id[s]}'
        ioPinsSpec.append((side_name, name,start,spacing_between_bits,range_pin))


    return ioPinsSpec   


#function that will be used to write ioPinsSpec in doDesign.py from template_doDesign.py
def write_connectors_in_file(filename, expression_to_detect, expression_to_write):
    file = open('./init_template_doDesign.py', 'r')
    lines = file.readlines()
    file.close()

    for i, line in enumerate(lines):
        if expression_to_detect in line:
            indentation = line.split(expression_to_detect)[0]
            lines.insert(i + 1, indentation+expression_to_write + '\n')
            break

    file = open(filename, 'w')
    file.writelines(lines)
    file.close()
    return
#replace the dumy entity by the real enity name
def replace_dummy(filename,real_name):
    with open(filename, 'r') as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        lines[i] = line.replace('dummy', real_name)

    with open(filename, 'w') as file:
        file.writelines(lines)
#get the area from the file
#def get_area_in_units(filename):
#    with open(filename, 'r') as file:
#        lines = file.readlines()
#
#    for i, line in enumerate(lines):
#        if 'conf.coreSize' in line:
#            horizental = eval(line.split('u(')[1].split(')')[0])
#            vertical   = eval(line.split(',')[1].split('u(')[1].split(')')[0])
#    return horizental,vertical        
#replace the area (horizontal, vertical) name in the designfile            
def replace_area(filename,h,v):
    with open(filename, 'r') as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        if 'conf.coreSize' in line:
         indentation = lines[i-1].split('conf')[0]
         lines[i] = f'{indentation}conf.coreSize            = ( u( {h}), u( {v}) )\n'
    with open(filename, 'w') as file:
        file.writelines(lines)
    return
#desactivate the htree for combinational block and set the right name of clk
#and reset for the htree configuration
def htree_configuration(filename,comb,clk_name='clk',reset_name='reset'):
    file = open(filename, 'r')
    lines = file.readlines()
    file.close()
    for i, line in enumerate(lines):
         if 'HTree' in line:
             if 'clk_pin' in line:
              lines[i] = line.replace('clk_pin', clk_name)
             elif 'reset_pin' in line : 
              lines[i] = line.replace('reset_pin', reset_name)
         if comb ==1:
            if 'HTree' in line:
                 lines[i-1] = '#'+ lines[i-1]
                 lines[i] = '#'+ lines[i]
    file = open(filename, 'w')
    file.writelines(lines)
    file.close()
    return
#combine functions to generate the doDesign.py file with connectors placed uniformely
def generate_doDesign_file(entity,all_signals,horizontal_area,vertical_area,horizontal_pitch,vertical_pitch,comb,clk_name='clk',reset_name='reset'):
 test = generate_ioPinsSpec(all_signals,horizontal_area,vertical_area,horizontal_pitch,vertical_pitch)
 write_connectors_in_file('./doDesign.py','#connectors placement in block design',test)
 replace_dummy('./doDesign.py',entity)
 replace_area('doDesign.py',horizontal_area,vertical_area)
 htree_configuration('doDesign.py',comb,clk_name,reset_name)
 return
#logic synthesis and b2v to generate the vst file used to get signals
def synthesis_and_initialize(entity):
     cmd_clean_b2v=  '../../../bin/crlenv.py doit clean_flow b2v >debug.txt'
     os.system(cmd_clean_b2v)
     #if the same net is present in two different outputs it generate a bug in b2v
     #it is solved by this rewrite_blif.py
     ###########if multioutput#####################
    # cmd_change_b2v=  'python rewrite_blif.py'
    # os.system(cmd_change_b2v)
    # cmd_b2v=  '../../../bin/crlenv.py doit b2v >debug.txt'
    # os.system(cmd_b2v)
     ##############################################
     return  get_signals_hurricane(entity)
#generate the dodo.py file
def generate_dodo_file(entity,comb):
    file = open('./template_dodo.py', 'r')
    lines = file.readlines()
    file.close()

    for i, line in enumerate(lines):
        if 'dummy' in line:
            lines[i] = line.replace('dummy', entity)
        if (('_cts' in line) and (comb == 1)):
            lines[i] = lines[i].replace('_cts', '')
    file = open('./dodo.py', 'w')
    file.writelines(lines)
    file.close()
    return
#define a routine that initialize, generate the doDesign.py and dodo.py file  
def routine(file,entity,h_area,v_area,horizontal_pitch,vertical_pitch,comb,clk_name='clk',reset_name='reset'):

 generate_dodo_file(entity,comb)
 cmd = 'cp init_template_doDesign.py doDesign.py'
 os.system (cmd)
 all_signals=synthesis_and_initialize(entity)
 generate_doDesign_file(entity,all_signals,h_area,v_area,horizontal_pitch,vertical_pitch,comb,clk_name,reset_name)
 cmd_pnr=  f'../../../bin/crlenv.py doit pnr >debug_{entity}.txt'
 os.system(cmd_pnr)

