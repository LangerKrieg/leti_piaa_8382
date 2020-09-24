#include <iostream>
#include <map>
#include <utility>
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::pair;
using std::move;

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

    vector<int> positionInPattern;

    BohrVertex(char name, int parent, int offset = -1, int patternNumber = -1, bool flag = false):
    name(name), parent(parent), offset(offset),patternNumber(patternNumber), flag(flag),suffLink(-1){}
};

class Bohr{
public:
    vector<BohrVertex> bohr;
    vector<string> patterns;
    string text;

    char joker{};
    int patternLenght{};
    vector<int> pos;
    void initBohrForJoker(char joker, string pattern, int textLenght);

    Bohr();
    void addPattern(const string& pattern, int positionInPattern);
    int getSuffLink(int index);
    int getAutoMove(int index, char symbol);
    void findAllPos(string textT);
};

Bohr::Bohr(){
    bohr.emplace_back('0', -1);
    bohr[0].suffLink = 0;
}

void Bohr::addPattern(const string& pattern, int positionInPattern = -1){
    char name;
    int indexBohr = 0;
    patterns.push_back(pattern);
    for(char i : pattern){
        name = i;

        auto it = bohr[indexBohr].nextVertex.find(name);
        if (it == bohr[indexBohr].nextVertex.end()){
            bohr[indexBohr].nextVertex.insert(pair<char, int>(name, bohr.size()));
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
    if(positionInPattern != -1){
        bohr[indexBohr].positionInPattern.push_back(positionInPattern);
    }
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
    auto it = bohr[vertexIndex].autoMove.find(symbol);
    if (it == bohr[vertexIndex].autoMove.end()){
        auto it2 = bohr[vertexIndex].nextVertex.find(symbol);
        if(it2 == bohr[vertexIndex].nextVertex.end()){
            if (vertexIndex == 0){
                bohr[vertexIndex].autoMove.insert(pair<char, int>(symbol, 0));
            }
            else{
                int move = getAutoMove(getSuffLink(vertexIndex), symbol);
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
    this->text = move(textT);
    int currentVertex = 0;

    for(int i  = 0; i <text.size(); i++){
        currentVertex = getAutoMove(currentVertex, text[i]);
        for(int j = currentVertex; j != 0; j = getSuffLink(j)){//check
            if(!bohr[j].positionInPattern.empty()){
                for(int & it : bohr[j].positionInPattern){
                    int posIndex = i - it + 1;
                    if(posIndex >= 0 && posIndex  + patternLenght - 1 < pos.size()){
                        pos[posIndex]++;
                    }
                }
            }
        }
    }
    for(int i = 0; i < pos.size(); i++){
        if(pos[i] == patterns.size()){
            cout << i + 1 << endl;
        }
    }
}

void Bohr::initBohrForJoker(char joker, std::string pattern, int textLenght){
    this->joker = joker;
    patternLenght = pattern.size();
    pos.clear();
    pos.resize(textLenght, 0);
    string tmpPattern;
    for(int i = 0; i < pattern.size(); i++){
        if(pattern[i] != joker){
            tmpPattern.push_back(pattern[i]);
        }
        else{
            if(!tmpPattern.empty()){
                addPattern(tmpPattern, i );
            }
            tmpPattern.clear();

        }
    }
    if(!tmpPattern.empty()){
        addPattern(tmpPattern, pattern.size());
    }
}

int main() {
    Bohr bor;
    string text;
    string patternJoker;
    char joker;
    cin >> text;
    cin >> patternJoker;
    cin >> joker;
    bor.initBohrForJoker(joker, patternJoker, text.size());
    bor.findAllPos(text);
    return 0;
}
