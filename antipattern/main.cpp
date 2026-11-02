#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>

// Антипатерн 1: виняток примітивного типу int
void throwInteger() {
    throw 42;
}

// Антипатерн 2: виняток типу std::string
void throwString() {
    throw std::string("Invalid bit sequence length");
}

// Рекомендований підхід: використання стандартизованих винятків
void throwStandardException() {
    throw std::invalid_argument("Invalid bit sequence length");
}

// Антипатерн 3: Object Slicing через throw ex;
void demonstrateSlicing() {
    try {
        throw std::invalid_argument("Argument is invalid");
    }
    catch (const std::exception& ex) {
        // НЕПРАВИЛЬНО: створює новий об'єкт типу std::exception, втрачаючи тип std::invalid_argument
        throw ex;
    }
}

int main() {
    std::cout << "=== Antipattern 1 & 2: Primitive Types ===\n";
    
    try {
        throwInteger();
    }
    catch (const std::exception& ex) {
        std::cout << "std::exception handler: " << ex.what() << '\n';
    }
    catch (int error) {
        std::cout << "Caught primitive int: " << error << '\n';
    }

    try {
        throwString();
    }
    catch (const std::exception& ex) {
        std::cout << "std::exception handler: " << ex.what() << '\n';
    }
    catch (const std::string& error) {
        std::cout << "Caught primitive string: " << error << '\n';
    }

    std::cout << "\n=== Recommended Approach ===\n";
    try {
        throwStandardException();
    }
    catch (const std::exception& ex) {
        std::cout << "Caught via std::exception base class: " << ex.what() << '\n';
    }

    std::cout << "\n=== Antipattern 3: Object Slicing (throw ex) ===\n";
    try {
        demonstrateSlicing();
    }
    catch (const std::invalid_argument&) {
        std::cout << "Caught invalid_argument (Correct rethrow)\n";
    }
    catch (const std::exception&) {
        std::cout << "Caught base std::exception (Object slicing occurred!)\n";
    }

    return 0;
}