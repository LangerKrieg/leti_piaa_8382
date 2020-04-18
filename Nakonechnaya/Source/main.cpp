#include <iostream>
#include <stack>
#include <cmath>

//обычный квадрат, используемый в разбиении
struct Square{
    int x;
    int y;
    int size;
};

//обязательный квадрат, используемый в разбиении
struct ReqSquare{
    int x;
    int y;
    int size;
};

//функция для нахождения делителя непростого числа
int primeOrNot(int tabletopSize){
    if (tabletopSize % 2 == 0) { //если наименьший делитель столешницы 2
        return 2;
    }
    if (tabletopSize % 3 == 0) { //если наименьший делитель столешницы 3
        return 3;
    }
    if (tabletopSize % 5 == 0) { //если наименьший делитель столешницы 5
        return 5;
    }
    return 1;
}

//функция установки квадрата в массив
void insertSquare(int **arr, int x, int y, int lengthSquare) {
    for (int i = 0; i < lengthSquare; i++) {
        for (int j = 0; j < lengthSquare; j++) {
            arr[x + i][y + j] = lengthSquare;
        }
    }
}

//функция копирования стека
void stackCopy(std::stack <Square>* squares, std::stack <Square>* squaresCopy) {
    while (!squares->empty()) {
        squaresCopy->push(squares->top());
        squares->pop();
    }
}

//функция нахождения пустого пространства для вставки квадрата
bool findEmptySpace(int** arr, int& x, int& y, int size) {
    for (x = 0; x < size; x++) {
        for (y = 0; y < size; y++) {
            if (arr[x][y] == 0) { //если нашли пустое место
                return true;
            }
        }
    }
    return (x == size) ? (false) : (true);//не нашли пустое место - false, иначе true
}

//функция находит максимальную длину квадрата, которую можно вставить
int findLength(int** arr, int& x, int& y, int size) {
    //находим расстояние до границы
    int distanceToBorder = (size - x > size - y) ? (size - y - !arr[0][0]) : (size - x - !arr[0][0]);
    int maxLength = 2; //начальная длина квадрата
    for (; maxLength <= distanceToBorder; maxLength++) {
        //пока длина квадрата меньше или равна длине до границы, увеличиваем длину квадрата
        for (int i = 0; i < maxLength; i++) {
            if (arr[x + maxLength - 1][y + i] || arr[x + i][y + maxLength - 1])
                return maxLength - 1;
        }
    }
    return maxLength - 1;
}

// рекурсивная функция для перебора возможных значений расстановки квадратов
int minSquares(int limit, std::stack <Square>* squares, int** arr, int size, int recursionCounter) {
    if (limit < recursionCounter) //если предел рекурсии меньше кол-ва входов рекурсии
        return -1;
    int x, y;
    if (!findEmptySpace(arr, x, y, size)) //если не нашли пустое пространство
        return 0;

    std::stack <Square> tmpSquares;
    std::stack <Square> maxSquares;

    int maxLength = findLength(arr, x, y, size); //находим максимальную длину квадрата, который можно поместить
    //в свободное пространство

    int tmpLength;
    int minNum = limit;
    int tmp;
    int desiredLength = 1;

    for (tmpLength = 1; tmpLength <= maxLength; tmpLength++) {
        insertSquare(arr, x, y, tmpLength);
        //рекурсия
        tmp = minSquares(limit, &tmpSquares, arr, size, recursionCounter + 1) + 1;

        if (tmp <= minNum && tmp != 0) { //если длина на данный момент становится наименьшей
            minNum = tmp;
            desiredLength = tmpLength;
            while (!maxSquares.empty()) { //очищаем массив, в котором будет минимальное разбиение
                maxSquares.pop();
            }
            stackCopy(&tmpSquares, &maxSquares); //копируем текущий стек в стек с наименьшим разбиением
        }
        else {
            while (!squares->empty()) { //опустошаем стек с текущими квадратами
                squares->pop();
            }
        }
        for (int i = 0; i < tmpLength; i++) //очищаем квадрат и переходим к следующей длине
            for (int j = 0; j < tmpLength; j++)
                arr[x + i][y + j] = 0;
    }

    while (!squares->empty())
        squares->pop();
    stackCopy(&maxSquares, squares); //копируем стек в результирующий
    squares->push({x, y, desiredLength});

    return minNum;
}

