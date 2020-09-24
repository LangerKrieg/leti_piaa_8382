#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cin;

// Вычисление префикс-функции для строки text, возвращает вектор того же размера, что и строка
vector<int> prefixFunction(string& text) {
    vector<int> prefix(text.length());
    prefix[0] = 0;
    for (int i = 1; i < text.length(); i++) {
        int currentLength = prefix[i - 1];

        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        // Этот потенциальный размер суффикса по сути является значением prefix[currentLength - 1]
        while (currentLength > 0 && text[currentLength] != text[i]) {
            currentLength = prefix[currentLength - 1];
        }

        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (text[currentLength] == text[i]) {
            currentLength++;
        }
        prefix[i] = currentLength;
    }
    return prefix;
}

// Вывод индексов вхождений pattern в text, либо -1, если их нет
void findPattern(string& text, string& pattern) {
    vector<int> patternPrefix = prefixFunction(pattern);

    int textLength = text.length();
    int patternLength = pattern.length();
    int currentLength = 0;

    // chechOccurrence используется для организации вывода программы
    // Нужно для расстановки запятых или для вывода сообщения о том, что вхождений не встречено
    bool chechOccurrence = false;

    for (int i = 0; i < textLength; i++) {

        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        // Этот потенциальный размер суффикса по сути является значением pattern[currentLength - 1]
        while (currentLength > 0 && pattern[currentLength] != text[i]) {
            currentLength = patternPrefix[currentLength - 1];
        }

        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (pattern[currentLength] == text[i]) {
            currentLength++;
        }

        // Если длина текущего суффикса равна длине шаблона, найдено вхождение
        if (currentLength == patternLength) {
            if (!chechOccurrence) {
                cout << i - currentLength + 1;
                chechOccurrence = true;
            }
            else {
                cout << "," << i - currentLength + 1;
            }
        }
    }
    // Если не было встречено ни одного вхождение шаблона, вывод сообщения об этом
    if (!chechOccurrence) {
        cout << -1;
    }
}

int main() {
    string pattern;
    string text;
    cin >> pattern >> text;

    findPattern(text, pattern);

    return 0;
}

