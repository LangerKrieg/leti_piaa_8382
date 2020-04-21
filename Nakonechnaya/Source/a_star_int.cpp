#include <iostream>
#include <map>
#include <list>
#include <vector>

std::map <int, float> heuristic(int minPeak, int maxPeak, int end){ //подсчёт значений эвристической функции
    std::map <int, float> H;
    std::cout << "Значения эвристической функции: " << std::endl;
    for (int i = minPeak; i <= maxPeak; i++){
        if (i > end){ //подсчитывается близость вершин к конечной вершине
            H[i] = i - end;
        }
        else{
            H[i] = end - i;
        }
        std::cout << H[i] << " ";
    }
    std::cout << std::endl;
    return H;
}

int minF(std::list <int> open, std::map <int, float> F){//поиск минимального значения f(x)
    int res = open.back();
    float min = F[res];
    open.pop_back();

    while (!open.empty()){
        if (F[open.back()] <= min){
            res = open.back();
            min = F[open.back()];
        }
        open.pop_back();
    }
    return res;
}

std::vector <int> neighbours(std::map <int, std::map <int, float>> info, int curr, int minPeak, int maxPeak){ //поиск соседей
    std::vector <int> N;
    std::cout << "Вершины соседние с " << curr << std::endl;
    for (int i = minPeak; i <= maxPeak; i++){
        if (info[curr][i]){ //если существует связь между вершинами
            N.push_back(i);
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return N;
}

bool inList(std::list <int> l, int currN){ //проверка на присутствие вершины в списке
    while(!l.empty()){
        if (currN == l.back()){
            return true;
        }
        l.pop_back();
    }
    return false;
}

void reconstruction(std::map <int, int> from, int begin, int end){ //реконструкция пути
    int curr = end;
    std::list <int> path;
    path.push_front(curr);
    while(curr != begin){
        curr = from[curr];
        path.push_front(curr);
    }
    std::cout << "Результат: " << std::endl;
    for (auto it : path){
        std::cout << it;
    }
    std::cout << std::endl;
}

void printList(std::list <int> l){
    while(!l.empty()){
        std::cout << l.back() << " ";
        l.pop_back();
    }
    std::cout << std::endl;
}

bool aStar(std::map <int, std::map <int, float>> info, int begin, int end, int minPeak, int maxPeak){
    std::list <int> close; //список пройденных вершин
    std::list <int> open = {begin}; //список рассматриваемых вершин
    std::map <int, int> from; //используется для восстановления пути
    std::vector <int> N; //контейнер с соседями
    std::map <int, float> G; //хранит стоимости путей от начальной вершины
    G[begin] = 0;
    std::map <int, float> H = heuristic(minPeak, maxPeak, end); //контейнер с значениями эвристической функции
    std::map <int, float> F; //оценки f(x) для каждой вершины
    F[begin] = G[begin] + H[begin];

    while(!open.empty()) {
        int curr = minF(open, F);

        std::cout << "Текущая вершина: " << curr << std::endl;
        std::cout << "Рассматриваемые вершины: " << std::endl;
        printList(open);
        std::cout << "Пройденные вершины: " << std::endl;
        printList(close);
        std::cout << "Стоимости путей от начальной вершины: " << std::endl;
        for (auto it : G){
            std::cout << "Вершина: " << it.first << " g(x): " << it.second << std::endl;
        }
        for (auto it : F){
            std::cout << "Вершина: " << it.first << " f(x): " << it.second << std::endl;
        }
        std::cout << "________________________" << std::endl;
        std::cout << std::endl;

        if (curr == end){ //если дошли до конца
            reconstruction(from, begin, end);
            return true; //путь найден
        }

        open.remove(curr);
        close.push_back(curr);

        N = neighbours(info, curr, minPeak, maxPeak);
        for (auto it : N){
            if (inList(close, it)){ //если вершина была пройдена
                continue;
            }
            bool tmpFlag; //обновление свойств текущего соседа
            float tmpG = G[curr] + info[curr][it]; //вычисление g(x) для обрабатываемого соседа
            if (!inList(open, it)){ //если сосед текущей вершины не находится в списке рассматриваемых вершин
                open.push_back(it);
                tmpFlag = true;
            }
            else{
                if (tmpG < G[it]){ //если вычисленная стоимость меньше
                    tmpFlag = true;
                }
                else{
                    tmpFlag = false;
                }
            }
            if (tmpFlag){ //обновление свойств соседа
                from[it] = curr;
                G[it] = tmpG;
                F[it] = G[it] + H[it];
            }
        }
    }
    return false; //путь не найден
}

int main(){
    int out = 0, in = 0, begin = 0, end = 0, maxPeak = 0, minPeak = 0;
    float weight = 0;

    std::cin >> begin;
    std::cin >> end;

    std::map<int, std::map<int, float>> info; //контейнер хранит связи графа

    int k = 0;
    while (std::cin) { //Для прекращения работы цикла следует нажать ctrl + D

        std::cin >> out;
        std::cin >> in;
        std::cin >> weight;

        if (weight < 0){
            std::cout << "Ошибка: вес не может быть отрицательным";
            return 0;
        }

        if (minPeak == 0) { //инициализируем минимальную вершину
            minPeak = begin;
        }
        if (maxPeak == 0) { //инициализируем максимальную вершину
            maxPeak = end;
        }
        if (in > maxPeak) { //если вершина in или out больше чем максимальная, то она становится максимальной
            maxPeak = in;
        }
        if (out > maxPeak) {
            maxPeak = out;
        }
        if (in < minPeak) { //если вершина in или out меньше чем минимальная, то она становится минимальной
            minPeak = in;
        }
        if (out < minPeak) {
            minPeak = out;
        }

        info[out][in] = weight;
        k++;
    }

    std::cout << std::endl;
    for(int i = minPeak; i <= maxPeak; i++){
        for(int j = minPeak; j <= maxPeak; j++) {
            if (info[i][j])
                std::cout << "Из вершины: " << i << " в вершину: " << j << " ребро: " << info[i][j] << std::endl;
        }
    }
    std::cout << std::endl;

    bool res = aStar(info, begin, end, minPeak, maxPeak);

    if (!res){
        std::cout << "Ошибка: путь не был найден\n";
    }

    return 0;
}