int main() {
    int tabletopSize = 0;
    int reqSquaresNumber = 0;
    int reqSquareSide = 0;
    ReqSquare *reqSquares = nullptr;
    bool reqFlag = false;
    int primeFlag = 0;
    int limit = 0;
    int res = 0;
    std::stack <Square> squares; //стек структур
    int **arr = nullptr;
    int **arrReq = nullptr;

    std::cout << "Enter tabletop size" << std::endl;
    if (!(std::cin >> tabletopSize)) {//пока не будет введено число
        std::cout << "Error: size isn't an integer" << std::endl;
        return 0;
    }
    if (tabletopSize < 2 || tabletopSize > 20) {//если размер не удовлетворяет условию задачи
        std::cout << "Error: tabletop have wrong size" << std::endl;
        return 0;
    }
    //обязательные квадраты
    std::cout << "Enter number of required squares" << std::endl;
    if (!(std::cin >> reqSquaresNumber)) {//пока не будет введено число
        std::cout << "Error: number of required squares isn't an integer" << std::endl;
        return 0;
    }
    if (reqSquaresNumber > 0) { //если есть обязательные кадраты
        arrReq = new int* [tabletopSize];
        for (int i = 0; i < tabletopSize; i++)
            arrReq[i] = new int[tabletopSize]();

        reqFlag = true;
        if (reqSquaresNumber < 0 || reqSquaresNumber > pow(tabletopSize,2)) {//если кол-во обязательных квадратов больше N^2 или отрицательное число
            std::cout << "Error: wrong number of required squares" << std::endl;
            return 0;
        }
        reqSquares = new ReqSquare[reqSquaresNumber];
        std::cout << "Enter a list of required squares. Format: side" << std::endl;
        for (int i = 0; i < reqSquaresNumber; i++) {
            if (!(std::cin >> reqSquareSide)) {//пока не будет введено число
                std::cout << "Error: side of required square isn't an integer" << std::endl;
                return 0;
            }
            if (reqSquareSide < 0 || reqSquareSide >= tabletopSize) {//если сторона обязательного квадрата >= tabletopSize или отрицательное число
                std::cout << "Error: wrong side of required square" << std::endl;
                return 0;
            }
            for (int j = 0; j < i; j++) {
                if (reqSquareSide + reqSquares[j].size > tabletopSize) { //нельзя вставить квадрат, который в сумме с любым другим выходит за пределы столешницы
                    std::cout << "Error: you can not use such a square" << std::endl;
                    return 0;
                }
            }
            reqSquares[i].size = reqSquareSide;
        }
    }

    if (!reqFlag){ //если нет обязательных квадратов
        primeFlag = primeOrNot(tabletopSize);
        if (primeFlag){ //если простое число
            limit = 12.15 * sqrt(sqrt(sqrt(sqrt((tabletopSize)))));
        }
        if (primeFlag == 2){ //если непростое число с наименьшим делителем 2
            limit = 4;
        }
        if (primeFlag == 3){ //если непростое число с наименьшим делителем 3
            limit = 6;
        }
        if (primeFlag == 5){ //если непростое число с наименьшим делителем 5
            limit = 8;
        }
    }
    else {
        limit = 1 + (tabletopSize - 1) + tabletopSize; //худший случай для квадратов <=20
    }

    if (reqFlag){
        int x = 0, y = 0;
        for (int i = 0; i < reqSquaresNumber; i++){
            if (!findEmptySpace(arrReq, x, y, tabletopSize)){ //нет пустого места
                std::cout << "Error: space isn't empty\n";
                return 0;
            }
            if ((x + reqSquares[i].size) > tabletopSize){ //квадраты выходят за предел столешницы
                std::cout << "Error: the square goes beyond the tabletop\n";
                return 0;
            }
            if ((y + reqSquares[i].size) > tabletopSize){ //квадраты выходят за предел столешницы
                std::cout << "Error: the square goes beyond the tabletop\n";
                return 0;
            }
            int reqLength = reqSquares[i].size;
            reqSquares[i].x = x;
            reqSquares[i].y = y;
            insertSquare(arrReq, x, y, reqLength);
        }
    }
    //функция вывода массива с квадратами на экран
    if (reqFlag) {
        std::cout << "Array with required squares" << std::endl;
        for (int i = 0; i < tabletopSize; i++) {
            for (int j = 0; j < tabletopSize; j++) {
                std::cout << arrReq[j][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;
    }

    if (primeFlag > 1) {
        arr = new int* [primeFlag];
        for (int i = 0; i < primeFlag; i++)
            arr[i] = new int[primeFlag];
        res = minSquares(limit, &squares, arr, primeFlag, 0);
    }
    if (primeFlag == 1){
        arr = new int* [tabletopSize];
        for (int i = 0; i < tabletopSize; i++)
            arr[i] = new int[tabletopSize];
        res = minSquares(limit, &squares, arr, tabletopSize, 0);
    }
    if (reqFlag){
        res = minSquares(limit, &squares, arrReq, tabletopSize, 0);
    }

    std::cout << "Result is: " << res + reqSquaresNumber << std::endl;

    std::cout << "Required squares is:" << std::endl;
    for (int i = 0; i < reqSquaresNumber; i++){
        std::cout << reqSquares[i].x + 1 << " " << reqSquares[i].y + 1 << " " << reqSquares[i].size << std::endl;
    }
    std::cout << "--------------------" << std::endl;

    Square tmp{};
    int flagCounter = 1;

    if (reqFlag) {
        std::cout << "Split squares is:" << std::endl;
        while (!squares.empty()) {
            if (flagCounter > res) {
                squares.pop();
                continue;
            }
            tmp = squares.top();
            std::cout << tmp.x + 1 << " " << tmp.y + 1 << " " << tmp.size << std::endl;
            insertSquare(arrReq, tmp.x, tmp.y, tmp.size);
            squares.pop();
            flagCounter++;
        }
        std::cout << "-----------------" << std::endl;

        std::cout << "Result tabletop splitting is:" << std::endl;
        for (int i = 0; i < tabletopSize; i++) {
            for (int j = 0; j < tabletopSize; j++) {
                std::cout << arrReq[j][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "----------------------------" << std::endl;
        std::cout << std::endl;
        for (int i = 0; i < tabletopSize; i++)
            delete[] arrReq[i];
        delete[] arrReq;
    }
    else {
        int scale = (primeFlag != 1) ? tabletopSize / primeFlag : 1;
        std::cout << "Split squares is:" << std::endl;
        while (!squares.empty()) {
            if (flagCounter > res) {
                squares.pop();
                continue;
            }
            tmp = squares.top();
            std::cout << tmp.x * scale + 1 << " " << tmp.y * scale + 1 << " " << tmp.size * scale << std::endl;
            insertSquare(arr, tmp.x, tmp.y, tmp.size);
            squares.pop();
            flagCounter++;
        }
        std::cout << "-----------------" << std::endl;
    }

    return 0;
}
