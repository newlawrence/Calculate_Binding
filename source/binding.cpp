#include <cstring>

#include "calculate.h"
#include "calculate.hpp"


namespace {

using Parser = calculate::Parser;
using Expression = calculate::Parser::Expression;
using Nodes = std::vector<calculate::Parser::Expression>;
using Lexer = calculate::Parser::Lexer;
using Symbol = calculate::Parser::Symbol;
using Associativity = calculate::Parser::Associativity;
using Constant = calculate::Parser::Constant;
using Function = calculate::Parser::Function;
using Operator = calculate::Parser::Operator;

using cParser = calculate_Parser;
using cExpression = calculate_Expression;
using cNodes = calculate_Nodes;
using cFunction = calculate_Function;
using cOperator = calculate_Operator;
using cError = calculate_Error;
using cSymbol = calculate_Symbol;
using cAssociativity = calculate_Associativity;
using cFunction1 = double(*)(double);
using cFunction2 = double(*)(double, double);
using cFunction3 = double(*)(double, double, double);
using cCbck1 = double(*)(void*, double);
using cCbck2 = double(*)(void*, double, double);
using cCbck3 = double(*)(void*, double, double, double);

struct Error {
    bool status;
    std::string message;
    void reset() { status = false, message = ""; }
    void notify(bool state, const std::string& what) { status = state, message = what; }
};


template<typename Type>
std::unordered_map<Type*, std::unique_ptr<Type>>& cache() {
    static std::unordered_map<Type*, std::unique_ptr<Type>> cache;
    return cache;
}

template<typename Cache, typename Wrapper>
bool find(Wrapper resource) {
    return cache<Cache>().find(reinterpret_cast<Cache*>(resource)) != cache<Cache>().end();
}

template<typename Cache, typename Wrapper>
void free(Wrapper resource) { cache<Cache>().erase(reinterpret_cast<Cache*>(resource)); }

template<typename Cache, typename Wrapper, typename Type = Cache>
struct Factory {
    template<typename... Args>
    static Wrapper get(Args&&... args) {
        auto object = std::make_unique<Type>(std::forward<Args>(args)...);
        auto pointer = object.get();
        cache<Cache>()[pointer] = std::move(object);
        return reinterpret_cast<Wrapper>(pointer);
    }

