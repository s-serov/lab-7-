#include <iostream>
#include <exception>
#include "HuffmanCodec.h"
#include "HuffmanExceptions.h"
#include "ScopeLogger.h"

// Рівень 3: виникнення винятку
void level3() {
    ScopeLogger logger("level3");
    std::cout << "Creating codec with invalid empty text...\n";
    HuffmanCodec codec(""); // Повинно згенерувати std::invalid_argument
}

// Рівень 2: перехоплення, логування та повторне генерування через throw;
void level2() {
    ScopeLogger logger("level2");
    try {
        level3();
    }
    catch (const std::exception& ex) {
        std::cout << "[LOG] level2 caught exception: " << ex.what() << '\n';
        throw; // Повторно поширюємо поточний виняток
    }
}

// Рівень 1: проміжний виклик
void level1() {
    ScopeLogger logger("level1");
    level2();
}

int main() {
    std::cout << "=== Stack Unwinding Demonstration ===\n";
    try {
        level1();
    }
    catch (const std::exception& ex) {
        std::cout << "[MAIN] Caught in main: " << ex.what() << '\n';
    }

    std::cout << "\nProgram finished normally after handling exception.\n";
    return 0;
}