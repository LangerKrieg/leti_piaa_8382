#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cin;

// Вычисление префикс-функции для строки text, возвращает вектор того же размера, что и строка
vector<int> createPrefix(string& text) {
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

// Если строка pattern является сдвигом text, выводится индекс начала вхождения, иначе -1
void shift(string& text, string& pattern) {
    if (text.length() != pattern.length()) {
        cout << -1;
        return;
    }
    vector<int> prefix = createPrefix(pattern);
    int textLength = text.length();
    int curLength = 0;
    for (int i = 0; i < textLength * 2; i++) {
        // j используется для циклического прохода по строке
        int j = i % textLength;
        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        // Этот потенциальный размер суффикса по сути является значением pattern[currentLength - 1]
        while (curLength > 0 && pattern[curLength] != text[j]) {
            curLength = prefix[curLength - 1];
        }
        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (pattern[curLength] == text[j]) {
            curLength++;
        }
        // Если длина текущего суффикса равна длине шаблона, найдено вхождение
        if (curLength == textLength) {
            cout << i - curLength + 1;
            return;
        }
    }
    cout << -1;
}

int main() {
    string A;
    string B;
    cin >> A;
    cin >> B;
    shift(A, B);
    return 0;
}

