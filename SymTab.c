#define _CRT_SECURE_NO_WARNINGS

#include "SymTab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Table* make_table(Table* current_table)
{
	Table* t = (Table*)malloc(sizeof(Table));
	t->father = current_table;
	t->head = NULL;
	return t;
}

Table *pop_table(Table* current_table)
{
	Parameters* tmp, *entry;
	Table* t = NULL;
	t = current_table->father;
	entry = current_table->head;

	while (entry != NULL)
	{
		tmp = entry;
		entry = entry->next;
		free(tmp);
	}

	free(current_table);
	return t;
}

Parameters *insert(Table* current_table, char* id_name)
{
	Parameters* tmp, *entry;
	entry = lookup(current_table, id_name);
	if (entry != NULL)
	{
		return NULL;
	}
	tmp = current_table->head;
	entry = (Parameters*)malloc(sizeof(Parameters));

	entry->entryInfo.arraySize = -1;
	entry->entryInfo.declLine = -1;
	entry->entryInfo.use = 0;
	entry->entryInfo.numOfParam = -1;
	strcpy(entry->name, id_name);
	entry->next = NULL;

	if (tmp != NULL)
	{
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = entry;
	}

	else
	{
		current_table->head = entry;
	}

	return entry;
}


Parameters* lookup(Table* current_table, char *id_name)
{
	Parameters* entry = current_table->head;
	while (entry != NULL && strcmp(entry->name, id_name) != 0)
	{
		entry = entry->next;
	}
	return entry;
}

Parameters* find(Table* current_table, char *id_name)
{
	Parameters* entry = NULL;
	Table* table = current_table;

	while (entry == NULL && table != 0)
	{
		entry = lookup(table, id_name);
		table = table->father;
	}
	return entry;
}

void set_id_type(Parameters* id_entry, elm_type id_type)
{
	if (id_entry != NULL)
		id_entry->entryInfo.varType = id_type;
}

elm_type get_id_type(Parameters* id_entry)
{
	if (id_entry != NULL)
		return id_entry->entryInfo.varType;
	else
		return TYPE_ERROR;
}


void set_arr_size(Parameters* arrParam, int sizeOfArr)
{
	if (arrParam != NULL)
		arrParam->entryInfo.arraySize = atoi(sizeOfArr);
}

void get_arr_size(Parameters* arrParam)
{
	if (arrParam != NULL)
		return arrParam->entryInfo.arraySize;
	else
		return 0;
}

void set_param_size(Parameters* param, int sizeOfParam)
{
	if (param != NULL)
		param->entryInfo.numOfParam = sizeOfParam;
}

void set_function_param(Parameters* id_entry, Parameters* param)
{
	Parameters* param2 = id_entry->entryInfo.param;
	while (param2->next != NULL)
		param2 = param2->next;
	param2->next = param;
}

void decleration_line(Parameters* param, int line)
{
	if (param != NULL)
		param->entryInfo.declLine = line;
}

void used(Parameters* param)
{
	if (param != NULL)
		param->entryInfo.use = 1;
}

char* enumConvert(elm_type type)
{
	switch (type)
	{
	case TYPE_INTEGER: return "TYPE_INTEGER";
	case TYPE_REAL: return "TYPE_REAL";
	case TYPE_INTEGER_ARRAY: return "TYPE_INTEGER_ARRAY";
	case TYPE_REAL_ARRAY: return "TYPE_REAL_ARRAY";
	case TYPE_VOID: return "TYPE_VOID";
	case TYPE_ERROR: return "TYPE_ERROR";
	}
}

Parameters* insertEntry(Table* current_table, Parameters* param)
{
	Parameters *tmp, *param2;
	param2 = lookup(current_table, param->name);
	if (param2 != NULL)
		return NULL;
	tmp = current_table->head;
	if (tmp != NULL)
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = param;
	}
	else
		current_table->head = param;
	return param;
}

Parameters* duplicateParam(Parameters* dup)
{
	Parameters *param = (Parameters*)calloc(1, sizeof(Parameters));

	strcpy(param->name, dup->name);
	param->entryInfo.arraySize = dup->entryInfo.arraySize;
	param->entryInfo.declLine = dup->entryInfo.declLine;
	param->entryInfo.use = dup->entryInfo.use;
	param->entryInfo.numOfParam = dup->entryInfo.numOfParam;
	param->next = NULL;

	return param;

}

