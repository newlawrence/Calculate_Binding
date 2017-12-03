#include "calculate.h"


cNodes calculate_get_nodes() {
    return Factory<Nodes, cNodes>::get(Nodes{});
}

cNodes calculate_promote(cExpression expr) {
    return Factory<Nodes, cNodes>::get(
        Nodes{*reinterpret_cast<Expression*>(expr)}
    );
}

size_t calculate_size(cNodes nodes) {
    return reinterpret_cast<Nodes*>(nodes)->size();
}

cExpression calculate_get_node(cError err, cNodes nodes, size_t n) {
    return Factory<Expression, cExpression>::create(
        [nodes, n]() { return reinterpret_cast<Nodes*>(nodes)->at(n); },
        err
    );
}

cNodes calculate_insert_node(cNodes nodes, cExpression expr) {
    Nodes extended{*reinterpret_cast<Nodes*>(nodes)};
    extended.push_back(*reinterpret_cast<Expression*>(expr));
    return Factory<Nodes, cNodes>::get(extended);
}

cNodes calculate_remove_node(cError err, cNodes nodes, size_t n) {
    using std::swap;
    Nodes trimmed{*reinterpret_cast<Nodes*>(nodes)};
    return Factory<Nodes, cNodes>::create(
        [trimmed{std::move(trimmed)}, n]() mutable {
            swap(trimmed.at(n), trimmed.back());
            trimmed.pop_back();
            return trimmed;
        },
        err
    );
}


size_t calculate_hash(cExpression expr) {
    return std::hash<typename Parser::Expression>{}(
        *reinterpret_cast<Expression*>(expr)
    );
}

int calculate_equal(cExpression one, cExpression another) {
    return reinterpret_cast<Expression*>(one)->operator==(
        *reinterpret_cast<Expression*>(another)
    );
}

void calculate_token(cExpression expr, char* token, size_t len) {
    write(token, reinterpret_cast<Expression*>(expr)->token(), len);
}

cSymbol calculate_symbol(cExpression expr) {
    return static_cast<cSymbol>(reinterpret_cast<Expression*>(expr)->symbol());
}

size_t calculate_branches(cExpression expr) {
    return reinterpret_cast<Expression*>(expr)->branches();
}

cNodes calculate_nodes(cExpression expr) {
    Nodes nodes;
    for (size_t i = 0; i < reinterpret_cast<Expression*>(expr)->branches(); i++)
        nodes.push_back(reinterpret_cast<Expression*>(expr)->operator[](i));
    return Factory<Nodes, cNodes>::get(nodes);
}

void calculate_infix(cExpression expr, char* str, size_t len) {
    write(str, reinterpret_cast<Expression*>(expr)->infix(), len);
}

void calculate_postfix(cExpression expr, char* str, size_t len) {
    write(str, reinterpret_cast<Expression*>(expr)->postfix(), len);
}

void calculate_variables(cExpression expr, char* vars, size_t len) {
    write(vars, to_string(reinterpret_cast<Expression*>(expr)->vars()), len);
}
