#pragma once
#include <stdexcept>
#include <string>
#include <cstddef>

// Базовий клас для всіх винятків бібліотеки Хаффмана
class HuffmanException : public std::runtime_error {
public:
    explicit HuffmanException(const std::string& message)
        : std::runtime_error(message) {}
};

// 1. Спеціалізований виняток: невідомий символ
class UnknownSymbolException : public HuffmanException {
private:
    char symbol_;
public:
    explicit UnknownSymbolException(char symbol)
        : HuffmanException(std::string("Symbol not found in Huffman dictionary: '") + symbol + "'"),
          symbol_(symbol) {}

    [[nodiscard]] char symbol() const noexcept { return symbol_; }
};

// 2. Спеціалізований виняток: некоректний бітовий рядок
class InvalidBitStringException : public HuffmanException {
private:
    std::size_t index_;
    char invalidChar_;
public:
    InvalidBitStringException(std::size_t index, char invalidChar)
        : HuffmanException("Invalid bit string character at index " + std::to_string(index) + ": '" + invalidChar + "'"),
          index_(index),
          invalidChar_(invalidChar) {}

    [[nodiscard]] std::size_t index() const noexcept { return index_; }
    [[nodiscard]] char invalidChar() const noexcept { return invalidChar_; }
};

// 3. Спеціалізований виняток: обрив бітового коду
class IncompleteCodeException : public HuffmanException {
public:
    IncompleteCodeException()
        : HuffmanException("Bit sequence is incomplete or corrupted: ended before reaching a leaf node") {}
};