    template<typename Operation>
    static Wrapper create(Operation operation, cError error) {
        try {
            auto object = std::make_unique<Cache>(operation());
            auto pointer = object.get();
            cache<Cache>()[pointer] = std::move(object);
            if (error != nullptr)
                reinterpret_cast<Error*>(error)->reset();
            return reinterpret_cast<Wrapper>(pointer);
        }
        catch (const std::exception& exception) {
            if (error != nullptr)
                reinterpret_cast<Error*>(error)->notify(true, exception.what());
        }
        return nullptr;
    }
};

std::string to_string(const std::vector<std::string>& list) {
    std::ostringstream stream;
    for(size_t i = 0; i < list.size(); i++)
        stream << (i ? "," : "") << list[i];
    return stream.str();
}

std::vector<std::string> from_string(const std::string& string) {
    std::istringstream stream{calculate::detail::replace(string, ",", " ")};
    return std::vector<std::string>{
        std::istream_iterator<std::string>{stream},
        std::istream_iterator<std::string>{}
    };
}

void write(char* where, const std::string& what, size_t length) {
    if (what.size() < length) {
        std::strncpy(where, what.c_str(), what.size());
        where[what.size()] = '\0';
    }
    else {
        std::fill(where, where + length - 1, '*');
        where[length - 1] = '\0';
    }
}

template<typename Operation>
void write(Operation operation, char* tokens, size_t length, cError error) {
    try {
        auto list = operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
        write(tokens, list, length);
        return;
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
    write(tokens, "", length);
}

template<typename Operation>
double evaluate(Operation operation, cError error) {
    try {
        auto value = operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
        return value;
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
    return std::numeric_limits<double>::quiet_NaN();
}

template<typename Operation>
void just_do(Operation operation, cError error) {
    try {
        operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
}

}


const char* calculate_left_token = Lexer::left.c_str();
const char* calculate_right_token = Lexer::right.c_str();
const char* calculate_decimal_token = Lexer::decimal.c_str();
const char* calculate_separator_token = Lexer::separator.c_str();
const char* calculate_number_regex = Lexer::number_regex.pattern.c_str();
const char* calculate_name_regex = Lexer::name_regex.pattern.c_str();
const char* calculate_symbol_regex = Lexer::symbol_regex.pattern.c_str();
const char* calculate_tokenizer_regex = Lexer::tokenizer_regex.pattern.c_str();


cParser _calculate_get_parser() { return Factory<Parser, cParser>::get(); }

cParser _calculate_get_default_parser() {
    return Factory<Parser, cParser, calculate::DefaultParser>::get();
}


int _calculate_has_constant(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Constant>(token);
}

int _calculate_has_function(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Function>(token);
}

int _calculate_has_operator(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Operator>(token);
}


double _calculate_get_constant(cError error, cParser parser, const char* token) {
    return evaluate(
        [parser, token]() { return reinterpret_cast<Parser*>(parser)->get<Constant>(token); },
        error
    );
}

cFunction _calculate_get_function(cError error, cParser parser, const char* token) {
    return Factory<Function, cFunction>::create(
        [parser, token]() { return reinterpret_cast<Parser*>(parser)->get<Function>(token); },
        error
    );
}

cOperator _calculate_get_operator(cError error, cParser parser, const char* token) {
    return Factory<Operator, cOperator>::create(
        [parser, token]() { return reinterpret_cast<Parser*>(parser)->get<Operator>(token); },
        error
    );
}


void _calculate_set_constant(cError error, cParser parser, const char* token, double value) {
    just_do(
        [parser, token, value]() {
            reinterpret_cast<Parser*>(parser)->set<Constant>(token, value);
        },
        error
    );
}

void _calculate_set_function1(
    cError error,
    cParser parser,
    const char* token,
    cFunction1 function
) {
    just_do(
        [parser, token, function]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(token, &(*function));
        },
        error
    );
}

void _calculate_set_function2(
    cError error,
    cParser parser,
    const char* token,
    cFunction2 function
) {
    just_do(
        [parser, token, function]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(token, &(*function));
        },
        error
    );
}

void _calculate_set_function3(
    cError error,
    cParser parser,
    const char* token,
    cFunction3 function
) {
    just_do(
        [parser, token, function]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(token, &(*function));
        },
        error
    );
}

void _calculate_set_operator(
    cError error,
    cParser parser,
    const char* token,
    const char* alias,
    size_t precedence,
    cAssociativity associativity,
    cFunction2 function
) {
    just_do(
        [parser, token, alias, precedence, associativity, function]() {
            reinterpret_cast<Parser*>(parser)->set<Operator>(
                token,
                alias,
                precedence,
                static_cast<Associativity>(associativity),
                &(*function)
            );
        },
        error
    );
}


void _calculate_set_callback1(
    cError error,
    cParser parser,
    const char* token,
    void* handler,
    cCbck1 cbck
) {
    just_do(
        [parser, token, handler, cbck]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(
                token,
                [handler, cbck](double x1) { return cbck(handler, x1); }
            );
        },
        error
    );
}

void _calculate_set_callback2(
    cError error,
    cParser parser,
    const char* token,
    void* handler,
    cCbck2 cbck
) {
    just_do(
        [parser, token, handler, cbck]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(
                token,
                [handler, cbck](double x1, double x2) { return cbck(handler, x1, x2); }
            );
        },
        error
    );
}

void _calculate_set_callback3(
    cError error,
    cParser parser,
    const char* token,
    void* handler,
    cCbck3 cbck
) {
    just_do(
        [parser, token, handler, cbck]() {
            reinterpret_cast<Parser*>(parser)->set<Function>(
                token,
                [handler, cbck](double x1, double x2, double x3) {
                    return cbck(handler, x1, x2, x3);
                }
            );
        },
        error
    );
}

