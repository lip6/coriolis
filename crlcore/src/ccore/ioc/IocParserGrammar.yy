%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/Pin.h"
#include  "hurricane/Names.h"
using namespace Hurricane;

#include "crlcore/AllianceFramework.h"
#include "crlcore/LefDefExtension.h"
#include "crlcore/NetExtension.h"
using namespace CRL;
#include "crlcore/Ioc.h"

#define  yyin    Iocin
#define  yytext  Ioctext

extern char    *yytext;

extern int yyerror(char *str);
extern int yylex();

enum Orientation {T=0, B=1, R=2, L=3, I=4};
static Orientation orientation;
static bool topUsed = false, bottomUsed = false, rightUsed = false, leftUsed = false, ignoreUsed = false;
static map<Name, Net*> pinsMap;

%}

%union {
char *text;
unsigned value;
}

%token IGNORE TOP BOTTOM LEFT RIGHT IOPIN SPACE PAROUV PARFER PTVIRG
%token <text> IOCID1
%token <text> IOCID2
%token <value> IOCVALUE
%start file

%%
file :
	| orientation PAROUV expr PARFER file
	;

orientation : 	  TOP {
			if (topUsed)
		  	  yyerror("on ioc file : TOP declared twice");
                        else
                        {
                          orientation = T;
                          topUsed = true;
                        }
		      }
		| BOTTOM {
			    if (bottomUsed)
		  	      yyerror("on ioc file : BOTTOM declared twice");
                            else
                            {
                              orientation = B;
                              bottomUsed = true;
                            }
			 }
		| RIGHT	{
                          if (rightUsed)
		  	    yyerror("on ioc file : RIGHT declared twice");
                          else
                          {
                            orientation = R;
                            rightUsed = true;
                          }
			}
		| LEFT {
			 if (leftUsed)
		  	   yyerror("on ioc file : LEFT declared twice");
                         else
                         {
                           orientation = L;
                           leftUsed = true;
                         }
		       }
		| IGNORE {
			   if (ignoreUsed)
		  	     yyerror("on ioc file : IGNORE declared twice");
                           else
                           {
                             orientation = I;
                             ignoreUsed = true;
                           }
                         }
		;

expr : 
	| iopin expr
	| space expr
	;


iopin : PAROUV IOPIN iopin1 PARFER PTVIRG
      ;

space : SPACE IOCVALUE PTVIRG { throw Error("SPACES are not implemented");}
      ;

iopin1 :   IOCID1{
                   string  stringName ($1);
                   if ((stringName.size() > 2) && (stringName.substr(stringName.size()-2) == ".0")) {
                       stringName.erase(stringName.size()-2);
                   }
                   map<Name, Net*>::iterator pmit = pinsMap.find(Name(stringName));
                   if (pmit == pinsMap.end())
                   {
                     cerr << stringName << " " << endl;
                     throw Error("Unknown Pin Name");
                   }
                   Layer* layer = DataBase::getDB()->getTechnology()->getLayer(AllianceFramework::get()->getDefaultCGPinLayerName());
                   Pin::AccessDirection accessDirection;
                   Name pinName = Name(stringName);
                   Net* net = pmit->second;
                   Cell* cell = net->getCell();
                   switch (orientation)
                   {
                     case T: accessDirection = Pin::AccessDirection::NORTH; LefDefExtension::getNorthPinIocOrder(cell,true).push_back(pinName); break;
                     case B: accessDirection = Pin::AccessDirection::SOUTH; LefDefExtension::getSouthPinIocOrder(cell,true).push_back(pinName); break;
                     case R: accessDirection = Pin::AccessDirection::EAST;  LefDefExtension::getEastPinIocOrder(cell,true).push_back(pinName); break;
                     case L: accessDirection = Pin::AccessDirection::WEST;  LefDefExtension::getWestPinIocOrder(cell,true).push_back(pinName); break;
                     case I: accessDirection = Pin::AccessDirection::UNDEFINED; LefDefExtension::getUndefinedPinIoc(cell,true).push_back(pinName); break;
                     default: throw Error("Unrecognized Orientation while parsing ioc file...");
                   }
                   (void)Pin::create(net, pinName, accessDirection,Pin::PlacementStatus::PLACED, layer, 0, 0);
		}
	| IOCID2 {
                   string  stringName ($1);
                   if ((stringName.size() > 2) && (stringName.substr(stringName.size()-2) == ".0")) {
                       stringName.erase(stringName.size()-2);
                   }
                  map<Name, Net*>::iterator pmit = pinsMap.find(stringName);
                  if (pmit == pinsMap.end())
                  {
                    cerr << stringName << " " << endl;
                    throw Error("Unknown Pin Name");
                  }
                  Layer* layer = DataBase::getDB()->getTechnology()->getLayer("CALU2");
                  Pin::AccessDirection accessDirection;
                  Name pinName = Name(stringName);
                  Net* net = pmit->second;
                  Cell* cell = net->getCell();
                  switch (orientation)
                  {
                    case T: accessDirection = Pin::AccessDirection::NORTH; LefDefExtension::getNorthPinIocOrder(cell,true).push_back(pinName); break;
                    case B: accessDirection = Pin::AccessDirection::SOUTH; LefDefExtension::getSouthPinIocOrder(cell,true).push_back(pinName); break;
                    case R: accessDirection = Pin::AccessDirection::EAST;  LefDefExtension::getEastPinIocOrder(cell,true).push_back(pinName); break;
                    case L: accessDirection = Pin::AccessDirection::WEST;  LefDefExtension::getWestPinIocOrder(cell,true).push_back(pinName); break;
                    case I: accessDirection = Pin::AccessDirection::UNDEFINED; LefDefExtension::getUndefinedPinIoc(cell,true).push_back(pinName); break;
                    default: throw Error("Unrecognized Orientation while parsing ioc file...");
                  }
                  (void)Pin::create(net, pinName, accessDirection, Pin::PlacementStatus::PLACED, layer, 0, 0);
		}
	;
%%

int yyerror (char *str)
{
    fflush(stdout);
    fprintf(stderr, "error parsing the ioc file: %s before %s\n", str, yytext);
    exit(1);
}

void IocParser(Cell* cell, const char* file)
{
  extern FILE *yyin;
  if (!cell)
    throw Error("NULL cell in IOC parser");
  if ( (yyin=fopen(file,"r")) )
  {
    for_each_net(net, cell->getExternalNets())
    {
      if (net->isGlobal())
        continue;
      const Name& pinNamePt = NetExtension::getPort(net);
      Name pinName;
      if (!pinNamePt.isEmpty())
        pinName = Name(pinNamePt);
      else
        pinName = net->getName();
      if (pinsMap.find(pinName) != pinsMap.end())
        throw Error("two connectors with the same name");
      pinsMap[pinName] = net;
      end_for;
    }
    yyparse();
    fclose(yyin);
  }
  else
    throw Error("IOC file not present");
}
