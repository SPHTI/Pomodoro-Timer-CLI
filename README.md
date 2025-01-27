# Pomodoro Timer CLI 🍅
![Release](https://img.shields.io/github/v/release/SPHTI/Pomodoro-Timer-CLI?style=flat-square&logo=github&color=3178C6)
![Last Commit](https://img.shields.io/github/last-commit/SPHTI/Pomodoro-Timer-CLI?style=flat-square&logo=git&color=7E4DD2)
![Repo Size](https://img.shields.io/github/repo-size/SPHTI/Pomodoro-Timer-CLI?style=flat-square&logo=github&color=4B89DC)
![C++](https://img.shields.io/badge/C++-17+-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.12+-064F8C?style=flat-square&logo=cmake&logoColor=white)
![Platforms](https://img.shields.io/badge/Platform-Windows_Linux-4B6E9F?style=flat-square&logo=windows11&logoColor=white)

Реализация техники Помодоро для командной строки с расширенными возможностями кастомизации и кросс-платформенной поддержкой.

## ✨ Особенности

- 🎨 **Интерактивный интерфейс** с цветовыми схемами и псевдографикой
- ⚙️ **Гибкая настройка** параметров через диалоговое меню:
  - Длительность рабочих сессий (25 мин по умолчанию)
  - Короткие (5 мин) и длинные перерывы (15 мин)
  - Количество сессий перед длинным перерывом
  - Подтверждение начала каждого этапа
- 📊 **Визуализация прогресса**:
  - Анимированный прогресс-бар
  - Точный отсчет оставшегося времени
  - Процент выполнения задачи
- 🔊 **Кросс-платформенные уведомления**:
  - Звуковые сигналы (поддержка Windows)
  - Визуальные алерт-сообщения
- 🌍 **Поддержка Unicode** и VT-экранов
- 🛡️ **Валидация ввода** с обработкой ошибок

## 📦 Зависимости

- Компилятор C++17 (GCC 9+/Clang 10+/MSVC 19.28+)
- CMake 3.12+
- Windows: Windows 7+ (для VT-поддержки)
- Linux: Терминал с поддержкой UTF-8 и ANSI-цветов

## 🚀 Установка

### Сборка из исходников

```bash
# Клонирование репозитория
git clone https://github.com/yourusername/Pomodoro-Timer-CLI.git
cd Pomodoro-Timer-CLI

# Создание build-директории
mkdir build && cd build

# Генерация проекта
cmake ..

# Компиляция (для Windows добавьте --config Release)
cmake --build .
```

### Готовые бинарники
Скачайте последний релиз для вашей платформы из [раздела Releases](https://github.com/yourusername/Pomodoro-Timer-CLI/releases).

## 🖥️ Использование

1. Запустите программу:
   ```bash
   ./PomodoroTimer # Linux
   .\PomodoroTimer.exe # Windows
   ```

2. Следуйте интерактивным подсказкам для настройки:
   ```
   ╔══════════════════════════════════════╗
   ║  Pomodoro Timer Setup                ║
   ╚══════════════════════════════════════╝
   ║ Work duration (minutes) (default 25): 
   ║ Short break (minutes) (default 5): 
   ║ Long break (minutes) (default 15): 
   ║ Number of sessions (default 4): 
   ```

3. Начните работу с таймером:
   ```
   ╔══════════════════════════════════════╗
   ║  Work Session #1                     ║
   ╚══════════════════════════════════════╝
   ║ [■■■■■■■■■                 ] 45%
   ║ Remaining: 12:34
   ```

### Поддерживаемые платформы
| ОС         | Поддержка VT | Звуковые сигналы |
|------------|--------------|-------------------|
| Windows 10+| ✅           | ✅                |
| Linux      | ✅           | ❌                |
| macOS*     | ✅           | ❌                |

*Тестирование на macOS не проводилось
