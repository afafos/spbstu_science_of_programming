#pragma once
#include <iostream>
#include <unordered_map>
#include <any>
#include <functional>
#include "Wrapper.h"

class Engine {
private:
    std::unordered_map<std::string,
    std::function<std::any(const std::unordered_map<std::string, std::any>&)>> commands;

public:
    template<typename ClassType, typename ReturnType, typename... Args>
    void register_command(Wrapper<ClassType, ReturnType, Args...>* wrapper, const std::string& command) {
        if (commands.find(command) != commands.end()) {
            throw std::runtime_error("Invalid command");
        }

        commands[command] = [wrapper](const std::unordered_map<std::string, std::any>& args) {
            return std::any(wrapper->invokeMethod(args));
        };
    };

    template<typename ReturnType = void>
    ReturnType execute(const std::string& command, const std::unordered_map<std::string, std::any>& args) {
        auto command_it = commands.find(command);

        if (command_it == commands.end()) {
            throw std::runtime_error("Command not found");
        }

        auto func = &command_it->second;

        if constexpr (std::is_void_v<ReturnType>) {
            (*func)(args);
        } else {
            return std::any_cast<ReturnType>((*func)(args));
        }
    }
};
