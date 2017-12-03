#include "calculate.h"


int calculate_has_constant(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Constant>(token);
}

int calculate_has_function(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Function>(token);
}

int calculate_has_operator(cParser parser, const char* token) {
    return reinterpret_cast<Parser*>(parser)->has<Operator>(token);
}


double calculate_get_constant(cError error, cParser parser, const char* token) {
    return evaluate(
        [parser, token]() {
            return reinterpret_cast<Parser*>(parser)->get<Constant>(token);
        },
        error
    );
}

cFunction calculate_get_function(cError error, cParser parser, const char* token) {
    return Factory<Function, cFunction>::create(
        [parser, token]() {
            return reinterpret_cast<Parser*>(parser)->get<Function>(token);
        },
        error
    );
}

cOperator calculate_get_operator(cError error, cParser parser, const char* token) {
    return Factory<Operator, cOperator>::create(
        [parser, token]() {
            return reinterpret_cast<Parser*>(parser)->get<Operator>(token);
        },
        error
    );
}


void calculate_set_constant(
    cError error,
    cParser parser,
    const char* token,
    double val
) {
    just_do(
        [parser, token, val]() {
            reinterpret_cast<Parser*>(parser)->set<Constant>(
                token,
                val
            );
        },
        error
    );
}

void calculate_set_operator(
    cError error,
    cParser parser,
    const char* token,
    const char* alias,
    size_t prec,
    cAssociativity assoc,
    cPointer func
) {
    using Pointer = double(*)(double, double);

    just_do(
        [parser, token, alias, prec, assoc, func]() {
            reinterpret_cast<Parser*>(parser)->set<Operator>(
                token,
                alias,
                prec,
                static_cast<Associativity>(assoc),
                &(*reinterpret_cast<Pointer>(func))
            );
        },
        error
    );
}

void calculate_set_operator_callback(
    cError error,
    cParser parser,
    const char* token,
    void* handler,
    const char* alias,
    size_t prec,
    cAssociativity assoc,
    cPointer cbck
) {
    using Pointer = double(*)(void*, double, double);

    just_do(
        [parser, token, alias, prec, assoc, handler, cbck]() {
            reinterpret_cast<Parser*>(parser)->set<Operator>(
                token,
                alias,
                prec,
                static_cast<Associativity>(assoc),
                [handler, cbck](double x1, double x2) {
                    return reinterpret_cast<Pointer>(cbck)(handler, x1, x2);
                }
            );
        },
        error
    );
}


void calculate_remove_constant(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() {
            reinterpret_cast<Parser*>(parser)->remove<Constant>(token);
        },
        error
    );
}

void calculate_remove_function(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() {
            reinterpret_cast<Parser*>(parser)->remove<Function>(token);
        },
        error
    );
}

void calculate_remove_operator(cError error, cParser parser, const char* token) {
    just_do(
        [parser, token]() {
            reinterpret_cast<Parser*>(parser)->remove<Operator>(token);
        },
        error
    );
}


void calculate_list_constants(cParser parser, char* tokens, size_t len) {
    write(
        [parser]() {
            return to_string(
                reinterpret_cast<Parser*>(parser)->list<Constant>()
            );
        },
        tokens,
        len,
        nullptr
    );
}

void calculate_list_functions(cParser parser, char* tokens, size_t len) {
    write(
        [parser]() {
            return to_string(
                reinterpret_cast<Parser*>(parser)->list<Function>()
            );
        },
        tokens,
        len,
        nullptr
    );
}

void calculate_list_operators(cParser parser, char* tokens, size_t len) {
    write(
        [parser]() {
            return to_string(
                reinterpret_cast<Parser*>(parser)->list<Operator>()
            );
        },
        tokens,
        len,
        nullptr
    );
}


size_t calculate_arguments(cFunction symbol) {
    return reinterpret_cast<Function*>(symbol)->args();
}


void calculate_alias(cOperator symbol, char* alias, size_t len) {
    write(alias, reinterpret_cast<Operator*>(symbol)->alias, len);
}

size_t calculate_precedence(cOperator symbol) {
    return reinterpret_cast<Operator*>(symbol)->prec;
}

cAssociativity calculate_associativity(cOperator symbol) {
    return static_cast<cAssociativity>(
        reinterpret_cast<Operator*>(symbol)->assoc
    );
}

cFunction calculate_function(cOperator symbol) {
    return Factory<Function, cFunction>::get(
        reinterpret_cast<Operator*>(symbol)->func
    );
}
