#include <algorithm> 
#include <cstdlib>
#include <ctime>
#include <filesystem>  
#include <fstream>
#include <iomanip> 
#include <iostream>
#include <locale.h>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include <tuple>
#include <thread>
#include <future>

using namespace std;

// Очистка экрана
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// класс игрового поля
class GameBoard {
private:
    vector<vector<char>> board;  // игровое поле
    int size;                    // зазмер поля
    int winCondition;            // условие победы (количество сколько в ряд)

public:
    GameBoard(int n, int winCond) : size(n), winCondition(winCond), board(n, vector<char>(n, ' ')) {}

    // отрисовка поля
    void print() const {
        cout << "  ";
        for (int j = 0; j < size; ++j) {
            cout << j % 10 << " ";
        }
        cout << endl;

        for (int i = 0; i < size; ++i) {
            cout << i % 10 << " ";
            for (int j = 0; j < size; ++j) {
                cout << board[i][j];
                if (j < size - 1) cout << "|";
            }
            cout << endl;
            if (i < size - 1) {
                cout << "  ";
                for (int j = 0; j < size * 2 - 1; ++j) {
                    cout << "-";
                }
                cout << endl;
            }
        }
    }

    // проверка свободна ли клетка
    bool isCellEmpty(int x, int y) const {
        return board[x][y] == ' ';
    }

    // установка символа в клетку
    void setCell(int x, int y, char symbol) {
        board[x][y] = symbol;
    }

    // получение символа из клетки
    char getCell(int x, int y) const {
        return board[x][y];
    }

    // проверка заполнено ли поле
    bool isFull() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (board[i][j] == ' ') return false;
            }
        }
        return true;
    }

    // получение размера поля
    int getSize() const {
        return size;
    }

    // проверка победы
    bool checkWin(char player) const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (checkDirection(i, j, 1, 0, player) ||  // горизонталь
                    checkDirection(i, j, 0, 1, player) ||  // вертикаль
                    checkDirection(i, j, 1, 1, player) ||  // диагональ вниз
                    checkDirection(i, j, 1, -1, player)) { // диагональ вверх
                    return true;
                }
            }
        }
        return false;
    }

    // подсветка победной комбинации
    void highlightWin(char player) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (checkDirection(i, j, 1, 0, player, true) ||
                    checkDirection(i, j, 0, 1, player, true) ||
                    checkDirection(i, j, 1, 1, player, true) ||
                    checkDirection(i, j, 1, -1, player, true)) {
                    return;
                }
            }
        }
    }

    int getWinCondition() const {
        return winCondition;
    }

private:
    bool checkDirection(int x, int y, int dx, int dy, char player, bool highlight = false) const {
        int count = 0;
        while (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == player) {
            if (highlight) {
                const_cast<vector<vector<char>>&>(board)[x][y] = toupper(board[x][y]);
            }
            count++;
            x += dx;
            y += dy;
        }
        return count >= winCondition;
    }
};

//запись результатов игры в файл
void saveGameResult(const string& result, int size, int winCondition, int mode, int difficulty) {
    ofstream outFile("C:/Users/ghjbr/source/repos/CrestikiNoliki/x64/Release/game_history.txt", ios::app);  // Открываем файл для добавления
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    // Получаем текущее время
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);

    // Записываем данные в файл
    outFile << "Дата и время: " << put_time(&localTime, "%Y-%m-%d %H:%M:%S") << endl;
    outFile << "Размер поля: " << size << "x" << size << endl;
    outFile << "Условие победы: " << winCondition << " в ряд" << endl;
    outFile << "Режим игры: " << (mode == 1 ? "Два игрока" : "Против компьютера") << endl;
    if (mode == 2) {
        outFile << "Уровень сложности: " << difficulty << endl;
    }
    outFile << "Результат: " << result << endl;
    outFile << "-------------------------" << endl;

    outFile.close();
}

//базовый класс для игрока
class Player {
protected:
    char symbol;

public:
    Player(char sym) : symbol(sym) {}
    virtual void makeMove(GameBoard& board) = 0;
    char getSymbol() const { return symbol; }
};

//класс для человека
class HumanPlayer : public Player {
public:
    HumanPlayer(char sym) : Player(sym) {}

