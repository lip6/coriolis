/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     Ampersand = 258,
     Apostrophe = 259,
     LeftParen = 260,
     RightParen = 261,
     DoubleStar = 262,
     Star = 263,
     Plus = 264,
     Comma = 265,
     Minus = 266,
     VarAsgn = 267,
     Colon = 268,
     Semicolon = 269,
     _LESym = 270,
     _Box = 271,
     _LTSym = 272,
     Arrow = 273,
     _EQSym = 274,
     _GESym = 275,
     _GTSym = 276,
     Bar = 277,
     _NESym = 278,
     Dot = 279,
     Slash = 280,
     Identifier = 281,
     DecimalInt = 282,
     DecimalReal = 283,
     AbstractLit = 284,
     BasedInt = 285,
     BasedReal = 286,
     CharacterLit = 287,
     StringLit = 288,
     BitStringLit = 289,
     ABS = 290,
     ACCESS = 291,
     AFTER = 292,
     ALIAS = 293,
     ALL = 294,
     tok_AND = 295,
     ARCHITECTURE = 296,
     ARRAY = 297,
     ARG = 298,
     ASSERT = 299,
     ATTRIBUTE = 300,
     _BEGIN = 301,
     BIT = 302,
     BIT_VECTOR = 303,
     BLOCK = 304,
     BODY = 305,
     BUFFER = 306,
     BUS = 307,
     CASE = 308,
     Y_CLOCK = 309,
     COMPONENT = 310,
     CONFIGURATION = 311,
     CONSTANT = 312,
     CONVERT = 313,
     DISCONNECT = 314,
     DOWNTO = 315,
     ELSE = 316,
     ELSIF = 317,
     _END = 318,
     ENTITY = 319,
     ERROR = 320,
     _EXIT = 321,
     _FILE = 322,
     FOR = 323,
     FUNCTION = 324,
     GENERATE = 325,
     GENERIC = 326,
     GUARDED = 327,
     IF = 328,
     _IN = 329,
     _INOUT = 330,
     INTEGER = 331,
     IS = 332,
     _LABEL = 333,
     LIBRARY = 334,
     _LINKAGE = 335,
     _LIST = 336,
     LOOP = 337,
     MAP = 338,
     MOD = 339,
     MUX_BIT = 340,
     MUX_VECTOR = 341,
     _NAND = 342,
     NATURAL = 343,
     NATURAL_VECTOR = 344,
     NEW = 345,
     _NEXT = 346,
     _NOR = 347,
     _NOT = 348,
     tok_NULL = 349,
     OF = 350,
     ON = 351,
     OPEN = 352,
     _OR = 353,
     OTHERS = 354,
     _OUT = 355,
     _PACKAGE = 356,
     PORT = 357,
     POSITIVE = 358,
     PROCEDURE = 359,
     PROCESS = 360,
     RANGE = 361,
     RECORD = 362,
     REG_BIT = 363,
     REG_VECTOR = 364,
     REGISTER = 365,
     REM = 366,
     REPORT = 367,
     RETURN = 368,
     SELECT = 369,
     SEVERITY = 370,
     SIGNAL = 371,
     _STABLE = 372,
     STRING = 373,
     SUBTYPE = 374,
     THEN = 375,
     TO = 376,
     TRANSPORT = 377,
     _TYPE = 378,
     UNITS = 379,
     UNTIL = 380,
     USE = 381,
     VARIABLE = 382,
     WAIT = 383,
     WARNING = 384,
     WHEN = 385,
     WHILE = 386,
     WITH = 387,
     WOR_BIT = 388,
     WOR_VECTOR = 389,
     _XOR = 390
   };
#endif
/* Tokens.  */
#define Ampersand 258
#define Apostrophe 259
#define LeftParen 260
#define RightParen 261
#define DoubleStar 262
#define Star 263
#define Plus 264
#define Comma 265
#define Minus 266
#define VarAsgn 267
#define Colon 268
#define Semicolon 269
#define _LESym 270
#define _Box 271
#define _LTSym 272
#define Arrow 273
#define _EQSym 274
#define _GESym 275
#define _GTSym 276
#define Bar 277
#define _NESym 278
#define Dot 279
#define Slash 280
#define Identifier 281
#define DecimalInt 282
#define DecimalReal 283
#define AbstractLit 284
#define BasedInt 285
#define BasedReal 286
#define CharacterLit 287
#define StringLit 288
#define BitStringLit 289
#define ABS 290
#define ACCESS 291
#define AFTER 292
#define ALIAS 293
#define ALL 294
#define tok_AND 295
#define ARCHITECTURE 296
#define ARRAY 297
#define ARG 298
#define ASSERT 299
#define ATTRIBUTE 300
#define _BEGIN 301
#define BIT 302
#define BIT_VECTOR 303
#define BLOCK 304
#define BODY 305
#define BUFFER 306
#define BUS 307
#define CASE 308
#define Y_CLOCK 309
#define COMPONENT 310
#define CONFIGURATION 311
#define CONSTANT 312
#define CONVERT 313
#define DISCONNECT 314
#define DOWNTO 315
#define ELSE 316
#define ELSIF 317
#define _END 318
#define ENTITY 319
#define ERROR 320
#define _EXIT 321
#define _FILE 322
#define FOR 323
#define FUNCTION 324
#define GENERATE 325
#define GENERIC 326
#define GUARDED 327
#define IF 328
#define _IN 329
#define _INOUT 330
#define INTEGER 331
#define IS 332
#define _LABEL 333
#define LIBRARY 334
#define _LINKAGE 335
#define _LIST 336
#define LOOP 337
#define MAP 338
#define MOD 339
#define MUX_BIT 340
#define MUX_VECTOR 341
#define _NAND 342
#define NATURAL 343
#define NATURAL_VECTOR 344
#define NEW 345
#define _NEXT 346
#define _NOR 347
#define _NOT 348
#define tok_NULL 349
#define OF 350
#define ON 351
#define OPEN 352
#define _OR 353
#define OTHERS 354
#define _OUT 355
#define _PACKAGE 356
#define PORT 357
#define POSITIVE 358
#define PROCEDURE 359
#define PROCESS 360
#define RANGE 361
#define RECORD 362
#define REG_BIT 363
#define REG_VECTOR 364
#define REGISTER 365
#define REM 366
#define REPORT 367
#define RETURN 368
#define SELECT 369
#define SEVERITY 370
#define SIGNAL 371
#define _STABLE 372
#define STRING 373
#define SUBTYPE 374
#define THEN 375
#define TO 376
#define TRANSPORT 377
#define _TYPE 378
#define UNITS 379
#define UNTIL 380
#define USE 381
#define VARIABLE 382
#define WAIT 383
#define WARNING 384
#define WHEN 385
#define WHILE 386
#define WITH 387
#define WOR_BIT 388
#define WOR_VECTOR 389
#define _XOR 390




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 206 "/home/xtof/workspace/crlcore/src/calliance/vst/CParsVstGram.yy"
{ unsigned int  _value;
         string*       _text;
         char          _flag;
         string*       _name;
         string*       _expr;
       }
/* Line 1489 of yacc.c.  */
#line 326 "/home/xtof/workspace/crlcore/src/calliance/vst/CParsVstGram.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE VSTlval;

