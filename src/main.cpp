#include "compiler/compiler.hpp"
#include "visitors/abstract_ast_visitor.hpp"
#include "visitors/print_visitor.hpp"
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include <memory>

void print_help() {
    std::cout << "OVERVIEW: YaYa language compiler\n";
    std::cout << "USAGE: yaya [-hp][-o <file>] <file>\n";
    std::cout << "OPTIONS:\n";
    std::cout << "\t-h: Print this message and exit\n";
    std::cout << "\t-p: Print the ast and exit\n";
    std::cout << "\t-o <file>: Write output to <file>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return EXIT_FAILURE;
    }

    char opt;
    bool print_ast = false;
    std::string output = "a.out";
    std::string input = argv[argc - 1];
    while ((opt = getopt(argc, argv, "hpo:")) != -1) {
        switch (opt) {
        case 'h':
            print_help();
            return EXIT_SUCCESS;
        case 'p':
            print_ast = true;
            break;
        case 'c':
            output = optarg;
            break;
        default:
            print_help();
            return EXIT_FAILURE;
        }
    }

    std::unique_ptr<yaya::AbstractASTVisitor> visitor;
    if (print_ast) {
        visitor = std::make_unique<yaya::PrintVisitor>();
    } else {
        // TODO: Replace print visitor with code generation visitor
        visitor = std::make_unique<yaya::PrintVisitor>();
    }

    yaya::Compiler compiler(input, std::move(visitor));
    if (!compiler.compile()) {
        std::cout << "Compilation failed\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
