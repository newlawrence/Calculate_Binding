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


double calculate_get_constant(
    cError err,
    cParser parser,
    const char* token
) {
    auto operation = [parser, token]() {
        return reinterpret_cast<Parser*>(parser)->get<Constant>(token);
    };

    return evaluate(operation, err);
}

cFunction calculate_get_function(
    cError err,
    cParser parser,
    const char* token
) {
    auto operation = [parser, token]() {
        return reinterpret_cast<Parser*>(parser)->get<Function>(token);
    };

    return Factory<Function, cFunction>::create(operation, err);
}

cOperator calculate_get_operator(
    cError err,
    cParser parser,
    const char* token
) {
    auto operation = [parser, token]() {
        return reinterpret_cast<Parser*>(parser)->get<Operator>(token);
    };

    return Factory<Operator, cOperator>::create(operation, err);
}


void calculate_set_constant(
    cError err,
    cParser parser,
    const char* token,
    double val
) {
    auto operation = [parser, token, val]() {
        reinterpret_cast<Parser*>(parser)->set<Constant>(token, val);
    };

    just_do(operation, err);
}

void calculate_set_operator(
    cError err,
    cParser parser,
    const char* token,
    const char* alias,
    size_t prec,
    cAssociativity assoc,
    cPointer func
) {
    using Pointer = double(*)(double, double);
    auto operation = [parser, token, alias, prec, assoc, func]() {
        reinterpret_cast<Parser*>(parser)->set<Operator>(
            token,
            alias,
            prec,
            static_cast<Associativity>(assoc),
            &(*reinterpret_cast<Pointer>(func))
        );
    };

    just_do(operation, err);
}

void calculate_set_operator_callback(
    cError err,
    cParser parser,
    const char* token,
    void* handler,
    const char* alias,
    size_t prec,
    cAssociativity assoc,
    cPointer cbck
) {
    using Pointer = double(*)(void*, double, double);
    auto operation = [parser, token, alias, prec, assoc, handler, cbck]() {
        reinterpret_cast<Parser*>(parser)->set<Operator>(
            token,
            alias,
            prec,
            static_cast<Associativity>(assoc),
            [handler, cbck](double x1, double x2) {
                return reinterpret_cast<Pointer>(cbck)(handler, x1, x2);
            }
        );
    };

    just_do(operation, err);
}


void calculate_remove_constant(cError err, cParser parser, const char* token) {
    auto operation = [parser, token]() {
        reinterpret_cast<Parser*>(parser)->remove<Constant>(token);
    };

    just_do(operation, err);
}

void calculate_remove_function(cError err, cParser parser, const char* token) {
    auto operation = [parser, token]() {
        reinterpret_cast<Parser*>(parser)->remove<Function>(token);
    };

    just_do(operation, err);
}

void calculate_remove_operator(cError err, cParser parser, const char* token) {
    auto operation = [parser, token]() {
        reinterpret_cast<Parser*>(parser)->remove<Operator>(token);
    };

    just_do(operation, err);
}


void calculate_list_constants(cParser parser, char* tokens, size_t len) {
    auto operation = [parser]() {
        return to_string(reinterpret_cast<Parser*>(parser)->list<Constant>());
    };

    write(operation, tokens, len, nullptr);
}

void calculate_list_functions(cParser parser, char* tokens, size_t len) {
    auto operation = [parser]() {
        return to_string(reinterpret_cast<Parser*>(parser)->list<Function>());
    };

    write(operation, tokens, len, nullptr);
}

void calculate_list_operators(cParser parser, char* tokens, size_t len) {
    auto operation = [parser]() {
        return to_string(reinterpret_cast<Parser*>(parser)->list<Operator>());
    };

    write(operation, tokens, len, nullptr);
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
