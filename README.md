# 🎮 Крестики-нолики на C++

![Игровое поле](https://github.com/user-attachments/assets/9e6eb803-5da0-4325-94a0-1af95772e43b)

## 📋 О проекте
**Крестики-нолики** с расширенными возможностями:
- 🔄 **Режимы**: Игрок vs Игрок (PvP) / Игрок vs ИИ (PvE)
- 🧠 **Уровни сложности ИИ**: 
  - Легкий (рандомные ходы)
  - Средний (блокировка победы игрока)
  - Сложный (Minimax + анализ угроз)
- 🏟 **Гибкие настройки**:
  - Размер поля: **3x3 до 10x10**
  - Условие победы: **от 3 до N символов в ряд**
-  **Сохранение результатов** в файл `game_history.txt`

## 🛠 Технические характеристики
- **Язык**: C++17
- **Платформа**: Windows (совместимо с Linux/macOS при замене `system("cls")`)
- **Зависимости**: Только стандартная библиотека
- **Сборка**: 
  - Visual Studio (`CrestikiNoliki.sln`)

## 🎮 Геймплей

![image](https://github.com/user-attachments/assets/db9cbaf6-b2ca-4f73-8f6a-a9e98771adfc)


Управление
Ввод координат в формате y x (например, 2 3).

## Правила
Игроки ходят по очереди (X начинает).

Победа при построении линии из N символов:

Горизонталь / Вертикаль / Диагональ

Ничья при заполнении поля.

## 🔧 Алгоритмы ИИ
Сложный уровень (Minimax)

```
int ComputerPlayerHard::minimax(GameBoard& board, int depth, bool isMaximizing, int alpha, int beta) {
    if (board.checkWin(symbol)) return 1000;
    if (board.checkWin(opponentSymbol)) return -1000;
    if (board.isFull()) return 0;
    
    // Альфа-бета отсечение
    for (int i = 0; i < board.getSize(); ++i) {
        for (int j = 0; j < board.getSize(); ++j) {
            if (board.isCellEmpty(i, j)) {
                board.setCell(i, j, isMaximizing ? symbol : opponentSymbol);
                int score = minimax(board, depth+1, !isMaximizing, alpha, beta);
                board.setCell(i, j, ' ');
                // ... оптимизация ...
            }
        }
    }
    return bestScore;
}
```
## 📈 Статистика
Результаты сохраняются в формате:
```
Дата: 2025-03-25 14:30
Размер поля: 5x5
Условие победы: 4 в ряд
Режим: Против компьютера (сложный)
Результат: Победил игрок O
-------------------------
```

## 📅 Планы развития
Графический интерфейс (SFML/Qt)

Сетевой мультиплеер

ИИ с машинным обучением

## 📜 Лицензия
MIT License.
© 2025 ProkopiiM (Amatsushi)
