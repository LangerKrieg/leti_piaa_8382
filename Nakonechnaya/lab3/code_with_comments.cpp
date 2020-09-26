#include <iostream>
#include <vector>
#include <stack>
#include <climits>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stack;
using std::min;

//структура для ребра
struct Edge{
    char fromTop;//идет из вершины
    char toTop;//идет в вершину
    int weight; //вес ребра
};

//сортировка вершин
bool compare(Edge a, Edge b){
    if (a.fromTop < b.fromTop) return true;
    else if (a.fromTop == b.fromTop) {
        if (a.toTop < b.toTop) return true;
    }
    return false;
}

//поиск в глубину
bool dfs(const vector<vector<int>>& Graph, int s, int t, vector<int>&parent, int vertexesNum, const string& nodes){
    //массив флагов посещаемости вершин
    //создаем стек
    vector<bool>visited(vertexesNum,false);
    stack<int>st;
    //кладем исходную вершину в стек
    st.push(s);

    //посетили вершину
    visited[s] = true;
    //исток является начальной вершиной
    parent[s] = -1;

    //обработка, пока стек не пуст
    while (!st.empty()) {
        //обработка первой вершины
        int i = st.top();
        st.pop();
        //если смежная вершина не обработана и имеет ребро с обрабатываемой вершиной
        for(int j = 0 ; j < vertexesNum; j++){
            if(Graph[i][j] > 0 && !visited[j]){
                cout << "Current vertex is: " << nodes[j] << endl;
                //добавляем смежную вершину
                st.push(j);
                //в пути инициализируем смежную вершину и делаем её посещённой
                parent[j] = i;
                visited[j] = true;
            }
        }
    }
    return visited[t];
}

