#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cctype>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace chrono;

// ============================ КОНФИГУРАЦИЯ ИНТЕРФЕЙСА ============================
/// Ширина прогресс-бара в символах
const int PROGRESS_BAR_WIDTH = 30;
/// Цветовая схема для рабочих интервалов (зеленый)
const string COLOR_WORK = "\033[1;32m";
/// Цветовая схема для перерывов (желтый)
const string COLOR_BREAK = "\033[1;33m";
/// Сброс цветовых настроек
const string COLOR_RESET = "\033[0m";
/// Символы для оформления рамок
const string BORDER = "║";
const string CORNER_TOP = "╔══════════════════════════════════════╗";
const string CORNER_BOT = "╚══════════════════════════════════════╝";

// ============================ WINDOWS-СПЕЦИФИЧНЫЕ НАСТРОЙКИ ============================
#ifdef _WIN32
/**
 * @brief Проверяет версию Windows (7 или новее)
 * @return true если система Windows 7 или новее, иначе false
 */
bool is_windows7_or_newer() {
    OSVERSIONINFOEX osvi = { sizeof(OSVERSIONINFOEX) };
    DWORDLONG mask = VerSetConditionMask(
        VerSetConditionMask(
            VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
            VER_MINORVERSION, VER_GREATER_EQUAL),
        VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

    osvi.dwMajorVersion = 6;
    osvi.dwMinorVersion = 1;
    osvi.wServicePackMajor = 0;

    return VerifyVersionInfo(&osvi, 
        VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, 
        mask) != FALSE;
}

/**
 * @brief Активирует поддержку VT-режима и UTF-8 для Windows
 *        Настраивает консоль для работы с цветами и Unicode
 */
void enable_vt_mode() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

// ============================ ФУНКЦИИ ОТОБРАЖЕНИЯ ============================
/**
 * @brief Выводит стилизованный заголовок с рамкой
 * @param text Текст для отображения в заголовке
 */
void print_header(const string& text) {
    cout << CORNER_TOP << "\n"
         << BORDER << "  " << text << string(36 - text.length(), ' ') << BORDER << "\n"
         << CORNER_BOT << endl;
}

/**
 * @brief Отображает прогресс выполнения таймера
 * @param elapsed Прошедшее время в секундах
 * @param total Общее время этапа в секундах
 * @param label Название текущего этапа
 * @param is_work Флаг типа этапа (true - работа, false - перерыв)
 */
void display_progress(int elapsed, int total, const string& label, bool is_work) {
    float progress = static_cast<float>(elapsed) / total;
    int pos = static_cast<int>(PROGRESS_BAR_WIDTH * progress);
    
    string color = is_work ? COLOR_WORK : COLOR_BREAK;
    int remaining = total - elapsed;

    // Формирование графического представления
    cout << color << BORDER << " " << label << "\n"
         << BORDER << " [";
    for(int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        cout << (i < pos ? "■" : " ");
    }
    cout << "] " << int(progress * 100) << "%\n"
         << BORDER << " Remaining: " 
         << string(remaining / 60 < 10 ? "0" : "") << remaining / 60 << ":"
         << string(remaining % 60 < 10 ? "0" : "") << remaining % 60
         << COLOR_RESET << "  \r\033[2A";
    
    cout.flush();
}

// ============================ УПРАВЛЕНИЕ ТАЙМЕРОМ ============================
/**
 * @brief Запускает и управляет обратным отсчетом времени
 * @param minutes Длительность этапа в минутах
 * @param phase_name Название этапа для отображения
 * @param is_work Тип этапа (true - работа, false - перерыв)
 */
void start_timer(int minutes, const string& phase_name, bool is_work) {
    const int total_seconds = minutes * 60;
    auto start_time = steady_clock::now();
    
    print_header(phase_name);
    
    // Основной цикл отсчета времени
    for(int elapsed = 0; elapsed <= total_seconds; ++elapsed) {
        display_progress(elapsed, total_seconds, phase_name, is_work);
        this_thread::sleep_for(1000ms);
    }
    
    cout << "\033[2B" << COLOR_RESET;
#ifdef _WIN32
    Beep(800, 200); Beep(1200, 400);
#endif
}

// ============================ ВВОД ДАННЫХ ============================
/**
 * @brief Запрашивает числовой ввод с валидацией
 * @param prompt Сообщение для пользователя
 * @param default_val Значение по умолчанию
 * @return Корректное введенное значение
 */
int get_valid_input(const string& prompt, int default_val) {
    string input;
    while(true) {
        cout << BORDER << " " << prompt << " (default " << default_val << "): ";
        getline(cin, input);
        
        if(input.empty()) return default_val;
        
        try {
            if(input.find_first_not_of("0123456789") != string::npos)
                throw invalid_argument("Contains non-digit characters");
                
            int val = stoi(input);
            if(val <= 0) throw out_of_range("Value must be positive");
            return val;
        }
        catch(const exception& e) {
            cout << BORDER << " \033[31mError: " << e.what() << "\033[0m\n";
        }
    }
}

/**
 * @brief Запрашивает подтверждение (Y/N)
 * @param prompt Текст вопроса
 * @return true если получено подтверждение
 */
bool get_confirmation(const string& prompt) {
    string input;
    cout << BORDER << " " << prompt << " (Y/N): ";
    getline(cin, input);
    return !input.empty() && toupper(input[0]) == 'Y';
}


// ============================ ОСНОВНАЯ ЛОГИКА ============================
/**
 * @brief Выполняет полный цикл Pomodoro
 * @param work_min Длительность работы в минутах
 * @param short_break Длительность короткого перерыва
 * @param long_break Длительность длинного перерыва
 * @param sessions Количество рабочих сессий
 * @param confirm_mode Флаг необходимости подтверждения этапов
 */
void run_pomodoro_cycle(int work_min, int short_break, int long_break, int sessions, bool confirm_mode) {
    for(int i = 0; i < sessions; ++i) {
        bool run_work = true;
        if(confirm_mode) {
            run_work = get_confirmation("Start Work Session #" + to_string(i+1) + "?");
        }
        if(run_work) {
            start_timer(work_min, " Work Session #" + to_string(i+1), true);
        }

        if(i < sessions - 1) {
            bool run_short_break = true;
            if(confirm_mode) {
                run_short_break = get_confirmation("Start Short Break #" + to_string(i+1) + "?");
            }
            if(run_short_break) {
                start_timer(short_break, " Short Break #" + to_string(i+1), false);
            }
        }
    }

    bool run_long_break = true;
    if(confirm_mode) {
        run_long_break = get_confirmation("Start Long Break?");
    }
    if(run_long_break) {
        start_timer(long_break, " Long Break", false);
    }
}

/**
 * @brief Главная функция программы
 * @return Код завершения приложения
 */
int main() {
#ifdef _WIN32
    if (is_windows7_or_newer()) {
        enable_vt_mode();
    }
#endif

    // Настройка параметров
    print_header(" Pomodoro Timer Setup ");
    
    int work_min = get_valid_input("Work duration (minutes)", 25);
    int short_break = get_valid_input("Short break (minutes)", 5);
    int long_break = get_valid_input("Long break (minutes)", 15);
    int sessions = get_valid_input("Number of sessions", 4);
    
    // Подтверждение запуска
    bool confirm_mode = get_confirmation("Enable confirmation before each timer?");
    
    if(!get_confirmation("Start timer with these settings?")) {
        print_header(" Timer Canceled ");
        return 0;
    }

    // Запуск основного цикла
    print_header(" Pomodoro Started ");
    run_pomodoro_cycle(work_min, short_break, long_break, sessions, confirm_mode);
    print_header(" Pomodoro Completed ");

    return 0;
}