    void makeMove(GameBoard& board) override {
        int x, y;
        while (true) {
            cout << "Игрок " << symbol << ", введите координаты (y x): ";
            cin >> x >> y;
            if (x >= 0 && x < board.getSize() && y >= 0 && y < board.getSize() && board.isCellEmpty(x, y)) {
                board.setCell(x, y, symbol);
                break;
            }
            else {
                cout << "Некорректный ход. Попробуйте снова." << endl;
            }
        }
    }
};

//класс для компьютера (легкий уровень)
class ComputerPlayerEasy : public Player {
public:
    ComputerPlayerEasy(char sym) : Player(sym) {}

    void makeMove(GameBoard& board) override {
        int x, y;
        while (true) {
            x = rand() % board.getSize();
            y = rand() % board.getSize();
            if (board.isCellEmpty(x, y)) {
                board.setCell(x, y, symbol);
                break;
            }
        }
        cout << "Компьютер сделал ход." << endl;
    }
};

// версия 1.0
//class ComputerPlayerMedium : public Player {
//public:
//    ComputerPlayerMedium(char sym) : Player(sym) {}
//
//    void makeMove(GameBoard& board) override {
//        // Проверка, может ли компьютер выиграть
//        if (tryWin(board, symbol)) return;
//
//        // Проверка, может ли игрок выиграть, и блокировка
//        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
//        if (tryWin(board, opponentSymbol)) return;
//
//        // Случайный ход
//        makeRandomMove(board);
//        cout << "Компьютер сделал ход." << endl;
//    }
//
//private:
//    // Попытка выиграть или заблокировать
//    bool tryWin(GameBoard& board, char playerSymbol) {
//        for (int i = 0; i < board.getSize(); ++i) {
//            for (int j = 0; j < board.getSize(); ++j) {
//                if (board.isCellEmpty(i, j)) {
//                    board.setCell(i, j, playerSymbol);
//                    if (board.checkWin(playerSymbol)) {
//                        board.setCell(i, j, symbol);  // Ход компьютера
//                        return true;
//                    }
//                    board.setCell(i, j, ' ');  // Отмена хода
//                }
//            }
//        }
//        return false;
//    }
//    // Метод для выполнения случайного хода
//    void makeRandomMove(GameBoard& board) {
//        int x, y;
//        while (true) {
//            x = rand() % board.getSize();
//            y = rand() % board.getSize();
//            if (board.isCellEmpty(x, y)) {
//                board.setCell(x, y, symbol);
//                break;
//            }
//        }
//    }
//};

//версия 1.1 класс для компьютера (среднйи уровень)
class ComputerPlayerMedium : public Player {
public:
    ComputerPlayerMedium(char sym) : Player(sym) {}

