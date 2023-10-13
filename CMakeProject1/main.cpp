#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "struct.h"

int main() {

    std::cout << "---- The calculator supports operations - + * / ^ sin cos ln ----\n  ";
    Calc* calc = new Calc();
    std::vector<std::string> functions;// Scan the plugins directory for DLL files
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {

        std::string ext = entry.path().extension().string();
        if (ext == ".dll") {
            functions.push_back(entry.path().filename().string());
        }
    }
    if (functions.empty())
        std::cout << "    No DLLs found." << std::endl;

    // Load and initialize plugins
    for (const auto& function : functions) {
        try {
            calc->load_plugin(function);
        }
        catch (const std::exception& e) {
            std::cerr << "    Error: " << e.what() << std::endl;
        }
    }

    std::string expression;
    std::cout << "Enter a mathematical expression: ";
    std::getline(std::cin, expression);

    try {
        double result = calc->parse_expression(expression);
        std::cout << "  Calculation result: " << result << std::endl << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl << std::endl;
    }

    delete calc;

    return 0;
};