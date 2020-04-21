#include <iostream>
#include <map>
#include <list>
#include <vector>

char min(std::map <char, std::map <char, float>> info, std::list <char> open, char previous){
    char res = previous;
    float min = 1000;

    while (!open.empty()){
        if (info[previous][open.front()] < min && info[previous][open.front()]){
            res = open.front();
            min = info[previous][open.front()];
        }
        open.erase(open.begin());
    }
    return res;
}

std::vector <char> neighbours(std::map <char, std::map <char, float>> info, char curr, char minPeak, char maxPeak){
    std::vector <char> N;
    for (char i = minPeak; i <= maxPeak; i++){
        if (info[curr][i]){
            N.push_back(i);
        }
    }
    return N;
}

bool inList(std::list <char> l, char currN){
    while(!l.empty()){
        if (currN == l.back()){
            return true;
        }
        l.pop_back();
    }
    return false;
}

void reconstruction(std::map <char, char> from, char begin, char end){
    char curr = end;
    std::list <char> path;
    path.push_front(curr);
    while(curr != begin){
        curr = from[curr];
        path.push_front(curr);
    }
    for (auto it : path){
        std::cout << it;
    }
}

bool greedy(std::map <char, std::map <char, float>> info, char begin, char end, char minPeak, char maxPeak){
    std::list <char> close;
    std::list <char> open = {begin};
    std::map <char, char> from;
    std::vector <char> N;
    char previous = begin;

    while(!open.empty()) {
        char curr = min(info, open, previous);
        if (curr == end){
            reconstruction(from, begin, end);
            return true;
        }
        open.remove(curr);
        close.push_back(curr);
        N = neighbours(info, curr, minPeak, maxPeak);
        for (auto it : N){
            if (inList(close, it)){
                continue;
            }
            if (!inList(open, it)){
                open.push_back(it);
            }
            from[it] = curr;
        }
        if (!N.empty()) {
            previous = curr;
        }
        else{
            for (char i = minPeak; i <= maxPeak; i++){
                if (info[i][curr]){
                    previous = i;
                }
                info[i][curr] = 0;
            }
        }
    }

    return false;
}

int main(){
    char out = 0, in = 0, begin = 0, end = 0, maxPeak = 0, minPeak = 0;
    float weight = 0;

    std::cin >> begin;
    std::cin >> end;

    std::map<char, std::map<char, float>> info;

    int k = 0;
    while (std::cin) {
        std::cin >> out;
        std::cin >> in;
        std::cin >> weight;

        if (minPeak == 0) {
            minPeak = begin;
        }
        if (maxPeak == 0) {
            maxPeak = end;
        }
        if (in > maxPeak) {
            maxPeak = in;
        }
        if (out > maxPeak) {
            maxPeak = out;
        }
        if (in < minPeak) {
            minPeak = in;
        }
        if (out < minPeak) {
            minPeak = out;
        }

        info[out][in] = weight;
        k++;
    }

    bool res = greedy(info, begin, end, minPeak, maxPeak);

    return 0;
}