void _calculate_set_operator_callback(
    cError error,
    cParser parser,
    const char* token,
    void* handler,
    const char* alias,
    size_t precedence,
    cAssociativity associativity,
    cCbck2 cbck
) {
    just_do(
        [parser, token, alias, precedence, associativity, handler, cbck]() {
            reinterpret_cast<Parser*>(parser)->set<Operator>(
                token,
                alias,
                precedence,
                static_cast<Associativity>(associativity),
                [handler, cbck](double x1, double x2) { return cbck(handler, x1, x2); }
            );
        },
        error
    );
}


void _calculate_remove_constant(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() { reinterpret_cast<Parser*>(parser)->remove<Constant>(token); },
        error
    );
}

void _calculate_remove_function(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() { reinterpret_cast<Parser*>(parser)->remove<Function>(token); },
        error
    );
}

void _calculate_remove_operator(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() { reinterpret_cast<Parser*>(parser)->remove<Operator>(token); },
        error
    );
}


void _calculate_list_constants(cParser parser, char* tokens, size_t length) {
    write(
        [parser]() { return to_string(reinterpret_cast<Parser*>(parser)->list<Constant>()); },
        tokens,
        length,
        nullptr
    );
}

void _calculate_list_functions(cParser parser, char* tokens, size_t length) {
    write(
        [parser]() { return to_string(reinterpret_cast<Parser*>(parser)->list<Function>()); },
        tokens,
        length,
        nullptr
    );
}

void _calculate_list_operators(cParser parser, char* tokens, size_t length) {
    write(
        [parser]() { return to_string(reinterpret_cast<Parser*>(parser)->list<Operator>()); },
        tokens,
        length,
        nullptr
    );
}


double _calculate_cast(cError error, cParser parser, const char* expression) {
    return evaluate(
        [parser, expression]() {
            return reinterpret_cast<Parser*>(parser)->cast(expression);
        },
        error
    );
}

void _calculate_to_string(
    cError error,
    cParser parser,
    double value,
    char* expression,
    size_t length
) {
    write(
        [parser, value]() { return reinterpret_cast<Parser*>(parser)->to_string(value); },
        expression,
        length,
        error
    );
}


cExpression _calculate_create_node(
    cError error,
    cParser parser,
    const char* token,
    cNodes nodes,
    const char* variables
) {
    return Factory<Expression, cExpression>::create(
        [parser, token, variables, nodes]() {
            return reinterpret_cast<Parser*>(parser)->
                create_node(token, *reinterpret_cast<Nodes*>(nodes), from_string(variables));
        },
        error
    );
}

cExpression _calculate_from_value(cError error, cParser parser, double value) {
    return Factory<Expression, cExpression>::create(
        [parser, value]() { return reinterpret_cast<Parser*>(parser)->create_node(value); },
        error
    );
}

cExpression _calculate_from_infix(
    cError error,
    cParser parser,
    const char* expression,
    const char* variables
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression, variables]() {
            return reinterpret_cast<Parser*>(parser)->
                from_infix(expression, from_string(variables));
        },
        error
    );
}

cExpression _calculate_from_postfix(
    cError error,
    cParser parser,
    const char* expression,
    const char* variables
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression, variables]() {
            return reinterpret_cast<Parser*>(parser)->
                from_postfix(expression, from_string(variables));
        },
        error
    );
}

cExpression _calculate_parse(cError error, cParser parser, const char* expression) {
    return Factory<Expression, cExpression>::create(
        [parser, expression]() { return reinterpret_cast<Parser*>(parser)->parse(expression); },
        error
    );
}


cExpression _calculate_new_variables(
    cError error,
    cParser parser,
    cExpression expression,
    const char* variables
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression, variables]() {
            return reinterpret_cast<Parser*>(parser)->
                variables(*reinterpret_cast<Expression*>(expression), from_string(variables));
        },
        error
    );
}

