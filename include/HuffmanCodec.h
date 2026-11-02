#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

// Вузли дерева Хаффмана
struct HuffmanNode {
    char ch;
    std::size_t freq;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char c, std::size_t f) 
        : ch(c), freq(f), left(nullptr), right(nullptr) {}
    HuffmanNode(std::size_t f, std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r) 
        : ch('\0'), freq(f), left(l), right(r) {}
};

class HuffmanCodec {
private:
    std::shared_ptr<HuffmanNode> root_;
    std::unordered_map<char, std::string> codeTable_;

    // Приватні допоміжні методи
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code);
    static std::shared_ptr<HuffmanNode> buildTreeImpl(const std::string& text);

public:
    // Конструктор за замовчуванням
    HuffmanCodec() noexcept : root_(nullptr) {}

    // Конструктор з ініціалізацією даними
    explicit HuffmanCodec(const std::string& initialData);

    // Метод оновлення моделі з Сильною Гарантією Безпеки (prepare-then-commit)
    void buildTree(const std::string& text);

    // Некоректний метод для демонстрації порушення сильної гарантії (Етап 8)
    void buildTreeUnsafe(const std::string& text);

    // Операції кодування та декодування
    std::string encode(const std::string& text) const;
    std::string decode(const std::string& bitString) const;

    // Геттери
    [[nodiscard]] bool isInitialized() const noexcept { return root_ != nullptr; }
    [[nodiscard]] std::size_t getUniqueSymbolsCount() const noexcept { return codeTable_.size(); }
    [[nodiscard]] const std::unordered_map<char, std::string>& getCodeTable() const noexcept { return codeTable_; }
};