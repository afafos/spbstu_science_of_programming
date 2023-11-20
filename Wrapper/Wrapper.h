#pragma once
#include <iostream>
#include <vector>

// structure for creating index sequences
template <std::size_t... Indices>
struct IndexSequence {};

// template to generate index sequences
template <std::size_t N, std::size_t... Indices>
struct MakeIndexSequence : MakeIndexSequence<N - 1, N - 1, Indices...> {};

template <std::size_t... Indices>
struct MakeIndexSequence<0, Indices...> : IndexSequence<Indices...> {};


template <typename ClassType, typename ReturnType, typename... Args>
class Wrapper {
private:
    using MethodType = ReturnType (ClassType::*)(Args...);
    ClassType* instance;
    MethodType method;
    std::vector<std::any> values;
    std::vector<std::string> names;

    // function to call the wrapped member function with stored arguments
    template <std::size_t... I>
    ReturnType call_function(IndexSequence<I...>) {
        return (instance->*method)(std::any_cast<Args>(values[I])...);
    }

public:
    // invoke the wrapped method with the provided arguments
    ReturnType invokeMethod(const std::unordered_map<std::string, std::any>& args_map) {
        for (const auto& arg_name : names) {
            auto it = args_map.find(arg_name);
            if (it == args_map.end()) {
                throw std::runtime_error("Invalid argument");
            }
            values.push_back(it->second);
        }

        if (values.size() != sizeof...(Args)) {
            throw std::runtime_error("Incorrect number of arguments");
        }

        return call_function(MakeIndexSequence<sizeof...(Args)>{});
    }

    Wrapper(ClassType* instance, MethodType method, std::unordered_map<std::string, std::any> arg_map)
            : instance(instance), method(method) {
        for (const auto& key_val : arg_map) {
            names.push_back(key_val.first);
        }
    }
};
