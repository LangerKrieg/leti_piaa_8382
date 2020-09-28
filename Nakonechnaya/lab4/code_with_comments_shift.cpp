#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::cin;

// Вычисление префикс-функции для строки text, возвращает вектор того же размера, что и строка
vector<int> createPrefix(string& text) {
    cout << "Поиск значений префикс-функции" << endl;
    vector<int> prefix(text.length());
    prefix[0] = 0;
    for (int i = 1; i < text.length(); i++) {
        int currentLength = prefix[i - 1];
        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        // Этот потенциальный размер суффикса по сути является значением prefix[currentLength - 1]
        cout << "Происходит вычисление значения префикс-функции для: " << text[i] << endl;
        while (currentLength > 0 && text[currentLength] != text[i]) {
            cout << "Предыдущий суффикс нельзя расширить, нужно взять суффикс меньшего размера" << endl;
            cout << "Размер суффикса: " << currentLength << endl;
            currentLength = prefix[currentLength - 1];
            cout << "Меняем на новый размер: " << currentLength << endl;
        }
        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (text[currentLength] == text[i]) {
            cout << "Можно расширить суффикс" << endl;
            cout << "Размер суффикса: " << currentLength << endl;
            currentLength++;
            cout << "Меняем на новый размер: " << currentLength << endl;
        }
        cout << "Значение префикс-функции для: " << text[i] << " = " << currentLength << endl;
        prefix[i] = currentLength;
    }
    return prefix;
}

// Если строка pattern является сдвигом text, выводится индекс начала вхождения, иначе -1
void shift(string& text, string& pattern) {
    if (text.length() != pattern.length()) {
        cout << "А не является циклическим сдвигом В, поэтому результат: ";
        cout << -1;
        return;
    }
    vector<int> prefix = createPrefix(pattern);
    int textLength = text.length();
    int curLength = 0;
    cout << "Начало основной проверки на циклический сдвиг после вычисления префикс-функции" << endl;
    for (int i = 0; i < textLength * 2; i++) {
        // j используется для циклического прохода по строке
        int j = i % textLength;
        // Если предыдущий суффикс нельзя расширить, нужно попытаться взять суффикс меньшего размера
        // Этот потенциальный размер суффикса по сути является значением pattern[currentLength - 1]
        while (curLength > 0 && pattern[curLength] != text[j]) {
            cout << pattern[curLength] << " != " << text[j] << " => Нельзя расширить суффикс, уменьшаем его" << endl;
            cout << "Размер суффикса: " << curLength << endl;
            curLength = prefix[curLength - 1];
            cout << "Меняем на новый размер: " << curLength << endl;
        }
        // Если символы справа от префикса и суффикса совпадают, суффикс расширяется
        if (pattern[curLength] == text[j]) {
            cout << pattern[curLength] << " == " << text[j] << " => Можно расширить суффикс" << endl;
            cout << "Размер суффикса: " << curLength << endl;
            curLength++;
            cout << "Меняем на новый размер: " << curLength << endl;
        }
        // Если длина текущего суффикса равна длине шаблона, найдено вхождение
        if (curLength == textLength) {
            cout << "Длина текущего суффикса: " << curLength << " == " << "Длине шаблона: " << textLength <<
            " => Строка является циклическим сдвигом, индекс строки: " << i - curLength + 1 << endl;
            return;
        }
    }
    cout << -1;
}

int main() {
    string A;
    string B;
    cout << "Введите строку А" << endl;
    cin >> A;
    cout << "Введите строку B" << endl;
    cin >> B;
    cout << "Начало функции циклического сдвига" << endl;
    shift(A, B);
    return 0;
}

