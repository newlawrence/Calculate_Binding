#include "calculate.h"


cParser calculate_get_parser() {
    return Factory<Parser, cParser>::get();
}

cParser calculate_get_default_parser() {
    return Factory<Parser, cParser, calculate::DefaultParser>::get();
}


double calculate_cast(cError error, cParser parser, const char* expr) {
    return evaluate(
        [parser, expr]() {
            return reinterpret_cast<Parser*>(parser)->cast(expr);
        },
        error
    );
}

void calculate_to_string(
    cError error,
    cParser parser,
    double val,
    char* expr,
    size_t len
) {
    write(
        [parser, val]() {
            return reinterpret_cast<Parser*>(parser)->to_string(val);
        },
        expr,
        len,
        error
    );
}


cExpression calculate_create_node(
    cError error,
    cParser parser,
    const char* token,
    cNodes nodes,
    const char* vars
) {
    return Factory<Expression, cExpression>::create(
        [parser, token, vars, nodes]() {
            return reinterpret_cast<Parser*>(parser)->create_node(
                token,
                *reinterpret_cast<Nodes*>(nodes),
                from_string(vars)
            );
        },
        error
    );
}

cExpression calculate_from_value(cError error, cParser parser, double val) {
    return Factory<Expression, cExpression>::create(
        [parser, val]() {
            return reinterpret_cast<Parser*>(parser)->create_node(val);
        },
        error
    );
}

cExpression calculate_from_infix(
    cError error,
    cParser parser,
    const char* expr,
    const char* vars
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr, vars]() {
            return reinterpret_cast<Parser*>(parser)->
                from_infix(expr, from_string(vars));
        },
        error
    );
}

cExpression calculate_from_postfix(
    cError error,
    cParser parser,
    const char* expr,
    const char* vars
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr, vars]() {
            return reinterpret_cast<Parser*>(parser)->
                from_postfix(expr, from_string(vars));
        },
        error
    );
}

cExpression calculate_parse(
    cError error,
    cParser parser,
    const char* expr
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr]() {
            return reinterpret_cast<Parser*>(parser)->parse(expr);
        },
        error
    );
}


cExpression calculate_new_variables(
    cError error,
    cParser parser,
    cExpression expr,
    const char* vars
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr, vars]() {
            return reinterpret_cast<Parser*>(parser)->vars(
                *reinterpret_cast<Expression*>(expr),
                from_string(vars)
            );
        },
        error
    );
}

cExpression calculate_optimize(cParser parser, cExpression expr) {
    return Factory<Expression, cExpression>::get(
        reinterpret_cast<Parser*>(parser)->
            optimize(*reinterpret_cast<Expression*>(expr))
    );
}

cExpression calculate_replace(
    cError error,
    cParser parser,
    cExpression expr,
    size_t branch,
    cExpression another,
    const char* vars
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr, branch, another, vars]() {
            return reinterpret_cast<Parser*>(parser)->
                replace(
                    *reinterpret_cast<Expression*>(expr),
                    branch,
                    *reinterpret_cast<Expression*>(another),
                    from_string(vars)
                );
        },
        error
    );
}

cExpression calculate_substitute(
    cError error,
    cParser parser,
    cExpression expr,
    const char* variable,
    double val
) {
    return Factory<Expression, cExpression>::create(
        [parser, expr, variable, val]() {
            return reinterpret_cast<Parser*>(parser)->
                substitute(
                    *reinterpret_cast<Expression*>(expr),
                    variable,
                    val
                );
        },
        error
    );
}
