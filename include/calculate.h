#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4503)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _CALCARG(_1, _2, _3, _4, _5, _6, _7, _8, arg, ...) arg
#define _CALCCNT(...) (sizeof((int[]){ __VA_ARGS__ })/sizeof(int))
#define _CALCDEF(...) NULL, __VA_ARGS__
#define _CALCEXP(...) __VA_ARGS__
#define _CALCDF3(f, ...) \
    f(_CALCARG(__VA_ARGS__, , , , , , _CALCEXP, _CALCDEF, )(__VA_ARGS__))
#define _CALCDF4(f, ...) \
    f(_CALCARG(__VA_ARGS__, , , , , _CALCEXP, _CALCDEF, )(__VA_ARGS__))
#define _CALCDF5(f, ...) \
    f(_CALCARG(__VA_ARGS__, , , , _CALCEXP, _CALCDEF, )(__VA_ARGS__))
#define _CALCDF6(f, ...) \
    f(_CALCARG(__VA_ARGS__, , , _CALCEXP, _CALCDEF, )(__VA_ARGS__))
#define _CALCDF7(f, ...) \
    f(_CALCARG(__VA_ARGS__, , _CALCEXP, _CALCDEF, )(__VA_ARGS__))
#define _CALCDF8(f, ...) \
    f(_CALCARG(__VA_ARGS__, _CALCEXP, _CALCDEF, )(__VA_ARGS__))

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

const char* calculate_left_token(void);
const char* calculate_right_token(void);
const char* calculate_decimal_token(void);
const char* calculate_separator_token(void);
const char* calculate_number_regex(void);
const char* calculate_name_regex(void);
const char* calculate_symbol_regex(void);
const char* calculate_tokenizer_regex(void);

#define calculate_get_parser _calculate_get_parser
#define calculate_get_default_parser _calculate_get_default_parser
calculate_Parser _calculate_get_parser(void);
calculate_Parser _calculate_get_default_parser(void);

#define calculate_has_constant _calculate_has_constant
#define calculate_has_function _calculate_has_function
#define calculate_has_operator _calculate_has_operator
int _calculate_has_constant(calculate_Parser, const char*);
int _calculate_has_function(calculate_Parser, const char*);
int _calculate_has_operator(calculate_Parser, const char*);

#define calculate_get_constant(...) \
    _CALCDF3(_calculate_get_constant, __VA_ARGS__)
#define calculate_get_function(...) \
    _CALCDF3(_calculate_get_function, __VA_ARGS__)
#define calculate_get_operator(...) \
    _CALCDF3(_calculate_get_operator, __VA_ARGS__)
double _calculate_get_constant(
    calculate_Error,
    calculate_Parser,
    const char*
);
calculate_Function _calculate_get_function(
    calculate_Error,
    calculate_Parser,
    const char*
);
calculate_Operator _calculate_get_operator(
    calculate_Error,
    calculate_Parser,
    const char*
);

#define calculate_set_constant(...) \
    _CALCDF4(_calculate_set_constant, __VA_ARGS__)
#define _calculate_set_function(x0, x1, x2, x3, ...) _Generic((x0),            \
    calculate_Error: _Generic((x3),                                            \
        double(*)(double): _calculate_set_function1,                           \
        double(*)(double, double): _calculate_set_function2,                   \
        double(*)(double, double, double): _calculate_set_function3,           \
        default: _calculate_set_function1                                      \
    ),                                                                         \
    default: _Generic((x2),                                                    \
        double(*)(double): _calculate_set_function1,                           \
        double(*)(double, double): _calculate_set_function2,                   \
        double(*)(double, double, double): _calculate_set_function3            \
    )                                                                          \
)(                                                                             \
    _Generic((x0), calculate_Error: (x0), default: NULL),                      \
    _Generic((x0), calculate_Error: (x1), default: (x0)),                      \
    _Generic((x0), calculate_Error: (x2), default: (x1)),                      \
    _Generic((x0), calculate_Error: (x3), default: (x2))                       \
)
#define calculate_set_function(...) \
    _calculate_set_function(__VA_ARGS__, 0, 0)