cExpression _calculate_optimize(cParser parser, cExpression expression) {
    return Factory<Expression, cExpression>::get(
        reinterpret_cast<Parser*>(parser)->optimize(*reinterpret_cast<Expression*>(expression))
    );
}

cExpression _calculate_replace(
    cError error,
    cParser parser,
    cExpression expression,
    size_t branch,
    cExpression another,
    const char* variables
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression, branch, another, variables]() {
            return reinterpret_cast<Parser*>(parser)->
                replace(
                    *reinterpret_cast<Expression*>(expression),
                    branch,
                    *reinterpret_cast<Expression*>(another),
                    from_string(variables)
                );
        },
        error
    );
}

cExpression _calculate_substitute(
    cError error,
    cParser parser,
    cExpression expression,
    const char* variable,
    double value
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression, variable, value]() {
            return reinterpret_cast<Parser*>(parser)->
                substitute(*reinterpret_cast<Expression*>(expression), variable, value);
        },
        error
    );
}


cNodes _calculate_get_nodes() { return Factory<Nodes, cNodes>::get(Nodes{}); }

cNodes _calculate_promote(cExpression expression) {
    return Factory<Nodes, cNodes>::get(Nodes{*reinterpret_cast<Expression*>(expression)});
}

size_t _calculate_size(cNodes nodes) { return reinterpret_cast<Nodes*>(nodes)->size(); }

cExpression _calculate_get_node(cError error, cNodes nodes, size_t n) {
    return Factory<Expression, cExpression>::create(
        [nodes, n]() { return reinterpret_cast<Nodes*>(nodes)->at(n); },
        error
    );
}

cNodes _calculate_insert_node(cNodes nodes, cExpression expression) {
    Nodes extended{*reinterpret_cast<Nodes*>(nodes)};
    extended.push_back(*reinterpret_cast<Expression*>(expression));
    return Factory<Nodes, cNodes>::get(extended);
}

cNodes _calculate_remove_node(cError error, cNodes nodes, size_t n) {
    using std::swap;
    Nodes trimmed{*reinterpret_cast<Nodes*>(nodes)};
    return Factory<Nodes, cNodes>::create(
        [trimmed{std::move(trimmed)}, n]() mutable {
            swap(trimmed.at(n), trimmed.back());
            trimmed.pop_back();
            return trimmed;
        },
        error
    );
}


size_t _calculate_hash(cExpression expression) {
    return std::hash<typename Parser::Expression>{}(*reinterpret_cast<Expression*>(expression));
}

int _calculate_equal(cExpression one, cExpression another) {
    return reinterpret_cast<Expression*>(one)->operator==(
        *reinterpret_cast<Expression*>(another)
    );
}

void _calculate_token(cExpression expression, char* token, size_t length) {
    write(token, reinterpret_cast<Expression*>(expression)->token(), length);
}

cSymbol _calculate_symbol(cExpression expression) {
    return static_cast<cSymbol>(reinterpret_cast<Expression*>(expression)->symbol());
}

size_t _calculate_branches(cExpression expression) {
    return reinterpret_cast<Expression*>(expression)->branches();
}

cNodes _calculate_nodes(cExpression expression) {
    Nodes nodes;
    for (size_t i = 0; i < reinterpret_cast<Expression*>(expression)->branches(); i++)
        nodes.push_back(reinterpret_cast<Expression*>(expression)->operator[](i));
    return Factory<Nodes, cNodes>::get(nodes);
}

void _calculate_infix(cExpression expression, char* infix, size_t length) {
    write(infix, reinterpret_cast<Expression*>(expression)->infix(), length);
}

void _calculate_postfix(cExpression expression, char* postfix, size_t length) {
    write(postfix, reinterpret_cast<Expression*>(expression)->postfix(), length);
}

void _calculate_variables(cExpression expression, char* variables, size_t length) {
    write(variables, to_string(reinterpret_cast<Expression*>(expression)->variables()), length);
}

