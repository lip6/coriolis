" Vim syntax file
" Language:	Stratus
"
" There are four options to control Stratus syntax highlighting :
"
" For highlighted numbers :
    let s_highlight_numbers    = 1
"
" For highlighted builtin functions :
    let s_highlight_builtins   = 1
"
" For highlighted standard exceptions :
    let s_highlight_exceptions = 1
"
" For highlighted stratus :
    let s_highlight_stratus    = 1


" remove old syntax
syn clear

syn keyword sStatement	        break continue del
syn keyword sStatement	        except exec finally
syn keyword sStatement	        pass print raise
syn keyword sStatement	        return try
syn keyword sStatement	        global assert
syn keyword sStatement	        lambda
syn keyword sStatement	        def class nextgroup=sFunction skipwhite
syn match   sFunction	        "[a-zA-Z_][a-zA-Z0-9_]*" contained
syn keyword sRepeat	        for while
syn keyword sConditional	if elif else
syn keyword sOperator	        and in is not or
syn keyword sPreCondit	        import from
syn match   sComment	        "#.*$" contains=sTodo
syn keyword sTodo		contained TODO FIXME XXX

" strings
syn region sString		matchgroup=Normal start=+'+  end=+'+ skip=+\\\\\|\\'+ contains=sEscape
syn region sString		matchgroup=Normal start=+"+  end=+"+ skip=+\\\\\|\\"+ contains=sEscape
syn region sString		matchgroup=Normal start=+"""+  end=+"""+ contains=sEscape
syn region sString		matchgroup=Normal start=+'''+  end=+'''+ contains=sEscape
syn region sRawString	matchgroup=Normal start=+[rR]'+ end=+'+ skip=+\\\\\|\\'+
syn region sRawString	matchgroup=Normal start=+[rR]"+    end=+"+ skip=+\\\\\|\\"+
syn region sRawString	matchgroup=Normal start=+[rR]"""+ end=+"""+
syn region sRawString	matchgroup=Normal start=+[rR]'''+ end=+'''+
syn match  sEscape		+\\[abfnrtv'"\\]+ contained
syn match  sEscape		"\\\o\o\=\o\=" contained
syn match  sEscape		"\\x\x\+" contained
syn match  sEscape		"\\$"

if exists("s_highlight_numbers")
  " numbers (including longs and complex)
  syn match   sNumber	"\<0x\x\+[Ll]\=\>"
  syn match   sNumber	"\<\d\+[LljJ]\=\>"
  syn match   sNumber	"\.\d\+\([eE][+-]\=\d\+\)\=[jJ]\=\>"
  syn match   sNumber	"\<\d\+\.\([eE][+-]\=\d\+\)\=[jJ]\=\>"
  syn match   sNumber	"\<\d\+\.\d\+\([eE][+-]\=\d\+\)\=[jJ]\=\>"
endif

if exists("s_highlight_builtins")
  " builtin functions, not really part of the syntax
  syn keyword sBuiltin	abs apply callable chr cmp coerce
  syn keyword sBuiltin	compile complex delattr dir divmod
  syn keyword sBuiltin	eval execfile filter float getattr
  syn keyword sBuiltin	globals hasattr hash hex id input
  syn keyword sBuiltin	int intern isinstance issubclass
"  syn keyword sBuiltin	len list locals long map max min
  syn keyword sBuiltin	len list locals long max min
  syn keyword sBuiltin	oct open ord pow range raw_input
  syn keyword sBuiltin	reduce reload repr round setattr
  syn keyword sBuiltin	slice str tuple type vars xrange
endif

if exists("s_highlight_exceptions")
  " builtin exceptions
  syn keyword sException	None
  syn keyword sException	ArithmeticError AssertionError
  syn keyword sException	AttributeError EOFError EnvironmentError
  syn keyword sException	Exception FloatingPointError IOError
  syn keyword sException	ImportError IndexError KeyError
  syn keyword sException	KeyboardInterrupt LookupError
  syn keyword sException	MemoryError NameError NotImplementedError
  syn keyword sException	OSError OverflowError RuntimeError
  syn keyword sException	StandardError SyntaxError SystemError
  syn keyword sException	SystemExit TypeError ValueError
  syn keyword sException	ZeroDivisionError
endif

if exists("s_highlight_stratus")
  " builtin exceptions
  syn keyword stratusSpecial    self
  
  syn keyword stratusClass  	Model
  syn keyword stratusClass  	SignalIn SignalOut SignalInOut SignalUnknown TriState CkIn VddIn VssIn Signal
  syn keyword stratusClass  	Inst
  syn keyword stratusClass  	XY
  
  syn keyword stratusMethod	    Interface Netlist Layout Vbe Pattern View Save Simul HurricanePlug Quit GetModelName Clean
  syn keyword stratusMethod	    Alias Extend
  syn keyword stratusMethod	    Buffer Mux Shift Reg Eq Ne
  syn keyword stratusMethod	    SetAnd SetOr SetNot SetXor SetNot SetMux SetAdd SetMult SetDiv
  syn keyword stratusMethod	    Delete

  syn keyword stratusMap	    param map
  
  syn keyword stratusNumber	    NOSYM SYM_X SYM_Y SYMXY ROT_P ROT_M SY_RP SY_RM 
  syn keyword stratusNumber	    VIEWS LOGICAL PHYSICAL 
  syn keyword stratusNumber	    STRATUS
  syn keyword stratusNumber	    UNPLACED PLACED FIXED
  syn keyword stratusNumber	    F_LSB_FIRST F_MSB_FIRST
  syn keyword stratusNumber	    TypePOWER TypeGROUND TypeCLOCK
  syn keyword stratusNumber	    DirectionIN DirectionOUT DirectionINOUT DirectionTRISTATE DirectionUNDEFINED
  
  syn keyword stratusFunction	Generate
  syn keyword stratusFunction	Place PlaceTop PlaceBottom PlaceRight PlaceLeft SetRefIns DefAb ResizeAb
  syn keyword stratusFunction	PlaceCentric AlimVerticalRail AlimHorizontalRail AlimConnectors PadNorth PadSouth PadEast PadWest AffichePad SearchVddVss IsConflit PowerRing GetNetInstances RoutageNet RouteCk FillCell PlaceGlue GlobalRoute DetailRoute 
  syn keyword stratusFunction	GetRefXY PlaceRef PlaceContact PlaceSegment PlacePin CopyUpSegment
  syn keyword stratusFunction	Cat
  syn keyword stratusFunction	Param
  syn keyword stratusFunction	One Zero
  
endif



" This is fast but code inside triple quoted strings screws it up. It
" is impossible to fix because the only way to know if you are inside a
" triple quoted string is to start from the beginning of the file. If
" you have a fast machine you can try uncommenting the "sync minlines"
" and commenting out the rest.
syn sync match sSync grouphere NONE "):$"
syn sync maxlines=100
"syn sync minlines=2000

if !exists("did_s_syntax_inits")
  let did_s_syntax_inits = 1
  " The default methods for highlighting.  Can be overridden later
  hi link sStatement	    Statement
  hi link sFunction	        Function
  hi link sConditional	    Conditional
  hi link sRepeat	        Repeat
  hi link sString	        String
  hi link sRawString	    String
  hi link sEscape	        Special
  hi link sOperator	        Operator
  hi link sPreCondit	    PreCondit
  hi link sComment	        Comment
  hi link sTodo		        Todo
  
  if exists("s_highlight_numbers")
    hi link sNumber	        Number
  endif
  
  if exists("s_highlight_builtins")
    hi link sBuiltin	    Function
  endif
  
  if exists("s_highlight_exceptions")
    hi link sException	    Exception
  endif
  
  if exists("s_highlight_stratus")
    hi link stratusSpecial  Statement
    hi link stratusClass	Include
    hi link stratusFunction	Special
    hi link stratusMethod	Function
    hi link stratusMap  	Identifier
    hi link stratusNumber  	Number
  endif
endif


let b:current_syntax = "stratus"