int fordFulkerson(vector<vector<int>>& graph, vector<vector<int>>& Graph, int s, int t, int vertexesNum, const string& nodes) {
    int u, v;
    //graph обнуляется, в дальнейшем будет использован для ответа
    cout << "Zeroing a graph" << endl;
    for (u = 0; u < vertexesNum; u++) {
        for (v = 0; v < vertexesNum; v++) {
            Graph[u][v] = graph[u][v];
            graph[u][v] = 0;
        }
    }
    //изначально поток = 0
    int maxFlow = 0;
    // массив для хранения пути
    vector<int>parent(vertexesNum, 0);
    //увеличивается поток, пока есть путь от истока к стоку
    cout << "Start searching for paths from start to finish" << endl;
    while (dfs(Graph, s, t, parent, vertexesNum, nodes)) {

        int pathFlow = INT_MAX;
        //выбор минимального значения пропускной способности для обновления прорускной способности каждого ребра
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            pathFlow = min(pathFlow, Graph[u][v]);
        }

        //обновление пропускной способности каждого ребра
        cout << "Now the throughput is as follows: " << endl;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            Graph[u][v] -= pathFlow;
            Graph[v][u] += pathFlow;
            graph[u][v] += pathFlow;
            graph[v][u] -= pathFlow;
            cout << "Graph[" << nodes[u] << "][" << nodes[v] << "] = " << Graph[u][v] << endl;
            cout << "Graph[" << nodes[v] << "][" << nodes[u] << "] = " << Graph[v][u] << endl;
            cout << "graph[" << nodes[u] << "][" << nodes[v] << "] = " << graph[u][v] << endl;
            cout << "graph[" << nodes[v] << "][" << nodes[u] << "] = " << Graph[v][u] << endl;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main() {
    //исток
    char start;
    //сток
    char finish;
    //ребро графа
    char tempFrom;
    char tempTo;
    //количество ориентированных рёбер графа
    int N = 0;
    //вес графа
    int weight;
    //строки, содержащие пути
    //вершины, из которых пришли
    string from;
    //вершины, в которые вошли
    string to;
    //полный путь
    string nodes;
    //ввод информации
    cin >> N >> start >> finish;
    if (start == finish) {
        cout << 0;
        return 0;
    }
    //массив с весами рёбер
    vector <int> weightVector;
    //добавление начального узла
    nodes = nodes + start;
    for (int i = 0; i < N; i++) {
        //считывание рёбер и веса
        cin >> tempFrom;
        cin >> tempTo;
        cin >> weight;
        cout << endl << "Adding an edge which goes from: " << tempFrom << " to: " << tempTo << " with weight: " << weight;
        //добавление вершины, из которой выходит ребро
        from += tempFrom;
        //добавление вершины, в которую входит ребро
        to += tempTo;
        //добавление веса в массив
        weightVector.push_back(weight);
        //если не будет найдена вершина, в которую идёт ребро, то добавляем
        if (nodes.find(tempTo) == string::npos) {
            nodes += tempTo;
        }
    }
    //сортировка строки с вершинами
    sort(nodes.begin(), nodes.end());
    cout << endl;
    cout << "Line view with sorted vertexes: " << nodes << endl;
    //кол-во узлов
    int vertexesNum = nodes.length();
    cout << "Number of nodes: " << vertexesNum << endl;
    //создаём граф
    //двумерный вектор
    vector<vector<int>> graph(vertexesNum, vector<int>(vertexesNum, 0));

    cout << "Graph creation in progress . . ." << endl;
    //поиск всех ребер, ведущих из вершины nodes[q]
    for (int q = 0; q < nodes.length(); q++) {
        vector <int> Temp;
        for (int j = 0; j < N; j++) {
            if (from[j] == nodes[q]) {
                Temp.push_back(j);
            }
        }
        //поиск в строке nodes[q] вершины, в которую ведут ребра из вектора temp
        vector <int> nodesTemp;
        for (int i : Temp) {
            for (int j = 0; j < nodes.length(); j++) {
                if (nodes[j] == to[i])
                    nodesTemp.push_back(j);
            }
        }
        //дополнение графа весом ребра
        for (int i = 0; i < Temp.size(); i++) {
            graph[q][nodesTemp[i]] = weightVector[Temp[i]];
        }

        //печать графа
        for (int i = 0; i < Temp.size(); i++) {
            cout << "From vertex: " << q << " to vertex: " << nodesTemp[i] << " weight is: " << weightVector[Temp[i]] << endl;
        }
    }
    //индексы стока и истока
    int startIndex = 0;
    int finishIndex = 0;
    for (int i = 0; i < vertexesNum; i++) {
        if (nodes[i] == start)
            startIndex = i;
        else if (nodes[i] == finish)
            finishIndex = i;
    }
    cout << "Start index is: " << startIndex << " Finish index is: " << finishIndex << endl;
    //граф смежности
    vector<vector<int>> Graph(vertexesNum, vector<int>(vertexesNum, 0));
    //нахождение максимального потока
    cout << "Begin of Ford-Fulkerson algorithm" << endl;
    int maxFlow = fordFulkerson(graph, Graph, startIndex, finishIndex, vertexesNum, nodes);
    cout << "End of Ford-Fulkerson algorithm" << endl;

    vector <Edge> One;
    //проходимся по всем вершинам
    for (int i = 0; i < vertexesNum; i++){
        //составление индексов
        vector <int> pointer;
        //проходимся по всем ориентированным рёбрам
        for (int j = 0; j < N; j++) {
            //если вершина является той, из которой выходит ребро
            if (nodes[i] == from[j])
                //то добавляем новый индекс
                pointer.push_back(j);
        }
        //пробегаем по всем полученным индексам
        for (int j : pointer) {
            //находим переменный значения, чтобы получить доступ к graph,
            //где хранятся необходимые фактические величины потока
            Edge edges{};
            edges.fromTop = from[j];
            edges.toTop = to[j];
            int tempF = 0;
            int tempT = 0;
            for (int k = 0; k < vertexesNum; k++) {
                if (nodes[k] == from[j])
                    tempF = k;
                else if (nodes[k] == to[j])
                    tempT = k;
            }
            if (graph[tempT][tempF] >= 0)
                edges.weight = 0;
            else
                edges.weight = abs(graph[tempT][tempF]);
            //запоминаем фактическую величину потока, необходимую для вывода
            One.push_back(edges);
        }
    }
    //сортируем данные
    sort(One.begin(), One.end(), compare);
    cout << "Max flow is: ";
    cout << maxFlow << endl;
    for (auto & i : One){
        cout << i.fromTop << " " << i.toTop << " " << i.weight << endl;
    }
    return 0;
}

