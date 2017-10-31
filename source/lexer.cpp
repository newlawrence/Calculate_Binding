#include "calculate.h"

const char* calculate_left_token = Lexer::left.c_str();
const char* calculate_right_token = Lexer::right.c_str();
const char* calculate_decimal_token = Lexer::decimal.c_str();
const char* calculate_separator_token = Lexer::separator.c_str();
const char* calculate_number_regex = Lexer::number_regex.pattern.c_str();
const char* calculate_name_regex = Lexer::name_regex.pattern.c_str();
const char* calculate_symbol_regex = Lexer::symbol_regex.pattern.c_str();
const char* calculate_tokenizer_regex = Lexer::tokenizer_regex.pattern.c_str();
