#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::pair;
using std::move;
using std::sort;

class BohrVertex{
public:
    bool flag;
    char name;
    int suffLink;
    int parent;
    int offset;
    int patternNumber;
    map<char, int> nextVertex;
    map<char,int> autoMove;

    BohrVertex(char name, int parent, int offset = -1, int patternNumber = -1, bool flag = false):
    name(name), parent(parent), offset(offset),patternNumber(patternNumber), flag(flag),suffLink(-1){}
};

class Bohr{
public:
    vector<BohrVertex> bohr;
    vector<string> patterns;
    map<int, vector<int>> result;

    string text;
    Bohr();
    void addPattern(const string& pattern);
    int getSuffLink(int index);
    int getAutoMove(int index, char symbol);
    void findAllPos(string textT);
    void printRes();
};


Bohr::Bohr(){
    bohr.emplace_back('0', -1);
    bohr[0].suffLink = 0;
}

void Bohr::addPattern(const string& pattern){
    char name;
    int indexBohr = 0;
    patterns.push_back(pattern);
    for(char i : pattern){
        name = i;
        auto it = bohr[indexBohr].nextVertex.find(name);
        if (it == bohr[indexBohr].nextVertex.end()){
            bohr[indexBohr].nextVertex.insert(std::pair<char, int>(name, bohr.size()));
            bohr.emplace_back(name, indexBohr);
            indexBohr = bohr.size()-1;
        }
        else{
            indexBohr = it->second;
        }
    }
    bohr[indexBohr].flag = true;
    bohr[indexBohr].patternNumber = patterns.size();
    bohr[indexBohr].offset = pattern.size();
}

int Bohr::getSuffLink(int vertexIndex){
    if(bohr[vertexIndex].suffLink == -1){
        if(vertexIndex == 0 || bohr[vertexIndex].parent == 0){
            bohr[vertexIndex].suffLink = 0;
            return 0;
        }
        else{
            bohr[vertexIndex].suffLink = getAutoMove(getSuffLink(bohr[vertexIndex].parent), bohr[vertexIndex].name);
        }
    }
    return bohr[vertexIndex].suffLink;
}

int Bohr::getAutoMove(int vertexIndex, char symbol){
    auto it =  bohr[vertexIndex].autoMove.find(symbol);
    if (it == bohr[vertexIndex].autoMove.end()){
        auto it2 = bohr[vertexIndex].nextVertex.find(symbol);
        if(it2 == bohr[vertexIndex].nextVertex.end()){
            if (vertexIndex == 0){
                bohr[vertexIndex].autoMove.insert(pair<char, int>(symbol, 0));
            }
            else{
                int move = getAutoMove(getSuffLink(vertexIndex), symbol);// ход автомата
                bohr[vertexIndex].autoMove.insert(pair<char, int>(symbol, move));
            }
        }
        else{
            bohr[vertexIndex].autoMove.insert(pair<char, int>(it2->first, it2->second));
        }
    }
    return bohr[vertexIndex].autoMove[symbol];
}

void Bohr::findAllPos(string textT){
    this->text = std::move(textT);
    int currentVertex = 0;
    for(int i  = 0; i <text.size(); i++){
        currentVertex = getAutoMove(currentVertex, text[i]);
        for(int j = currentVertex; j != 0; j = getSuffLink(j)){
            if(bohr[j].flag){
                result[i+2 - bohr[j].offset].push_back(bohr[j].patternNumber);
            }
        }
    }
}

void Bohr::printRes(){
    for(auto & it : result){
        sort(it.second.begin(), it.second.end());
    }
    if(!result.empty()){
        for(auto & it : result){
            for(auto it2 = it.second.begin(); it2 != it.second.end(); it2++){
                cout << it.first << " " << (*it2) << endl;
            }
        }
    }
}

int main() {
    Bohr bor;
    string text;
    string pattern;
    int countP = 0;
    cin >> text;
    cin >> countP;
    for(int i = 0; i < countP; i++){
        cin >> pattern;
        bor.addPattern(pattern);
    }
    bor.findAllPos(text);
    bor.printRes();

    return 0;
}
