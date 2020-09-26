#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <iomanip>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::pair;
using std::setw;
using std::move;
using std::sort;

class BohrVertex{
public:
    //флаг конечной, терминальной вершины
    bool flag;
    //имя вершины
    char name;
    //суффиксная ссылка
    int suffLink;
    //индекс в боре родительской вершины
    int parent;
    //длина паттерна (для конечной вершины)
    int offset;
    //номер паттерна (для конечной вершины)
    int patternNumber;

    //контейнер для вершин потомков
    //key - имя value - индекс в боре
    map <char, int> nextVertex;
    //возможность прохода из вершины
    //key - имя value - индекс в боре
    map <char, int> autoMove;
    //инициализация вершины бора
    BohrVertex(char name, int parent, int offset = -1, int patternNumber = -1, bool flag = false)
    :name(name), parent(parent), offset(offset),patternNumber(patternNumber), flag(flag),suffLink(-1){}
};

class Bohr{
public:
    vector<BohrVertex> bohr;
    //контейнер всех паттернов в боре
    vector<string> patterns;
    string text;
    //key - индекс value - номер образца
    map<int, vector<int>> result;

    int maxSuffLink; //максимальная длина цепочки из суффиксных ссылок
    int maxEndLink; //максимальная длина цепочки из конечных ссылок

    Bohr();
    //функция добавления паттерна в бор
    void addPattern(const string& pattern);
    //функция получения суффиксной ссылки
    int getSuffLink(int index);
    //функция перехода
    int getAutoMove(int index, char symbol);
    //функция поиска всех вхождений
    void findAllPos(string textT);
    //печать ответа
    void printRes();
    //печать бора
    void printBohr();
};

//конструктор
Bohr::Bohr(){
    //push_back создание корня
    bohr.emplace_back('0', -1);
    bohr[0].suffLink = 0;
    maxEndLink = 0;
    maxSuffLink = 0;
}

//функция добавления паттерна в бор
void Bohr::addPattern(const string& pattern){
    //рассматриваемый символ в паттерне
    char name;
    //текущая вершина в боре
    int currentVertex = 0;
    //добавление паттерна в вектор всех паттернов
    patterns.push_back(pattern);
    //проходимся по всему паттерну
    for(char i : pattern){
        name = i;
        //ищем вершину для перехода
        auto it = bohr[currentVertex].nextVertex.find(name);
        //переход невозможен -> создание новой вершины
        if (it == bohr[currentVertex].nextVertex.end()){
            bohr[currentVertex].nextVertex.insert(pair<char, int>(name, bohr.size()));
            bohr.emplace_back(name, currentVertex);
            currentVertex = bohr.size() - 1;
        }
        //переход по рассматриваемому символу возможен из текущей вершины
        //значит просто переходим к этой вершине
        else{
            currentVertex = it->second;
        }
    }
    //после добавления паттерна, последняя вершина получает флаг терминальной
    bohr[currentVertex].flag = true;
    //паттерн получает соответствующий номер
    bohr[currentVertex].patternNumber = patterns.size();
    //добавление к данным длины паттерна
    bohr[currentVertex].offset = pattern.size();
}

//функция получения суффиксной ссылки
int Bohr::getSuffLink(int vertexIndex){
    cout << endl << "Getting sufflink for vertex " << bohr[vertexIndex].name << " (" << vertexIndex << ")" << endl;
    //суффиксная ссылка еще не была определена
    if(bohr[vertexIndex].suffLink == -1){
        cout << "link not yet defined go to parent" << endl;
        //текущая вершина является корнем, либо же ее предок корень
        if(vertexIndex == 0 || bohr[vertexIndex].parent == 0){
            bohr[vertexIndex].suffLink = 0;
            cout << "Link from " << bohr[vertexIndex].name << " (" << vertexIndex << ") is " <<
            bohr[vertexIndex].suffLink << endl << endl;
            return 0;
        }
        //иначе шаг из суффиксной ссылки родителя
        else{
            bohr[vertexIndex].suffLink = getAutoMove(getSuffLink(bohr[vertexIndex].parent), bohr[vertexIndex].name);
        }
    }
    cout << "Link from " << bohr[vertexIndex].name << " (" << vertexIndex << ") is " <<
    bohr[vertexIndex].suffLink << endl << endl;
    return bohr[vertexIndex].suffLink;
}