    void makeMove(GameBoard& board) override {
        //// Проверка, может ли компьютер выиграть
        //if (tryWin(board, symbol)) return;

        // Проверка, может ли игрок выиграть через один ход, и блокировка
        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
        if (tryWin(board, opponentSymbol)) return;

        // Случайный ход
        makeRandomMove(board);
        cout << "Компьютер сделал ход." << endl;
    }

private:
    // Попытка выиграть или заблокировать (один ход до победы)
    bool tryWin(GameBoard& board, char playerSymbol) {
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, playerSymbol);
                    if (board.checkWin(playerSymbol)) {
                        board.setCell(i, j, symbol);  // Ход компьютера
                        return true;
                    }
                    board.setCell(i, j, ' ');  // Отмена хода
                }
            }
        }
        return false;
    }

    // Оценка угрозы
    int evaluateThreat(const GameBoard& board, char playerSymbol) const {
        int threatScore = 0;

        // Проверка всех возможных линий
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                // Горизонтальные линии
                if (j + board.getWinCondition() <= board.getSize()) {
                    threatScore += evaluateLineThreat(board, i, j, 0, 1, playerSymbol);
                }
                // Вертикальные линии
                if (i + board.getWinCondition() <= board.getSize()) {
                    threatScore += evaluateLineThreat(board, i, j, 1, 0, playerSymbol);
                }
                // Диагонали вниз
                if (i + board.getWinCondition() <= board.getSize() && j + board.getWinCondition() <= board.getSize()) {
                    threatScore += evaluateLineThreat(board, i, j, 1, 1, playerSymbol);
                }
                // Диагонали вверх
                if (i - board.getWinCondition() >= -1 && j + board.getWinCondition() <= board.getSize()) {
                    threatScore += evaluateLineThreat(board, i, j, -1, 1, playerSymbol);
                }
            }
        }

        return threatScore;
    }

    // Оценка угрозы в одной линии
    int evaluateLineThreat(const GameBoard& board, int x, int y, int dx, int dy, char playerSymbol) const {
        int playerCount = 0;
        int emptyCount = 0;

        for (int k = 0; k < board.getWinCondition(); ++k) {
            char cell = board.getCell(x + k * dx, y + k * dy);
            if (cell == playerSymbol) {
                playerCount++;
            }
            else if (cell == ' ') {
                emptyCount++;
            }
            else {
                // Если встретился символ противника, линия не подходит
                return 0;
            }
        }

        // Оценка угрозы
        if (playerCount == board.getWinCondition() - 2 && emptyCount == 2) {
            // Два шага до победы
            return 10;
        }
        else if (playerCount == board.getWinCondition() - 3 && emptyCount == 3) {
            // Три шага до победы
            return 1;
        }

        return 0;
    }

    // Метод для выполнения случайного хода
    void makeRandomMove(GameBoard& board) {
        int x, y;
        while (true) {
            x = rand() % board.getSize();
            y = rand() % board.getSize();
            if (board.isCellEmpty(x, y)) {
                board.setCell(x, y, symbol);
                break;
            }
        }
    }
};

//версия 1.16 класс для компьютера (сложный уровень)
class ComputerPlayerHard : public Player {
public:
    ComputerPlayerHard(char sym) : Player(sym) {}

    void makeMove(GameBoard& board) override {
        // проверка на победу
        if (tryWin(board, symbol,false)) return;

        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';

        //проверка на блокировку победы противника
        if (tryBlockWin(board, opponentSymbol)) return;

        //проверка на победу в 2 хода
        if (tryWinInTwoMoves(board, symbol)) return;

        // проверка на блокировку победы противника в 2 хода
        if (tryBlockWinInTwoMoves(board, opponentSymbol)) return;

        // используем Minimax если проверки не получились
        int bestScore = -1000;
        int bestX = -1, bestY = -1;

        //глубины поиска
        int maxDepth = (board.getSize() <= 5) ? 5 : 3;

        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, symbol);
                    int score = minimax(board, 0, false, -1000, 1000, maxDepth);
                    board.setCell(i, j, ' ');  

                    if (score > bestScore) {
                        bestScore = score;
                        bestX = i;
                        bestY = j;
                    }
                }
            }
        }

        if (bestX != -1 && bestY != -1) {
            board.setCell(bestX, bestY, symbol);
        }
        else {
            std::cout << "No valid move found!" << std::endl;
        }
    }

