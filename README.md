# Compiler
Grammar 

PROGRAM -> program DEFINITIONS; STATEMENTS end

DEFINITIONS -> VAR_DEFINITIONS; FUNC_DEFINITIONS

VAR_DEFINITIONS -> VAR_DEFINITION |  VAR_DEFINITION; VAR_DEFINITIONS

VAR_DEFINITION -> TYPE  VARIABLES_LIST  

TYPE -> real  |  integer

VARIABLES_LIST -> VARIABLE | VARIABLES_LIST , VARIABLE

VARIABLE -> id  |  id [int_number]    /* id is a variable name  */

FUNC_DEFINITIONS -> FUNC_DEFINITION | FUNC_DEFINITIONS FUNC_DEFINITION

FUNC_DEFINITION -> RETURNED_TYPE  id  (PARAM_DEFINITIONS)  BLOCK  
                                       /* id is a function name  */
RETURNED_TYPE  -> void | TYPE

PARAM_DEFINITIONS -> ε  |  VAR_DEFINITIONS

STATEMENTS -> STATEMENT; |  STATEMENT; STATEMENTS

STATEMENT ->  VARIABLE = EXPRESSION   |  
		BLOCK  | 
		return  |  return EXPRESSION  |
		FUNCTION_CALL   
		
BLOCK -> { VAR_DEFINITIONS; STATEMENTS }    

FUNCTION_CALL -> id (PARAMETERS_LIST)   /* id is a function name  */

PARAMETERS_LIST -> ε  |  VARIABLES_LIST

EXPRESSION -> int_number  |  real_number  |  VARIABLE  |   id ar_op EXPRESSION  


Using Flex to create the lexical analyzer

Develop several Parsers (Syntax analyzer) using the recursive descent syntax analysis

Develop Semantic analyzer
