#include "calculate.h"


cError calculate_get_error() {
    return Factory<Error, cError>::get();
}

int calculate_status(cError error) {
    return static_cast<int>(reinterpret_cast<Error*>(error)->status);
}

void calculate_message(cError error, char* message, size_t len) {
    write(message, reinterpret_cast<Error*>(error)->message.c_str(), len);
}


int calculate_check_parser(cParser resource) {
    return find<Parser, cParser>(resource);
}

int calculate_check_nodes(cNodes resource) {
    return find<Nodes, cNodes>(resource);
}

int calculate_check_expression(cExpression resource) {
    return find<Expression, cExpression>(resource);
}

int calculate_check_function(cFunction resource) {
    return find<Function, cFunction>(resource);
}

int calculate_check_operator(cOperator resource) {
    return find<Operator, cOperator>(resource);
}

int calculate_check_error(cError resource) {
    return find<Error, cError>(resource);
}


void calculate_free_parser(cParser resource) {
    return free<Parser, cParser>(resource);
}

void calculate_free_nodes(cNodes resource) {
    return free<Nodes, cNodes>(resource);
}

void calculate_free_expression(cExpression resource) {
    return free<Expression, cExpression>(resource);
}

void calculate_free_function(cFunction resource) {
    return free<Function, cFunction>(resource);
}

void calculate_free_operator(cOperator resource) {
    return free<Operator, cOperator>(resource);
}

void calculate_free_error(cError resource) {
    return free<Error, cError>(resource);
}