private:
    // попытка выиграть
    bool tryWin(GameBoard& board, char playerSymbol, bool check) {
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, playerSymbol);
                    if (board.checkWin(playerSymbol)) {
                        if (!check)
                        {
                            board.setCell(i, j, symbol);
                        }
                        else
                        {
                            board.setCell(i, j, ' ');
                        }
                        return true;
                    }
                    board.setCell(i, j, ' ');
                }
            }
        }
        return false;
    }

    //попытка заблокировать победу противника
    bool tryBlockWin(GameBoard& board, char opponentSymbol) {
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, opponentSymbol);
                    if (board.checkWin(opponentSymbol)) {
                        board.setCell(i, j, symbol); 
                        return true;
                    }
                    board.setCell(i, j, ' '); 
                }
            }
        }
        return false;
    }

    bool tryWinInTwoMoves(GameBoard& board, char playerSymbol) {
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, playerSymbol);
                    if (tryWin(board, playerSymbol, true)) {
                        board.setCell(i, j, symbol);  
                        return true;
                    }
                    board.setCell(i, j, ' '); 
                }
            }
        }
        return false;
    }

    bool tryBlockWinInTwoMoves(GameBoard& board, char opponentSymbol) {
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                if (board.isCellEmpty(i, j)) {
                    board.setCell(i, j, opponentSymbol);
                    if (tryWin(board, opponentSymbol,true)) {
                        board.setCell(i, j, symbol);  
                        return true;
                    }
                    board.setCell(i, j, ' '); 
                }
            }
        }
        return false;
    }

    //оценка хода
    int evaluate(const GameBoard& board) const {
        if (board.checkWin(symbol)) return 1000;  
        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
        if (board.checkWin(opponentSymbol)) return -1000;  
        return heuristicEvaluation(board);  
    }

    // эвристическая оценка
    int heuristicEvaluation(const GameBoard& board) const {
        int score = 0;

        //оценка для компьютера
        score += evaluateLines(board, symbol);

        //оценка для противника
        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
        score -= evaluateLines(board, opponentSymbol);

        return score;
    }

    //оценка линий
    int evaluateLines(const GameBoard& board, char playerSymbol) const {
        int score = 0;

        //проверка всех линий
        for (int i = 0; i < board.getSize(); ++i) {
            for (int j = 0; j < board.getSize(); ++j) {
                //горизонталь
                if (j + board.getWinCondition() <= board.getSize()) {
                    score += evaluateLine(board, i, j, 0, 1, playerSymbol);
                }
                //вертикаль
                if (i + board.getWinCondition() <= board.getSize()) {
                    score += evaluateLine(board, i, j, 1, 0, playerSymbol);
                }
                // диагональ вниз
                if (i + board.getWinCondition() <= board.getSize() && j + board.getWinCondition() <= board.getSize()) {
                    score += evaluateLine(board, i, j, 1, 1, playerSymbol);
                }
                // даигональ вверх
                if (i - board.getWinCondition() >= -1 && j + board.getWinCondition() <= board.getSize()) {
                    score += evaluateLine(board, i, j, -1, 1, playerSymbol);
                }
            }
        }

        return score;
    }

    //оценка одной линии
    int evaluateLine(const GameBoard& board, int x, int y, int dx, int dy, char playerSymbol) const {
        int playerCount = 0;
        int emptyCount = 0;

        for (int k = 0; k < board.getWinCondition(); ++k) {
            char cell = board.getCell(x + k * dx, y + k * dy);
            if (cell == playerSymbol) {
                playerCount++;
            }
            else if (cell == ' ') {
                emptyCount++;
            }
            else {
                //если есть символ то линию пропускаем
                return 0;
            }
        }

        //оценка линии
        if (playerCount == board.getWinCondition() - 1 && emptyCount == 1) {
            return 100;
        }
        else if (playerCount == board.getWinCondition() - 2 && emptyCount == 2) {
            return 10;
        }
        else if (playerCount == board.getWinCondition() - 3 && emptyCount == 3) {
            return 1;
        }

        return 0;
    }

    //алгоритм минимакс с альфа-бета отсечением (отсечение ненужных ходов)
    int minimax(GameBoard& board, int depth, bool isMaximizing, int alpha, int beta, int maxDepth) {
        int score = evaluate(board);

        if (score == 1000 || score == -1000 || board.isFull() || depth == maxDepth) {
            return score;
        }

        if (isMaximizing) {
            int bestScore = -1000;
            for (int i = 0; i < board.getSize(); ++i) {
                for (int j = 0; j < board.getSize(); ++j) {
                    if (board.isCellEmpty(i, j)) {
                        board.setCell(i, j, symbol);
                        int currentScore = minimax(board, depth + 1, false, alpha, beta, maxDepth);
                        board.setCell(i, j, ' ');

                        if (currentScore > bestScore) {
                            bestScore = currentScore;
                        }
                        alpha = max(alpha, bestScore);
                        if (beta <= alpha) break;
                    }
                }
            }
            return bestScore;
        }
        else {
            int bestScore = 1000;
            for (int i = 0; i < board.getSize(); ++i) {
                for (int j = 0; j < board.getSize(); ++j) {
                    if (board.isCellEmpty(i, j)) {
                        char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
                        board.setCell(i, j, opponentSymbol);
                        int currentScore = minimax(board, depth + 1, true, alpha, beta, maxDepth);
                        board.setCell(i, j, ' ');

                        if (currentScore < bestScore) {
                            bestScore = currentScore;
                        }
                        beta = min(beta, bestScore);
                        if (beta <= alpha) break;
                    }
                }
            }
            return bestScore;
        }
    }
};


