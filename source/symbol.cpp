#include "calculate.h"


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
