#pragma once

#include <stdlib.h>
#include<string.h>

typedef enum elm_type {
	TYPE_INTEGER,
	TYPE_REAL,
	TYPE_INTEGER_ARRAY,
	TYPE_REAL_ARRAY,
	TYPE_VOID,
	TYPE_ERROR
} elm_type;

typedef enum elm_role {
	DEFINITON,
	STATEMENT
} elm_role;

typedef struct entry_info {
	elm_type varType;
	int use;
	int arraySize;
	int numOfParam;
	int declLine;
	struct parameters* param;
} entryInfo;

typedef struct parameters {
	struct entry_info entryInfo;
	struct parameters *next;
	char name[100];
}Parameters;

typedef struct table {
	struct table *father;
	struct paramerters *head;
} Table;

Table *make_table(Table* current_table);
Table *pop_table(Table* current_table);
Parameters *insert(Table* current_table, char* id_name);
Parameters* lookup(Table* current_table, char *id_name);
Parameters* find(Table* current_table, char *id_name);
void set_id_type(Parameters* id_entry, elm_type id_type);
elm_type get_id_type(Parameters* id_entry);
void set_arr_size(Parameters* arrParam, int sizeOfArr);
void get_arr_size(Parameters* arrParam);
void set_param_size(Parameters* param, int sizeOfParam);
void set_function_param(Parameters* id_entry, Parameters* param);
void decleration_line(Parameters* param, int line);
void used(Parameters* param);
char* enumConvert(enum elm_type type);
Parameters* insertEntry(Table* current_table, Parameters* param);
Parameters* duplicateParam(Parameters* dup);


