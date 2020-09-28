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
    //для подсчёта кол-ва блоков при их поиске

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

    //функция поиска индекса elem в результирующем векторе
    bool find(int elem){
        for(int i : result){
            if(i == elem)
                return true;
        }
        return false;
    }

    //разрезание исходного текста на блоки
    void trainingText(){
        cout << "Исходный текст: ";
        printText();
        cout << "Количество блоков равно: " << numBlock << endl;
        //изменение размера вектора
        blocks.resize(numBlock);
        //минимальный размер блока
        sizeBlock = floor(sizeText / numBlock);
        //максимальный размер блока = мин размер + размер пересечений
        if(sizePattern != 1)
            maxSizeBlock = sizeBlock + (sizePattern - 1);
        //максимальный размер блока для 1 - это размер текста
        else
            maxSizeBlock = sizeBlock;
        //временные переменные, необходимые для разрезания текста на части
        string temp;
        int index;
        cout << "Максимальный размер блока: " << maxSizeBlock << endl;
        //разрезание на блоки
        for(int i = 0; i < numBlock; i++){
            index = sizeBlock * i;
            //отрезаем от исходного текста необходимый блок
            //отрез идёт от определённого индекса, отрезается наибольшая возможная часть
            for(int j = index; j < maxSizeBlock + index; j++){
                //если ещё есть, что отрезать, сохраняем часть текста к отрезу
                if(j < sizeText)
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
            //вывод получившихся блоков
            cout << "Блок " << i + 1 << ": ";
            printBlock(i);
            cout << endl;
        }
    }

    //формирование массива префикс-функции
    void createPrefix(){
        cout << "Обрабатываемый шаблон: ";
        printPattern();
        //j, i - указывают на рассматриваемые символы
        int j = 0;
        int i = 1;
        //prefixFunc[0] = 0 - всегда для начального символа
        prefix.push_back(0);

        //пока не просмотрели весь шаблон
        while(i < sizePattern){
            cout << "Обработка: шаблон[" << j << "] = " <<  pattern[j];
            cout << " и шаблон[" << i << "] = " <<  pattern[i] << endl;
            //если символ повторяется
            if(pattern[i] == pattern[j]){
                cout << "Найдено совпадение символов" << endl;
                cout << "prefixFunc[" << i << "] = " << j + 1 << endl;
                //фиксируем полученное значение в префикс-функции
                prefix[i] = j + 1;
                //и двигаемся дальше
                i++;
                j++;
            }
            else{
                cout << "Найдено несовпадение символов";
                //j == 0
                if(!j){
                    cout << "prefixFunc[" << i << "] = 0" << endl;
                    prefix[i] = 0;
                    i++;
                }
                //если j не указывает на начало суффикса
                else{
                    cout << "j = prefixFunc[" << j - 1 << "] = " << prefix[j-1] << endl;
                    //присваеваем значения префикс-функции предыдущего символа, на который указывает j
                    j = prefix[j - 1];
                }
            }
        }
        cout << "Значения полученной префикс-функции: " << endl;
        printPrefix();
    }

    //поиск вхождений шаблона в текст
    void patternSearch(string block, int index){
        //T - текст P - шаблон
        //начинаем поиск из начала текста и шаблона
        int indexT = 0;
        int indexP = 0;
        cout << endl << "Шаблон: ";
        printPattern();
        cout << "Блок: ";
        printBlock(index);

        //пока не просмотрели весь текст
        while(indexT != block.length()) {
            //проверка на выход из размера шаблона indexP, если индекс вышел за предел размера, то возврат
            //к самому началу шаблона
            if (indexP > sizePattern - 1){
                indexP = 0;
            }
            //проверка на выход за пределы текста
            if (indexT > block.length())
                break;
            cout << endl;
            cout << "Символы для сравнения: шаблон[" << indexP << "] = " << pattern[indexP];
            cout << " и блок[" << indexT << "] = " << block[indexT];
            cout << endl;
            // если нашли совпадение
            if(pattern[indexP] == block[indexT]){
                cout << "Найдено совпадение символов" << endl;
                //двигаемся дальше
                indexT++;
                indexP++;
                //достигнут конец шаблона
                if(indexP == sizePattern){
                    cout << "НАЙДЕНО ВХОЖДЕНИЕ ШАБЛОНА В ТЕКСТ: ";
                    //индекс, на котором был найден паттерн
                    cout << indexT + (sizeBlock * index) - (sizePattern);
                    //в результирующем векторе уже есть данный индекс
                    if(find(indexT+(sizeBlock * index) - (sizePattern)))
                        cout << "Вхождение уже записано" << endl;
                    //фиксируем индекс вхождения найденного шаблона
                    else
                        result.push_back(indexT+(sizeBlock * index) - (sizePattern));
                    //indexT возвращаем на следующий индекс после нахождения последнего паттерна, чтобы не пропустить
                    //ничего. Пример, тест: pattern - "aba", text - "abababababa", numBlock = 1
                    if(sizePattern != 1)
                        indexT = indexT + (sizeBlock * index) - (sizePattern) + 1;
                }
            }
            else{
                cout << "Символы не совпадают" << endl;
                //indexP == 0
                if(!indexP)
                    //сдвигаемся по тексту
                    indexT++;
                //перемещаемся на элемент с индексом = префикс-функции предыдущего элемента
                else
                    indexP = prefix[indexP - 1];
            }
        }
    }

    //промежуточная функция для работы с остальными элементами кода, организатор алгоритма
    void algorithm() {
        //разбиение текста
        trainingText();
        //изменение размера массива для хранения значений префикс-функции
        prefix.resize(pattern.length() - 1);
        //вычисление префикс-функции
        createPrefix();
        //для всех блоков
        for (int i = 0; i < numBlock; i++) {
            cout << endl << "Обрабатываемый блок: " << i + 1 << ") ";
            printBlock(i);
            //поиск шаблона в текущем блоке текста
            patternSearch(blocks[i], i);
        }
        cout << endl;
        cout << endl << endl << "РЕШЕНИЕ: "<< endl;
        printAnswer();
    }

public:
    void readData(){
        //ввод информации
        string P;
        string T;
        cout << "Введите шаблон: " << endl;
        cin >> P;
        cout << "Введите текст: " << endl;
        cin >> T;
        //инициализация паттерна и текста
        setPattern(P);
        setText(T);
        //вычисление размера паттерна и текста
        sizePattern = (int)pattern.length();
        sizeText = (int)text.length();

        //считывание кол-ва блоков
        int B;
        cout << "Введите количество блоков: " << endl;
        cin >> B;
        int flag = 0;

        //проверка кол-ва блоков
        while(!flag){
            //если кол-во блоков меньше= 0;
            //или размер паттерна = 1, тогда кол-во блоков не может быть больше размера текста
            if(B <= 0 || (sizePattern == 1 && B > sizeText)){
                cout << "Некорректное количество блоков" << endl;
                cin >> B;
            }
            //ввод блоков оказался верным
            else{
                flag = 1;
            }
        }
        //инициализация кол-ва блоков
        setBlock(B);
        //вызов алгоритма
        algorithm();
    }

    //функция для печати исходного текста
    void printText(){
        for(char i : text){
            cout << i;
        }
        cout << endl;
    }

    //функция для печати вектора с блоками
    void printBlock(int index){
        cout << blocks[index];
    }

    //функция для печати шаблона
    void printPattern(){
        for(char i : pattern){
            cout << i;
        }
        cout << endl;
    }

    //функция для печати префикс-функции
    void printPrefix(){
        for(int i = 0; i < prefix.size(); i++){
            cout <<"Шаблон[" << i << "] = " << pattern[i] << " имеет prefixFunc[" << i << "] = " << prefix[i] << endl;
        }
        cout << endl;
    }

    //функция для печати результата
    void printAnswer(){
        cout << "Вхождения шаблона в текст ";
        if(result.empty())
            cout << "не найдены: -1" << endl;
        else{
            cout << "найдены: " ;
            for(int k = 0; k < result.size(); k++){
                if(result.size() - 1 != k)
                    cout << result[k] << ",";
                else
                    cout << result[k] << endl;
            }
        }
    }
};

int main() {
    //класс, реализующий КМП
    Base start;
    //начало считывания данных
    start.readData();
    return 0;
}

