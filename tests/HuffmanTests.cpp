#include <gtest/gtest.h>
#include "HuffmanCodec.h"
#include "HuffmanExceptions.h"
#include <stdexcept>

// 1. Перевірка створення коректного об'єкта та основних операцій
TEST(HuffmanCodecTest, EncodesAndDecodesCorrectly) {
    HuffmanCodec codec("abracadabra");
    
    EXPECT_TRUE(codec.isInitialized());
    EXPECT_EQ(codec.getUniqueSymbolsCount(), 5u);

    std::string encoded = codec.encode("abra");
    EXPECT_FALSE(encoded.empty());

    std::string decoded = codec.decode(encoded);
    EXPECT_EQ(decoded, "abra");
}

// 2. Перевірка реакції на порожні вхідні дані під час конструювання
TEST(HuffmanCodecTest, RejectsEmptyConstructorData) {
    EXPECT_THROW(
        HuffmanCodec(""),
        std::invalid_argument
    );
}

// 3. Перевірка реакції на порожній рядок при кодуванні
TEST(HuffmanCodecTest, RejectsEmptyEncodingText) {
    HuffmanCodec codec("test");
    EXPECT_THROW(
        codec.encode(""),
        std::invalid_argument
    );
}

// 4. Перевірка кодування символу, відсутнього у словнику
TEST(HuffmanCodecTest, ThrowsOnUnknownSymbol) {
    HuffmanCodec codec("abc");
    
    try {
        codec.encode("abcd");
        FAIL() << "Expected UnknownSymbolException";
    }
    catch (const UnknownSymbolException& ex) {
        EXPECT_EQ(ex.symbol(), 'd');
    }
    catch (...) {
        FAIL() << "Expected UnknownSymbolException, but caught different exception";
    }
}

// 5. Перевірка декодування бітового рядка з некоректними символами
TEST(HuffmanCodecTest, ThrowsOnInvalidBitStringChar) {
    HuffmanCodec codec("abc");
    
    try {
        codec.decode("01201");
        FAIL() << "Expected InvalidBitStringException";
    }
    catch (const InvalidBitStringException& ex) {
        EXPECT_EQ(ex.index(), 2u);
        EXPECT_EQ(ex.invalidChar(), '2');
    }
    catch (...) {
        FAIL() << "Expected InvalidBitStringException";
    }
}

// 6. Перевірка неповного або пошкодженого бітового коду
TEST(HuffmanCodecTest, ThrowsOnIncompleteBitSequence) {
    HuffmanCodec codec("hello world");
    std::string encoded = codec.encode("hello");
    
    std::string truncated = encoded.substr(0, encoded.size() - 1);

    EXPECT_THROW(
        codec.decode(truncated),
        IncompleteCodeException
    );
}

// 7. Етап 7: Сильна гарантія безпеки (стан зберігається після помилки в buildTree)
TEST(HuffmanCodecTest, BuildTreePreservesStateOnException) {
    HuffmanCodec codec("initial state text");
    const std::size_t initialSymbols = codec.getUniqueSymbolsCount();

    EXPECT_THROW(codec.buildTree(""), std::invalid_argument);

    // Стан після винятку НЕ змінився!
    EXPECT_EQ(codec.getUniqueSymbolsCount(), initialSymbols);
    EXPECT_NO_THROW(codec.encode("initial"));
}

// 8. Етап 8: Перевірка виявлення дефекту у buildTreeUnsafe()
TEST(HuffmanCodecTest, UnsafeBuildTreeViolatesStrongGuarantee) {
    HuffmanCodec codec("initial state text");

    // Виняток виникне...
    EXPECT_THROW(codec.buildTreeUnsafe(""), std::invalid_argument);

    // ...але стан об'єкта зіпсовано! Кодек більше не ініціалізований.
    // Очікуємо, що стан зберігся (що ДЕФЕКТНИЙ метод buildTreeUnsafe НЕ виконує).
    // Тест має показати FAILED!
    EXPECT_TRUE(codec.isInitialized());
}