//функция перехода
int Bohr::getAutoMove(int vertexIndex, char symbol){
    auto it = bohr[vertexIndex].autoMove.find(symbol);
    //нет перехода по данному символу из вершины
    if (it == bohr[vertexIndex].autoMove.end()){
        cout << "Search path from " << bohr[vertexIndex].name << " (" << vertexIndex << ") to "<< symbol << endl;
        auto it2 = bohr[vertexIndex].nextVertex.find(symbol);
        //прямого перехода не было найдено
        if(it2 == bohr[vertexIndex].nextVertex.end()){
            cout << "Go by suffLink (no child)" << endl;
            if (vertexIndex == 0){//в корень
                cout << "to the root" << endl;
                bohr[vertexIndex].autoMove.insert(pair<char, int>(symbol, 0));
            }
            //иначе по суффиксной ссылке символа
            else{
                //ход автомата
                int move = getAutoMove(getSuffLink(vertexIndex), symbol);
                bohr[vertexIndex].autoMove.insert(pair<char, int>(symbol, move));
            }
        }
        //был найден прямой переход
        else{
            bohr[vertexIndex].autoMove.insert(pair<char, int>(it2->first, it2->second));
        }
    }
    cout << "Path from " << bohr[vertexIndex].name << " (" << vertexIndex << ") to " << symbol << " is found " << endl;
    return bohr[vertexIndex].autoMove[symbol];
}

//функция поиска всех вхождений паттернов в текст
void Bohr::findAllPos(string textT){
    //this->text = textT
    this->text = move(textT);
    //текущая вершина в боре
    int currentVertex = 0;
    cout << "Pattern entry search started. Start from root" << endl;
    //проходимся по всему тексту
    for(int i = 0; i < text.size(); i++){
        cout << "//-------------------//" << endl << "Step by symbol in text: " << text[i] << endl;
        //шаг по автомату
        //можно ли перейти по символу текста из вершины
        currentVertex = getAutoMove(currentVertex, text[i]);

        int currentSuffLenght = 0;
        int currentEndLenght = 0;

        for(int j = currentVertex; j != 0; j = getSuffLink(j), currentSuffLenght++){//check
            if(bohr[j].flag){
                currentEndLenght++;
                result[i+2 - bohr[j].offset].push_back(bohr[j].patternNumber);
            }
        }
        maxEndLink = currentEndLenght>maxEndLink ? currentEndLenght : maxEndLink;
        maxSuffLink = currentSuffLenght>maxSuffLink ? currentSuffLenght : maxSuffLink;
    }
    cout << "//-------------------//" << endl << endl;
    printRes();
}

//функция для печати ответа
void Bohr::printRes(){
    //сортировка по номеру паттерна для одинаковых позиций в тексте
    for(auto & it : result){
        sort(it.second.begin(), it.second.end());
    }
    cout << "Result: " << endl << "Max suff link chain length: " << maxSuffLink <<
    endl << "Max end link chain length: " << maxEndLink << endl << "Positions:" << endl;
    //вывод результата
    if(!result.empty()){
        for(auto & it : result){
            for(auto it2 = it.second.begin(); it2 != it.second.end(); it2++){
                cout << "Offset in text: " << it.first << " Pattern №" << (*it2) << ": " << patterns[(*it2)-1] << endl;
            }
        }
    }
    else{
        cout << "there are no matches for your search!" << endl;
    }

}

//функция для печати получившегося бора
void Bohr::printBohr(){
    cout << endl << "Bohr: " << endl;
    cout << setw(8) << "Symbol" << setw(8) << "Index"<< setw(8) << "Parent" << setw(10) <<
    "  SuffLink  "<< setw(10) << "Sons (index)" << endl;
    for(int i = 0; i < bohr.size(); i++){
        cout << setw(8) << bohr[i].name << setw(8) << i << setw(8) << bohr[i].parent << setw(8)
        << bohr[i].suffLink << setw(8);
        for(auto & it : bohr[i].nextVertex){
            cout << it.second << " ";
        }
        cout << endl;
    }
}

int main() {
    //создание бора
    Bohr bor;
    //считывание информации(текст, кол-во образцв и сами образцы)
    string text;
    string pattern;
    int countP = 0;
    cin >> text;
    cin >> countP;
    //добавление паттернов к бору
    for(int i = 0; i < countP; i++){
        cin >> pattern;
        bor.addPattern(pattern);
    }
    //печать бора
    bor.printBohr();
    //поиск позиций всех образцов в тексте
    bor.findAllPos(text);
    return 0;
}

