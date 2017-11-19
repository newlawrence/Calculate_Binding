#include "calculate.h"


cParser _calculate_get_parser() {
    return Factory<Parser, cParser>::get();
}

cParser _calculate_get_default_parser() {
    return Factory<Parser, cParser, calculate::DefaultParser>::get();
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
        [parser, value]() {
            return reinterpret_cast<Parser*>(parser)->to_string(value);
        },
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
            return reinterpret_cast<Parser*>(parser)->create_node(
                token,
                *reinterpret_cast<Nodes*>(nodes),
                from_string(variables)
            );
        },
        error
    );
}

cExpression _calculate_from_value(cError error, cParser parser, double value) {
    return Factory<Expression, cExpression>::create(
        [parser, value]() {
            return reinterpret_cast<Parser*>(parser)->create_node(value);
        },
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

cExpression _calculate_parse(
    cError error,
    cParser parser,
    const char* expression
) {
    return Factory<Expression, cExpression>::create(
        [parser, expression]() {
            return reinterpret_cast<Parser*>(parser)->parse(expression);
        },
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
            return reinterpret_cast<Parser*>(parser)->variables(
                *reinterpret_cast<Expression*>(expression),
                from_string(variables)
            );
        },
        error
    );
}

cExpression _calculate_optimize(cParser parser, cExpression expression) {
    return Factory<Expression, cExpression>::get(
        reinterpret_cast<Parser*>(parser)->
            optimize(*reinterpret_cast<Expression*>(expression))
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
                substitute(
                    *reinterpret_cast<Expression*>(expression),
                    variable,
                    value
                );
        },
        error
    );
}
