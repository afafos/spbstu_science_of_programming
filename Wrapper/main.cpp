#include <unordered_map>
#include <any>
#include <string>
#include "Engine.h"
#include "Subject.h"

int main() {
    Subject subj;

    Wrapper wrapper1(&subj, &Subject::f1, {{"arg1", 0}, {"arg2", 0}});
    Wrapper wrapper2(&subj, &Subject::f2, {{"arg1", std::string()}, {"arg2", std::string()}});

    Engine engine;

    engine.register_command(&wrapper1, "command1");
    engine.register_command(&wrapper2, "command2");

    std::unordered_map<std::string, std::any> args1 = {{"arg1", 10}, {"arg2", 20}};
    engine.execute("command1", args1);
    std::unordered_map<std::string, std::any> args2 = {{"arg1", std::string("World")}, {"arg2", std::string("Hello")}};
    engine.execute("command2", args2);

    return 0;
}