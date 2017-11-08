#include "calculate.h"

const char* calculate_left_token() { return Lexer::left().c_str(); };
const char* calculate_right_token() { return Lexer::right().c_str(); };
const char* calculate_decimal_token() { return Lexer::decimal().c_str(); };
const char* calculate_separator_token() { return Lexer::separator().c_str(); };
const char* calculate_number_regex() { return Lexer::number_regex().pattern.c_str(); };
const char* calculate_name_regex() { return Lexer::name_regex().pattern.c_str(); };
const char* calculate_symbol_regex() { return Lexer::symbol_regex().pattern.c_str(); };
const char* calculate_tokenizer_regex() { return Lexer::tokenizer_regex().pattern.c_str(); };
