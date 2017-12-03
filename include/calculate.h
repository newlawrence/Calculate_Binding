#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4503)
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct calculate_ParserHandler;
struct calculate_ExpressionHandler;
struct calculate_NodesHandler;
struct calculate_FunctionHandler;
struct calculate_OperatorHandler;
struct calculate_ErrorHandler;

enum calculate_SymbolType {
    calculate_LFT=0,
    calculate_RGT,
    calculate_SEP,
    calculate_CON,
    calculate_FUN,
    calculate_OPE
};
enum calculate_AssociativityType {
    calculate_LEFT=0,
    calculate_RIGHT,
    calculate_BOTH
};


#ifdef __cplusplus
#include <cstdlib>
using calculate_Parser = calculate_ParserHandler*;
using calculate_Expression = calculate_ExpressionHandler*;
using calculate_Nodes = calculate_NodesHandler*;
using calculate_Function = calculate_FunctionHandler*;
using calculate_Operator = calculate_OperatorHandler*;
using calculate_Error = calculate_ErrorHandler*;
using calculate_Symbol = calculate_SymbolType;
using calculate_Associativity = calculate_AssociativityType;
using size_t = std::size_t;
#else
typedef struct calculate_ParserHandler* calculate_Parser;
typedef struct calculate_ExpressionHandler* calculate_Expression;
typedef struct calculate_NodesHandler* calculate_Nodes;
typedef struct calculate_FunctionHandler* calculate_Function;
typedef struct calculate_OperatorHandler* calculate_Operator;
typedef struct calculate_ErrorHandler* calculate_Error;
typedef enum calculate_SymbolType calculate_Symbol;
typedef enum calculate_AssociativityType calculate_Associativity;
#endif
typedef void(*calculate_Pointer)(void);

#define cParser calculate_Parser
#define cExpression calculate_Expression
#define cNodes calculate_Nodes
#define cFunction calculate_Function
#define cOperator calculate_Operator
#define cError calculate_Error
#define cSymbol calculate_Symbol
#define cAssociativity calculate_Associativity
#define cPointer calculate_Pointer


const char* calculate_left_token(void);
const char* calculate_right_token(void);
const char* calculate_decimal_token(void);
const char* calculate_separator_token(void);
const char* calculate_number_regex(void);
const char* calculate_name_regex(void);
const char* calculate_symbol_regex(void);
const char* calculate_tokenizer_regex(void);


cParser calculate_get_parser(void);
cParser calculate_get_default_parser(void);


int calculate_has_constant(cParser, const char*);
int calculate_has_function(cParser, const char*);
int calculate_has_operator(cParser, const char*);

double calculate_get_constant(cError, cParser, const char*);
calculate_Function calculate_get_function(cError, cParser, const char*);
calculate_Operator calculate_get_operator(cError, cParser, const char*);

void calculate_set_constant(cError, cParser, const char*, double);
void calculate_set_function(cError, cParser, const char*, cPointer);
void calculate_set_operator(
    cError,
    cParser,
    const char*,
    const char*,
    size_t,
    cAssociativity,
    cPointer
);

void calculate_set_function_callback(
    cError,
    cParser,
    const char*,
    void*,
    cPointer
);
void calculate_set_operator_callback(
    cError,
    cParser,
    const char*,
    void*,
    const char*,
    size_t,
    cAssociativity,
    cPointer
);

void calculate_remove_constant(cError, cParser, const char*);
void calculate_remove_function(cError, cParser, const char*);
void calculate_remove_operator(cError, cParser, const char*);

void calculate_list_constants(cParser, char*, size_t);
void calculate_list_functions(cParser, char*, size_t);
void calculate_list_operators(cParser, char*, size_t);


double calculate_cast(cError, cParser, const char*);
void calculate_to_string(cError, cParser, double, char*, size_t);

cExpression calculate_create_node(
    cError,
    cParser,
    const char*,
    cNodes,
    const char*
);
cExpression calculate_from_value(cError, cParser, double);
cExpression calculate_from_infix(cError, cParser, const char*, const char*);
cExpression calculate_from_postfix(cError, cParser, const char*, const char*);
cExpression calculate_parse(cError, cParser, const char*);

cExpression calculate_new_variables(cError, cParser, cExpression, const char*);
cExpression calculate_optimize(cParser, cExpression);
cExpression calculate_replace(
    cError,
    cParser,
    cExpression,
    size_t,
    cExpression,
    const char*
);
cExpression calculate_substitute(
    cError,
    cParser,
    cExpression,
    const char*,
    double
);

cNodes calculate_get_nodes(void);
cNodes calculate_promote(cExpression);
size_t calculate_size(cNodes);
cExpression calculate_get_node(cError, cNodes, size_t);
cNodes calculate_insert_node(cNodes, cExpression);
cNodes calculate_remove_node(cError, cNodes, size_t);

size_t calculate_hash(cExpression);
int calculate_equal(cExpression, cExpression);
void calculate_token(cExpression, char*, size_t);
cSymbol calculate_symbol(cExpression);
size_t calculate_branches(cExpression);
cNodes calculate_nodes(cExpression);
void calculate_infix(cExpression, char*, size_t);
void calculate_postfix(cExpression, char*, size_t);
void calculate_variables(cExpression, char*, size_t);
void(*)(void) calculate_expression(cExpression);

size_t calculate_arguments(cFunction);
void(*)(void) calculate_function(cFunction);

void calculate_alias(cOperator, char*, size_t);
size_t calculate_precedence(cOperator);
cAssociativity calculate_associativity(cOperator);
cFunction calculate_function(cOperator);


cError calculate_get_error(void);
int calculate_status(cError);
void calculate_message(cError, char*, size_t);

int calculate_check_parser(cParser);
int calculate_check_nodes(cNodes);
int calculate_check_expression(cExpression);
int calculate_check_function(cFunction);
int calculate_check_operator(cOperator);
int calculate_check_error(cError);

void calculate_free_parser(cParser);
void calculate_free_nodes(cNodes);
void calculate_free_expression(cExpression);
void calculate_free_function(cFunction);
void calculate_free_operator(cOperator);
void calculate_free_error(cError);


#undef cPointer
#undef cAssociativity
#undef cSymbol
#undef cError
#undef cOperator
#undef cFunction
#undef cNodes
#undef cExpression
#undef cParser

#ifdef __cplusplus
}

#include "calculate/binding.hpp"
#endif

#endif
