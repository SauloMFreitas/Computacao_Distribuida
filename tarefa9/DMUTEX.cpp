#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstdlib>

using namespace std;

// Número de processos na simulação
const int N = 3;

// Relógio lógico global
int clock_global = 0;

// Fila de requisições (min heap)
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> request_queue;

// Locks para garantir a consistência do relógio e da fila
mutex clock_mutex;
mutex queue_mutex;
condition_variable cv;

// Função para incrementar o relógio lógico
int increment_clock() {
    lock_guard<mutex> lock(clock_mutex);
    return ++clock_global;
}

// Classe para representar um Processo
class Process {
public:
    Process(int id) : id(id), local_clock(0) {}

    void operator()() {
        for (int i = 0; i < 2; ++i) {  // Cada processo tenta acessar duas vezes a seção crítica
            request_entry();
            this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));
            release_entry();
            this_thread::sleep_for(chrono::seconds(rand() % 3 + 1));
        }
    }

private:
    int id;
    int local_clock;

    void request_entry() {
        // Incrementar o relógio ao fazer a requisição
        local_clock = increment_clock();
        {
            lock_guard<mutex> lock(queue_mutex);
            request_queue.push({local_clock, id});
            cout << "Processo " << id << " fez uma requisição com o timestamp " << local_clock << endl;
        }
        enter_critical_section();
    }

    void enter_critical_section() {
        unique_lock<mutex> lock(queue_mutex);
        cv.wait(lock, [this]() { return request_queue.top().second == id; });
        cout << "Processo " << id << " entrou na seção crítica (Clock: " << local_clock << ")" << endl;
    }

    void release_entry() {
        {
            lock_guard<mutex> lock(queue_mutex);
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> temp_queue;
            while (!request_queue.empty()) {
                if (request_queue.top().second != id) {
                    temp_queue.push(request_queue.top());
                }
                request_queue.pop();
            }
            swap(request_queue, temp_queue);
            cout << "Processo " << id << " liberou a seção crítica (Clock: " << local_clock << ")" << endl;
        }
        cv.notify_all();
    }
};

int main() {
    srand(time(0));
    vector<thread> processes;

    // Inicializando os processos
    for (int i = 0; i < N; ++i) {
        processes.emplace_back(Process(i));
    }

    // Iniciando todos os processos
    for (auto& process : processes) {
        process.join();
    }

    return 0;
}
