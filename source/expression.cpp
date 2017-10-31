#include "calculate.h"


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
        [expression]() { return reinterpret_cast<Expression*>(expression)->operator()(); },
        error
    );
}