double _calculate_evaluate_expression(
    cError error,
    cExpression expression,
    size_t arguments,
    double x,
    double y,
    double z
) {
    switch(arguments) {
    case 1:
        return evaluate(
            [expression, x]() {
                return reinterpret_cast<Expression*>(expression)->operator()(x);
            },
            error
        );
    case 2:
        return evaluate(
            [expression, x, y]() {
                return reinterpret_cast<Expression*>(expression)->operator()(x, y);
            },
            error
        );
    case 3:
        return evaluate(
            [expression, x, y, z]() {
                return reinterpret_cast<Expression*>(expression)->operator()(x, y, z);
            },
            error
        );
    }
    return evaluate(
        [expression]() { return double{*reinterpret_cast<Expression*>(expression)}; },
        error
    );
}


double _calculate_evaluate_function(
    cError error,
    cFunction function,
    size_t arguments,
    double x,
    double y,
    double z
) {
    auto parser = calculate::DefaultParser();
    switch(arguments) {
    case 1:
        return evaluate(
            [parser, function, x]() {
                return reinterpret_cast<Function*>(function)->operator()(
                    parser.create_node(x)
                );
            },
            error
        );
    case 2:
        return evaluate(
            [parser, function, x, y]() {
                return reinterpret_cast<Function*>(function)->operator()(
                    parser.create_node(x),
                    parser.create_node(y)
                );
            },
            error
        );
    case 3:
        return evaluate(
            [parser, function, x, y, z]() {
                return reinterpret_cast<Function*>(function)->operator()(
                    parser.create_node(x),
                    parser.create_node(y),
                    parser.create_node(z)
                );
            },
            error
        );
    }
    return evaluate(
        [function]() { return reinterpret_cast<Function*>(function)->operator()(); },
        error
    );
}


size_t _calculate_arguments(cFunction symbol) {
    return reinterpret_cast<Function*>(symbol)->arguments();
}


void _calculate_alias(cOperator symbol, char* alias, size_t length) {
    write(alias, reinterpret_cast<Operator*>(symbol)->alias, length);
}

size_t _calculate_precedence(cOperator symbol) {
    return reinterpret_cast<Operator*>(symbol)->precedence;
}

cAssociativity _calculate_associativity(cOperator symbol) {
    return static_cast<cAssociativity>(reinterpret_cast<Operator*>(symbol)->associativity);
}

cFunction _calculate_function(cOperator symbol) {
    return Factory<Function, cFunction>::get(reinterpret_cast<Operator*>(symbol)->function);
}


cError _calculate_get_error() { return Factory<Error, cError>::get(); }

int _calculate_status(cError error) {
    return static_cast<int>(reinterpret_cast<Error*>(error)->status);
}

void _calculate_message(cError error, char* message, size_t length) {
    write(message, reinterpret_cast<Error*>(error)->message.c_str(), length);
}


int _calculate_check_parser(cParser resource) { return find<Parser, cParser>(resource); }

int _calculate_check_nodes(cNodes resource) { return find<Nodes, cNodes>(resource); }

int _calculate_check_expression(cExpression resource) {
    return find<Expression, cExpression>(resource);
}

int _calculate_check_function(cFunction resource) {
    return find<Function, cFunction>(resource);
}

int _calculate_check_operator(cOperator resource) {
    return find<Operator, cOperator>(resource);
}

int _calculate_check_error(cError resource) { return find<Error, cError>(resource); }


void _calculate_free_parser(cParser resource) { return free<Parser, cParser>(resource); }

void _calculate_free_nodes(cNodes resource) { return free<Nodes, cNodes>(resource); }

void _calculate_free_expression(cExpression resource) {
    return free<Expression, cExpression>(resource);
}

void _calculate_free_function(cFunction resource) {
    return free<Function, cFunction>(resource);
}

void _calculate_free_operator(cOperator resource) {
    return free<Operator, cOperator>(resource);
}

void _calculate_free_error(cError resource) { return free<Error, cError>(resource); }
