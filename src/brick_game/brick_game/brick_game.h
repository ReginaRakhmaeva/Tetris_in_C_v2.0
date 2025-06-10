#ifndef BRICKGAME_H
#define BRICKGAME_H

/** @brief Количество строк в игровом поле. */
#define ROWS 20

/** @brief Количество столбцов в игровом поле. */
#define COLS 10

/** @brief Код успешного выполнения. */
#define SUCCESS 0

/**
 * @struct GameInfo_t
 * @brief Структура, содержащая информацию о состоянии игры.
 */
typedef struct {
  int **field;    /**< Двумерный массив, представляющий игровое поле */
  int **next;     /**< Следующая фигура */
  int score;      /**< Текущий счет */
  int high_score; /**< Рекордный счет */
  int level;      /**< Текущий уровень */
  int speed;      /**< Скорость падения фигур */
  int pause;      /**< Флаг паузы */
} GameInfo_t;

/**
 * @enum UserAction_t
 * @brief Перечисление возможных действий пользователя.
 */
typedef enum {
  Start,     /**< Начало игры */
  Pause,     /**< Пауза */
  Terminate, /**< Завершение игры */
  Left,      /**< Движение влево */
  Right,     /**< Движение вправо */
  Up,        /**< Не используется */
  Down,      /**< Движение вниз */
  Action     /**< Поворот */
} UserAction_t;

/**
 * @brief Обрабатывает ввод пользователя.
 *
 * @param action Действие пользователя, представленное в виде перечисления
 * UserAction_t.
 * @param hold Флаг удержания клавиши (true - если удерживается, false -
 * одиночное нажатие).
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Обновляет текущее состояние игры.
 *
 * @return Структура GameInfo_t с обновленными параметрами игры.
 */
GameInfo_t updateCurrentState();  // я пуга я люблю пипа
// а я пип я люблю пугу
#endif  // BRICKGAME_H