//основнйо класс для игры
class Game {
private:
    GameBoard board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;
    int size;
    int winCondition;
    int mode;
    int difficulty;
    string difficultyToString;

public:
    Game(int size, int winCond, bool vsComputer, int difficulty) : board(size, winCond) {
        player1 = new HumanPlayer('X');
        if (vsComputer) {
            this->mode = 2;
            if (difficulty == 1) {
                this->difficulty = 1;
                this->difficultyToString = "простая";
                player2 = new ComputerPlayerEasy('O');
            }
            else if (difficulty == 2) {
                this->difficulty = 2;
                this->difficultyToString = "средний";
                player2 = new ComputerPlayerMedium('O');
            }
            else if (difficulty == 3) {
                this->difficulty = 3;
                this->difficultyToString = "сложный";
                player2 = new ComputerPlayerHard('O');
            }
        }
        else {
            this->mode = 1;
            player2 = new HumanPlayer('O');
        }
        currentPlayer = player1;
        this->size = size;
        this->winCondition = winCond;
    }

    ~Game() {
        delete player1;
        delete player2;
    }

    void play() {
        while (true) {
            clearScreen();
            board.print();
            currentPlayer->makeMove(board);

            if (board.checkWin(currentPlayer->getSymbol())) {
                clearScreen();
                board.print();
                board.highlightWin(currentPlayer->getSymbol());
                cout << "Игрок " << currentPlayer->getSymbol() << " выиграл!" << endl;

               //сохранение результатов
                string result = "Победил игрок " + string(1, currentPlayer->getSymbol());
                saveGameResult(result, size, winCondition, mode, difficulty);
                break;
            }

            if (board.isFull()) {
                clearScreen();
                board.print();
                cout << "Ничья!" << endl;

                //сохранение результатоы
                saveGameResult("Ничья", size, winCondition, mode, difficulty);
                break;
            }

            switchPlayer();
        }
    }

private:
    void switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }
};

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
#endif
    setlocale(LC_ALL, "Russian");

    srand(time(0));


    while (true) {
        int mode;
        cout << "Выберите режим (1 - два игрока, 2 - против компьютера): ";
        cin >> mode;

        int difficulty = 1;
        if (mode == 2) {
            cout << "Выберите уровень сложности (1 - легкий, 2 - средний, 3 - сложный): ";
            cin >> difficulty;
        }

        int size, winCond;
        bool check = true;

        while (check)
        {
            cout << "Введите размер поля: ";
            cin >> size;
           
           if (mode != 2 && difficulty != 3)
            {
                if (size < 3 || size > 10)
                {
                    cout << "Размер поля не может быть меньше 3 или больше 10, введите размер в заданном диапозоне." << endl;
                }
                else
                {
                    check = false;
                }
            }
            else
            {
                if (size < 3 || size > 7)
                {
                    cout << "Размер поля на сложном уровне не может быть меньше 3 или больше 7, введите размер в заданном диапозоне." << endl;
                }
                else
                {
                    check = false;
                }
            }

           /*if (size < 3 || size > 10)
           {
               cout << "Размер поля не может быть меньше 3 или больше 10, введите размер в заданном диапозоне." << endl;
           }
           else
           {
               check = false;
           }*/
        }
    
        check = true;
        while (check)
        {
            cout << "Введите количество символов для победы: ";
            cin >> winCond;
            if (winCond < 3 || winCond > size)
            {
                cout << "Число для победы меньше 3, введите число больше 3, но меньше или равно размеру поля" << endl;
            }
            else
            {
                check = false;
            }
        }
        
        Game game(size, winCond, mode == 2, difficulty);
        game.play();

        char choice;
        cout << "Хотите сыграть ещё раз? (y/n): ";
        cin >> choice;
        if (choice != 'y' && choice != 'Y') break;
    }
    return 0;
}