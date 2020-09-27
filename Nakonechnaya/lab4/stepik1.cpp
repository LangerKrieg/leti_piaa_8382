#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::move;
using std::vector;

//класс, реализующий КМП
class Base{
private:
    //длина шаблона
    int sizePattern{};
    //длина текста
    int sizeText{};
    //кол-во блоков для разделения текста
    int numBlock{};
    //размер блока
    int sizeBlock{};
    //максимальный размер обрабатываемого блока
    int maxSizeBlock{};
    //шаблон
    string pattern;
    //текст
    string text;
    //префикс-функция
    vector <int> prefix;
    //результирующий вектор
    vector <int> result;
    //для хранения блоков
    vector <string> blocks;

    //функции-сеттеры
    void setPattern(string P){
        pattern = move(P);
    }

    void setText(string T){
        text = move(T);
    }

    void setBlock(int B){
        numBlock = B;
    }

//разрезание исходного текста на блоки
    vector<string> trainingText(){
        //минимальный размер блока
        sizeBlock = floor(sizeText / numBlock);
        //максимальный размер блока = мин размер + размер пересечений
        if(pattern.length() != 1)
            maxSizeBlock = sizeBlock + (pattern.length() - 1);
            //максимальный размер блока для 1 - это размер текста
        else
            maxSizeBlock = sizeBlock;
        //временные переменные, необходимые для разрезания текста на части
        string temp;
        int index;
        //разрезание на блоки
        for(int i = 0; i < numBlock; i++){
            index = sizeBlock * i;
            //отрезаем от исходного текста необходимый блок
            //отрез идёт от определённого индекса, отрезается наибольшая возможная часть
            for(int j = index; j < maxSizeBlock + index; j++){
                //если ещё есть, что отрезать, сохраняем часть текста к отрезу
                if(j < text.length())
                    temp += text[j];
                    //последний блок может быть меньше максимального размера,
                    // поэтому есть возможность добавить просто остаток
                else
                    break;
            }
            //кладем в вектор блоков
            blocks[i] = temp;
            //очищаем временную переменную для следующей итерации
            temp.clear();
        }
        return blocks;
    }

    //формирование массива префикс-функции
    void prefixFunction(){
        //j, i - указывают на рассматриваемые символы
        int j = 0;
        int i = 1;
        //prefixFunc[0] = 0 - всегда для начального символа
        prefix.push_back(0);

        //пока не просмотрели весь шаблон
        while(i < pattern.length()){
            //если символ повторяется
            if(pattern[i] == pattern[j]){
                //фиксируем полученное значение в префикс-функции
                prefix[i] = j + 1;
                //и двигаемся дальше
                i++;
                j++;
            }
            else{
                //j == 0
                if(!j){
                    prefix[i] = 0;
                    i++;
                }
                    //если j не указывает на начало суффикса
                else{
                    //присваеваем значения префикс-функции предыдущего символа, на который указывает j
                    j = prefix[j - 1];
                }
            }
        }
    }

    // Вывод индексов вхождений pattern в text, либо -1, если их нет
    void patternSearch(string block) {
        int currentLength = 0;
        bool chechOccurrence = false;
        int indexT = 0;
        //пока не просмотрели весь текст
        while (indexT != block.length()) {
            //проверка на выход из размера шаблона indexP, если индекс вышел за предел размера, то возврат
            //к самому началу шаблона
            while (currentLength > 0 && pattern[currentLength] != block[indexT]) {
                currentLength = prefix[currentLength - 1];
            }
            if (pattern[currentLength] == block[indexT]) {
                currentLength++;
            }
            if (currentLength == sizePattern) {
                if (!chechOccurrence) {
                    cout << indexT - currentLength + 1;
                    chechOccurrence = true;
                }
                else {
                    cout << "," << indexT - currentLength + 1;
                }
            }
            indexT++;
        }
        if (!chechOccurrence) {
            cout << -1;
        }
    }

    //промежуточная функция для работы с остальными элементами кода, организатор алгоритма
    void algorithm() {
        blocks.resize(numBlock);
        //разбиение текста
        trainingText();
        //изменение размера массива для хранения значений префикс-функции
        prefix.resize(pattern.length() - 1);
        //вычисление префикс-функции
        prefixFunction();
        //для всех блоков
        for (int i = 0; i < numBlock; i++) {
            //поиск шаблона в текущем блоке текста
            patternSearch(blocks[i]);
        }
    }

public:
    void readData(){
        //ввод информации
        string P;
        string T;
        cin >> P;
        cin >> T;
        int B = 1;
        //инициализация паттерна и текста
        setPattern(P);
        setText(T);
        //вычисление размера паттерна и текста
        sizePattern = (int)pattern.length();
        sizeText = (int)text.length();
        //инициализация кол-ва блоков
        setBlock(B);
        //вызов алгоритма
        algorithm();
    }
};

int main() {
    //класс, реализующий КМП
    Base start;
    //начало считывания данных
    start.readData();
    return 0;
}