#define calculate_set_operator(...) \
    _CALCDF7(_calculate_set_operator, __VA_ARGS__)
void _calculate_set_constant(
    calculate_Error,
    calculate_Parser,
    const char*,
    double
);
void _calculate_set_function1(
    calculate_Error,
    calculate_Parser,
    const char*,
    double(*)(double)
);
void _calculate_set_function2(
    calculate_Error,
    calculate_Parser,
    const char*,
    double(*)(double, double)
);
void _calculate_set_function3(
    calculate_Error,
    calculate_Parser,
    const char*,
    double(*)(double, double, double)
);
void _calculate_set_operator(
    calculate_Error,
    calculate_Parser,
    const char*,
    const char*,
    size_t,
    calculate_Associativity,
    double(*)(double, double)
);

#define _calculate_set_callback(x0, x1, x2, x3, x4, ...) _Generic((x0),        \
    calculate_Error: _Generic((x4),                                            \
        double(*)(void*, double): _calculate_set_callback1,                    \
        double(*)(void*, double, double): _calculate_set_callback2,            \
        double(*)(void*, double, double, double): _calculate_set_callback3,    \
        default: _calculate_set_callback1                                      \
    ),                                                                         \
    default: _Generic((x3),                                                    \
        double(*)(void*, double): _calculate_set_callback1,                    \
        double(*)(void*, double, double): _calculate_set_callback2,            \
        double(*)(void*, double, double, double): _calculate_set_callback3     \
    )                                                                          \
)(                                                                             \
    _Generic((x0), calculate_Error: (x0), default: NULL),                      \
    _Generic((x0), calculate_Error: (x1), default: (x0)),                      \
    _Generic((x0), calculate_Error: (x2), default: (x1)),                      \
    _Generic((x0), calculate_Error: (x3), default: (x2)),                      \
    _Generic((x0), calculate_Error: (x4), default: (x3))                       \
)
#define calculate_set_callback(...) _calculate_set_callback(__VA_ARGS__, 0, 0)
#define calculate_set_operator_callback(...) _CALCDF8(                         \
    _calculate_set_operator_callback,                                          \
    __VA_ARGS__                                                                \
)
void _calculate_set_callback1(
    calculate_Error,
    calculate_Parser,
    const char*,
    void*,
    double(*)(void*, double)
);
void _calculate_set_callback2(
    calculate_Error,
    calculate_Parser,
    const char*,
    void*,
    double(*)(void*, double, double)
);
void _calculate_set_callback3(
    calculate_Error,
    calculate_Parser,
    const char*,
    void*,
    double(*)(void*, double, double, double)
);
void _calculate_set_operator_callback(
    calculate_Error,
    calculate_Parser,
    const char*,
    void*,
    const char*,
    size_t,
    calculate_Associativity,
    double(*)(void*, double, double)
);

#define calculate_remove_constant(...) \
    _CALCDF3(_calculate_remove_constant, __VA_ARGS__)
#define calculate_remove_function(...) \
    _CALCDF3(_calculate_remove_function, __VA_ARGS__)
#define calculate_remove_operator(...) \
    _CALCDF3(_calculate_remove_operator, __VA_ARGS__)
void _calculate_remove_constant(calculate_Error, calculate_Parser, const char*);
void _calculate_remove_function(calculate_Error, calculate_Parser, const char*);
void _calculate_remove_operator(calculate_Error, calculate_Parser, const char*);

#define calculate_list_constants _calculate_list_constants
#define calculate_list_functions _calculate_list_functions
#define calculate_list_operators _calculate_list_operators
void _calculate_list_constants(calculate_Parser, char*, size_t);
void _calculate_list_functions(calculate_Parser, char*, size_t);
void _calculate_list_operators(calculate_Parser, char*, size_t);

