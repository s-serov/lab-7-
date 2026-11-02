#include "HuffmanCodec.h"
#include "HuffmanExceptions.h"
#include <queue>
#include <vector>
#include <stdexcept>

struct NodeComparator {
    bool operator()(const std::shared_ptr<HuffmanNode>& a, const std::shared_ptr<HuffmanNode>& b) const {
        return a->freq > b->freq;
    }
};

HuffmanCodec::HuffmanCodec(const std::string& initialData) : root_(nullptr) {
    buildTree(initialData);
}

void HuffmanCodec::generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        codeTable_[node->ch] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

std::shared_ptr<HuffmanNode> HuffmanCodec::buildTreeImpl(const std::string& text) {
    if (text.empty()) {
        throw std::invalid_argument("Cannot build Huffman tree from empty input data.");
    }

    std::unordered_map<char, std::size_t> freqs;
    for (char c : text) {
        freqs[c]++;
    }

    std::priority_queue<std::shared_ptr<HuffmanNode>, 
                       std::vector<std::shared_ptr<HuffmanNode>>, 
                       NodeComparator> pq;

    for (const auto& [ch, freq] : freqs) {
        pq.push(std::make_shared<HuffmanNode>(ch, freq));
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto parent = std::make_shared<HuffmanNode>(left->freq + right->freq, left, right);
        pq.push(parent);
    }

    return pq.top();
}

// Забезпечення Сильної Гарантії Безпеки (prepare-then-commit)
void HuffmanCodec::buildTree(const std::string& text) {
    // 1. Prepare: будуємо нове дерево у тимчасових змінних
    auto newRoot = buildTreeImpl(text);
    std::unordered_map<char, std::string> newTable;

    auto fillTable = [&](auto& self, const std::shared_ptr<HuffmanNode>& node, const std::string& code) -> void {
        if (!node) return;
        if (!node->left && !node->right) {
            newTable[node->ch] = code.empty() ? "0" : code;
            return;
        }
        self(self, node->left, code + "0");
        self(self, node->right, code + "1");
    };
    fillTable(fillTable, newRoot, "");

    // 2. Commit: заміна стану відбувається лише після успішної побудови без винятків
    this->root_ = std::move(newRoot);
    this->codeTable_ = std::move(newTable);
}

// Етап 8: Порушення сильної гарантії (стан очищається до побудови нового дерева)
void HuffmanCodec::buildTreeUnsafe(const std::string& text) {
    // Помилка: скидаємо внутрішній стан об'єкта до перевірок і обчислень
    root_.reset();
    codeTable_.clear();

    // Якщо тут виникне виняток (наприклад, порожній рядок), початковий стан уже втрачено!
    root_ = buildTreeImpl(text);
    generateCodes(root_, "");
}

std::string HuffmanCodec::encode(const std::string& text) const {
    if (!isInitialized()) {
        throw std::runtime_error("Codec is not initialized with a valid tree.");
    }
    if (text.empty()) {
        throw std::invalid_argument("Text for encoding cannot be empty.");
    }

    std::string result;
    for (char c : text) {
        auto it = codeTable_.find(c);
        if (it == codeTable_.end()) {
            throw UnknownSymbolException(c);
        }
        result += it->second;
    }
    return result;
}

std::string HuffmanCodec::decode(const std::string& bitString) const {
    if (!isInitialized()) {
        throw std::runtime_error("Codec is not initialized with a valid tree.");
    }
    if (bitString.empty()) {
        throw std::invalid_argument("Bit string for decoding cannot be empty.");
    }

    std::string result;
    auto current = root_;

    for (std::size_t i = 0; i < bitString.size(); ++i) {
        char bit = bitString[i];
        if (bit != '0' && bit != '1') {
            throw InvalidBitStringException(i, bit);
        }

        if (!current->left && !current->right) {
            result += current->ch;
            continue;
        }

        current = (bit == '0') ? current->left : current->right;

        if (!current) {
            throw IncompleteCodeException();
        }

        if (!current->left && !current->right) {
            result += current->ch;
            current = root_;
        }
    }

    if (current != root_ && (root_->left || root_->right)) {
        throw IncompleteCodeException();
    }

    return result;
}