
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "SymTab.h"
#include "Semantic.h"


void Parser();
void Parse_PROGRAM();
int Parse_VAR_DEFINITIONS(elm_role role);
int Parse_VAR_DEFINITIONS1(elm_role role);
int Parse_VAR_DEFINITION(elm_role role);
elm_type Parse_TYPE();
int Parse_VARIABLES_LIST(elm_type return_type, elm_role role);
int Parse_VARIABLES_LIST1(elm_type return_type, elm_role role);
void Parse_VARIABLE(elm_type return_type, elm_role role);
int Parse_VARIABLE1(Parameters *param, elm_role role);
void Parse_FUNC_DEFINITIONS(elm_role role);
void Parse_FUNC_DEFINITIONS1(elm_role role);
void Parse_FUNC_DEFINITION(elm_role role);
elm_type Parse_RETURNED_TYPE(elm_role role);
void Parse_PARAM_DEFINITIONS(Parameters *param, elm_role role);
elm_type Parse_STATEMENTS(elm_role role);
elm_type Parse_STATEMENTS1(elm_role role);
elm_type Parse_STATEMENTS2(elm_role role);
elm_type Parse_STATEMENT(elm_role role);
elm_type Parse_STATEMENT1(elm_role role);
void Parse_FUNCTION_CALL(Parameters *entry, elm_role role);
elm_type Parse_BLOCK(elm_role role);
void Parse_PARAMETERS_LIST(Parameters *entry, elm_role role);
elm_type Parse_EXPRESSION(elm_role role);
elm_type Parse_EXPRESSION(elm_role role);