#define calculate_cast(...) _CALCDF3(_calculate_cast, __VA_ARGS__)
#define calculate_to_string(...) _CALCDF5(_calculate_to_string, __VA_ARGS__)
double _calculate_cast(
    calculate_Error,
    calculate_Parser,
    const char*
);
void _calculate_to_string(
    calculate_Error,
    calculate_Parser,
    double,
    char*,
    size_t
);

#define calculate_create_node(...) \
    _CALCDF5(_calculate_create_node, __VA_ARGS__)
#define calculate_from_value(...) \
    _CALCDF3(_calculate_from_value, __VA_ARGS__)
#define calculate_from_infix(...) \
    _CALCDF4(_calculate_from_infix, __VA_ARGS__)
#define calculate_from_postfix(...) \
    _CALCDF4(_calculate_from_postfix, __VA_ARGS__)
#define calculate_parse(...) \
    _CALCDF3(_calculate_parse, __VA_ARGS__)
calculate_Expression _calculate_create_node(
    calculate_Error,
    calculate_Parser,
    const char*,
    calculate_Nodes,
    const char*
);
calculate_Expression _calculate_from_value(
    calculate_Error,
    calculate_Parser,
    double
);
calculate_Expression _calculate_from_infix(
    calculate_Error,
    calculate_Parser,
    const char*,
    const char*
);
calculate_Expression _calculate_from_postfix(
    calculate_Error,
    calculate_Parser,
    const char*,
    const char*
);
calculate_Expression _calculate_parse(
    calculate_Error,
    calculate_Parser,
    const char*
);

#define calculate_new_variables(...) \
    _CALCDF4(_calculate_new_variables, __VA_ARGS__)
#define calculate_optimize \
    _calculate_optimize
#define calculate_replace(...) \
    _CALCDF6(_calculate_replace, __VA_ARGS__)
#define calculate_substitute(...) \
    _CALCDF5(_calculate_substitute, __VA_ARGS__)
calculate_Expression _calculate_new_variables(
    calculate_Error,
    calculate_Parser,
    calculate_Expression,
    const char*
);
calculate_Expression _calculate_optimize(
    calculate_Parser,
    calculate_Expression
);
calculate_Expression _calculate_replace(
    calculate_Error,
    calculate_Parser,
    calculate_Expression,
    size_t,
    calculate_Expression,
    const char*
);
calculate_Expression _calculate_substitute(
    calculate_Error,
    calculate_Parser,
    calculate_Expression,
    const char*,
    double
);

#define calculate_get_nodes _calculate_get_nodes
#define calculate_promote _calculate_promote
#define calculate_size _calculate_size
#define calculate_get_node(...) _CALCDF3(_calculate_get_node, __VA_ARGS__)
#define calculate_insert_node _calculate_insert_node
#define calculate_remove_node(...) _CALCDF3(_calculate_remove_node, __VA_ARGS__)
calculate_Nodes _calculate_get_nodes(void);
calculate_Nodes _calculate_promote(calculate_Expression);
size_t _calculate_size(calculate_Nodes);
calculate_Expression _calculate_get_node(
    calculate_Error,
    calculate_Nodes,
    size_t
);
calculate_Nodes _calculate_insert_node(calculate_Nodes, calculate_Expression);
calculate_Nodes _calculate_remove_node(
    calculate_Error,
    calculate_Nodes,
    size_t
);

#define calculate_hash _calculate_hash
#define calculate_equal _calculate_equal
#define calculate_token _calculate_token
#define calculate_symbol _calculate_symbol
#define calculate_branches _calculate_branches
#define calculate_nodes _calculate_nodes
#define calculate_infix _calculate_infix
#define calculate_postfix _calculate_postfix
#define calculate_variables _calculate_variables
size_t _calculate_hash(calculate_Expression);
int _calculate_equal(calculate_Expression, calculate_Expression);
void _calculate_token(calculate_Expression, char*, size_t);
calculate_Symbol _calculate_symbol(calculate_Expression);
size_t _calculate_branches(calculate_Expression);
calculate_Nodes _calculate_nodes(calculate_Expression);
void _calculate_infix(calculate_Expression, char*, size_t);
void _calculate_postfix(calculate_Expression, char*, size_t);
void _calculate_variables(calculate_Expression, char*, size_t);

