#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cctype>
#include <cmath>
#include <windows.h>

using namespace std;
using namespace chrono;

const int PROGRESS_BAR_WIDTH = 50; // Ширина прогресс-бара в символах

// Функция для отображения прогресс-бара
void display_progress(int current, int total, const string& label) {
    float progress = static_cast<float>(current) / total;
    int pos = static_cast<int>(PROGRESS_BAR_WIDTH * progress);
    
    cout << label << " [";
    for(int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        if(i < pos) cout << "=";
        else if(i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << "% \r";
    cout.flush();
}

// Функция для запуска таймера
void start_timer(int minutes, const string& message) {
    const int total_seconds = minutes * 60;
    const auto start_time = steady_clock::now();

    while(true) {
        const auto current_time = steady_clock::now();
        const auto elapsed = duration_cast<seconds>(current_time - start_time).count();
        const int remaining = total_seconds - static_cast<int>(elapsed);

        if(remaining <= 0) break;

        // Рассчет времени
        const int min_rem = remaining / 60;
        const int sec_rem = remaining % 60;
        const int elapsed_seconds = total_seconds - remaining;
        
        // Форматирование времени
        string time_left = "Time left: " 
            + string(min_rem < 10 ? "0" : "") + to_string(min_rem) + ":"
            + string(sec_rem < 10 ? "0" : "") + to_string(sec_rem);
        
        // Объединение с прогресс-баром
        string full_message = message + " | " + time_left;
        display_progress(elapsed_seconds, total_seconds, full_message);

        this_thread::sleep_for(50ms);
    }

    // Очистка строки перед выводом финального сообщения
    cout << string(100, ' ') << "\r"; // Очистка строки
    cout << message << " | COMPLETED [";
    for(int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
        cout << "=";
    }
    cout << "] 100%\n";
    Beep(1000, 500);
}

// Функция для подтверждения действий
bool get_confirmation(const string& prompt) {
    string input;
    while(true) {
        cout << prompt << " (Y/N): ";
        getline(cin, input);

        if(!input.empty()) {
            char choice = toupper(input[0]);
            if(choice == 'Y') return true;
            if(choice == 'N') return false;
        }
        cout << "Invalid input. Please enter Y or N.\n";
    }
}

// Функция для безопасного ввода чисел
int get_valid_input(const string& prompt, int default_value) {
    string input;
    while(true) {
        cout << prompt << " (default " << default_value << "): ";
        getline(cin, input);

        if(input.empty()) return default_value;

        try {
            for(char c : input) {
                if(!isdigit(c)) throw invalid_argument("Invalid characters");
            }

            int value = stoi(input);
            if(value <= 0) throw out_of_range("Value must be positive");
            
            return value;
        }
        catch(const exception& e) {
            cerr << "Error: " << e.what() << ". Please try again.\n";
            cin.clear();
        }
    }
}

// Основная логика Pomodoro
void run_pomodoro(int work_min, int short_break, int long_break, 
                 int sessions, bool confirm_each) {
    for(int i = 0; i < sessions; ++i) {
        const string work_msg = "Work session #" + to_string(i + 1);
        
        // Подтверждение рабочей сессии
        if(confirm_each && !get_confirmation("Start " + work_msg + "?")) {
            cout << "Pomodoro canceled!\n";
            return;
        }
        
        start_timer(work_min, work_msg);

        // Короткий перерыв, кроме последней сессии
        if(i < sessions - 1) {
            const string break_msg = "Short break #" + to_string(i + 1);
            
            if(confirm_each && !get_confirmation("Start " + break_msg + "?")) {
                cout << "Pomodoro canceled!\n";
                return;
            }
            
            start_timer(short_break, break_msg);
        }
    }
    
    // Длинный перерыв после всех сессий
    if(confirm_each && !get_confirmation("Start long break?")) {
        cout << "Long break canceled!\n";
        return;
    }
    start_timer(long_break, "Long break");
}

int main() {
    const int DEFAULT_WORK = 25;
    const int DEFAULT_SHORT_BREAK = 5;
    const int DEFAULT_LONG_BREAK = 15;
    const int DEFAULT_SESSIONS = 4;

    cout << "==== Pomodoro Timer Setup ====\n";
    
    // Получение параметров от пользователя
    int work_min = get_valid_input("Work duration (minutes)", DEFAULT_WORK);
    int short_break = get_valid_input("Short break duration (minutes)", DEFAULT_SHORT_BREAK);
    int long_break = get_valid_input("Long break duration (minutes)", DEFAULT_LONG_BREAK);
    int sessions = get_valid_input("Number of work sessions", DEFAULT_SESSIONS);
    
    // Выбор режима подтверждения
    bool confirm_each = get_confirmation("\nRequire confirmation before each stage?");

    // Вывод сводки параметров
    cout << "\n==== Timer Settings Summary ====\n";
    cout << "Work sessions: " << sessions << "\n"
         << "Work duration: " << work_min << " min\n"
         << "Short break: " << short_break << " min\n"
         << "Long break: " << long_break << " min\n"
         << "Confirmations: " << (confirm_each ? "ENABLED" : "DISABLED") << "\n";

    // Подтверждение запуска
    if(!get_confirmation("\nStart pomodoro with these settings?")) {
        cout << "\nTimer canceled. Goodbye!\n";
        return 0;
    }

    // Запуск Pomodoro
    cout << "\n==== Starting Pomodoro Timer ====\n";
    run_pomodoro(work_min, short_break, long_break, sessions, confirm_each);

    cout << "\n==== Pomodoro Completed! ====\n";
    return 0;
}