#ifndef __CALCULATE_INTERFACE_HPP__
#define __CALCULATE_INTERFACE_HPP__

#include <cstring>

#include "calculate.hpp"


using Parser = calculate::Parser;
using Expression = calculate::Parser::Expression;
using Nodes = std::vector<calculate::Parser::Expression>;
using Lexer = calculate::Parser::Lexer;
using Symbol = calculate::Parser::Symbol;
using Associativity = calculate::Parser::Associativity;
using Constant = calculate::Parser::Constant;
using Function = calculate::Parser::Function;
using Operator = calculate::Parser::Operator;

using cParser = calculate_Parser;
using cExpression = calculate_Expression;
using cNodes = calculate_Nodes;
using cFunction = calculate_Function;
using cOperator = calculate_Operator;
using cError = calculate_Error;
using cSymbol = calculate_Symbol;
using cAssociativity = calculate_Associativity;
using cFunction1 = double(*)(double);
using cFunction2 = double(*)(double, double);
using cFunction3 = double(*)(double, double, double);
using cCbck1 = double(*)(void*, double);
using cCbck2 = double(*)(void*, double, double);
using cCbck3 = double(*)(void*, double, double, double);

struct Error {
    bool status;
    std::string message;
    void reset() { status = false, message = ""; }
    void notify(bool state, const std::string& what) { status = state, message = what; }
};


template<typename Type>
std::unordered_map<Type*, std::unique_ptr<Type>>& cache() {
    static std::unordered_map<Type*, std::unique_ptr<Type>> cache;
    return cache;
}

template<typename Cache, typename Wrapper>
bool find(Wrapper resource) {
    return cache<Cache>().find(reinterpret_cast<Cache*>(resource)) != cache<Cache>().end();
}

template<typename Cache, typename Wrapper>
void free(Wrapper resource) { cache<Cache>().erase(reinterpret_cast<Cache*>(resource)); }

template<typename Cache, typename Wrapper, typename Type = Cache>
struct Factory {
    template<typename... Args>
    static Wrapper get(Args&&... args) {
        auto object = std::make_unique<Type>(std::forward<Args>(args)...);
        auto pointer = object.get();
        cache<Cache>()[pointer] = std::move(object);
        return reinterpret_cast<Wrapper>(pointer);
    }

    template<typename Operation>
    static Wrapper create(Operation operation, cError error) {
        try {
            auto object = std::make_unique<Cache>(operation());
            auto pointer = object.get();
            cache<Cache>()[pointer] = std::move(object);
            if (error != nullptr)
                reinterpret_cast<Error*>(error)->reset();
            return reinterpret_cast<Wrapper>(pointer);
        }
        catch (const std::exception& exception) {
            if (error != nullptr)
                reinterpret_cast<Error*>(error)->notify(true, exception.what());
        }
        return nullptr;
    }
};

inline std::string to_string(const std::vector<std::string>& list) {
    std::ostringstream stream;
    for(size_t i = 0; i < list.size(); i++)
        stream << (i ? "," : "") << list[i];
    return stream.str();
}

inline std::vector<std::string> from_string(const std::string& string) {
    std::istringstream stream{calculate::detail::replace(string, ",", " ")};
    return std::vector<std::string>{
        std::istream_iterator<std::string>{stream},
        std::istream_iterator<std::string>{}
    };
}

inline void write(char* where, const std::string& what, size_t length) {
    if (what.size() < length) {
        std::strncpy(where, what.c_str(), what.size());
        where[what.size()] = '\0';
    }
    else {
        std::fill(where, where + length - 1, '*');
        where[length - 1] = '\0';
    }
}

template<typename Operation>
void write(Operation operation, char* tokens, size_t length, cError error) {
    try {
        auto list = operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
        write(tokens, list, length);
        return;
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
    write(tokens, "", length);
}

template<typename Operation>
double evaluate(Operation operation, cError error) {
    try {
        auto value = operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
        return value;
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
    return std::numeric_limits<double>::quiet_NaN();
}

template<typename Operation>
void just_do(Operation operation, cError error) {
    try {
        operation();
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->reset();
    }
    catch (const std::exception& exception) {
        if (error != nullptr)
            reinterpret_cast<Error*>(error)->notify(true, exception.what());
    }
}

#endif