#define _calculate_evaluate(x0, x1, x2, x3, x4, ...) _Generic((x0),            \
    calculate_Error: _Generic((x1),                                            \
        calculate_Expression: _calculate_evaluate_expression,                  \
        calculate_Function: _calculate_evaluate_function,                      \
        default: _calculate_evaluate_expression                                \
    ),                                                                         \
    calculate_Expression: _calculate_evaluate_expression,                      \
    calculate_Function: _calculate_evaluate_function                           \
)(                                                                             \
    _Generic((x0), calculate_Error: (x0), default: NULL),                      \
    _Generic((x0), calculate_Error: (x1), default: (x0)),                      \
    _CALCCNT(__VA_ARGS__) - _Generic((x0), calculate_Error: 2, default: 1),    \
    _Generic((x0), calculate_Error: (x2), default: (x1)),                      \
    _Generic((x0), calculate_Error: (x3), default: (x2)),                      \
    _Generic((x0), calculate_Error: (x4), default: (x3))                       \
)
#define calculate_evaluate(...) _calculate_evaluate(__VA_ARGS__, 0, 0, 0, 0, 0)
double _calculate_evaluate_expression(
    calculate_Error,
    calculate_Expression,
    size_t,
    double,
    double,
    double
);
double _calculate_evaluate_function(
    calculate_Error,
    calculate_Function,
    size_t,
    double,
    double,
    double
);

#define calculate_arguments _calculate_arguments
size_t _calculate_arguments(calculate_Function);

#define calculate_alias _calculate_alias
#define calculate_precedence _calculate_precedence
#define calculate_associativity _calculate_associativity
#define calculate_function _calculate_function
void _calculate_alias(
    calculate_Operator,
    char*,
    size_t
);
size_t _calculate_precedence(calculate_Operator);
calculate_Associativity _calculate_associativity(calculate_Operator);
calculate_Function _calculate_function(calculate_Operator);

#define calculate_get_error _calculate_get_error
#define calculate_status _calculate_status
#define calculate_message _calculate_message
calculate_Error _calculate_get_error(void);
int _calculate_status(calculate_Error);
void _calculate_message(calculate_Error, char*, size_t);

#define calculate_check(resource) _Generic((resource),                         \
        calculate_Parser: _calculate_check_parser,                             \
        calculate_Expression: _calculate_check_expression,                     \
        calculate_Nodes: _calculate_check_nodes,                               \
        calculate_Function: _calculate_check_function,                         \
        calculate_Operator: _calculate_check_operator,                         \
        calculate_Error: _calculate_check_error,                               \
    )(resource)
int _calculate_check_parser(calculate_Parser);
int _calculate_check_nodes(calculate_Nodes);
int _calculate_check_expression(calculate_Expression);
int _calculate_check_function(calculate_Function);
int _calculate_check_operator(calculate_Operator);
int _calculate_check_error(calculate_Error);

#define calculate_free(resource) _Generic((resource),                          \
        calculate_Parser: _calculate_free_parser,                              \
        calculate_Expression: _calculate_free_expression,                      \
        calculate_Nodes: _calculate_free_nodes,                                \
        calculate_Function: _calculate_free_function,                          \
        calculate_Operator: _calculate_free_operator,                          \
        calculate_Error: _calculate_free_error,                                \
    )(resource)
void _calculate_free_parser(calculate_Parser);
void _calculate_free_nodes(calculate_Nodes);
void _calculate_free_expression(calculate_Expression);
void _calculate_free_function(calculate_Function);
void _calculate_free_operator(calculate_Operator);
void _calculate_free_error(calculate_Error);

#ifdef __cplusplus
}

#include "calculate/binding.hpp"
#endif

#endif
