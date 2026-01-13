#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

struct MadeReader {
    std::ifstream reader;
    int _len;
};

struct FlightNumber {
    std::string airport;
    int id;
};

struct DepartureTime {
    int hh;
    int mm;
};

struct Record {
    FlightNumber flight_number;
    DepartureTime time;
    int cost;
    std::vector<std::string> departure_days;

    bool operator== (Record& to_comp){
        if (flight_number.airport == to_comp.flight_number.airport &&
        flight_number.id == to_comp.flight_number.id &&
        time.hh == to_comp.time.hh &&
        time.mm == to_comp.time.mm &&
        cost == to_comp.cost){
            if (departure_days.size() == to_comp.departure_days.size()) {
                if (departure_days.empty()){
                    return true;
                }
                for (auto elem: departure_days) {
                    if (departure_days != to_comp.departure_days) {
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    }
    bool operator!= (Record& to_comp){
        if (flight_number.airport != to_comp.flight_number.airport &&
            flight_number.id != to_comp.flight_number.id &&
            time.hh != to_comp.time.hh &&
            time.mm != to_comp.time.mm &&
            cost != to_comp.cost){
            for (auto elem:departure_days){
                if (departure_days == to_comp.departure_days){
                    return false;
                }
                return true;
            }
        }
        return false;
    }
};

std::string slice(std::string data, int first_ind, int second_ind) {
    std::string result = "";
    if (second_ind > data.size()) {
        std::cout << "Second ind error";
        return data;
    }
    if (first_ind < 0) {
        std::cout << "First ind error";
    }
    for (int ind = first_ind; ind < second_ind; ind++) {
        result += data[ind];
    }
    return result;
}

std::vector<std::string> split(std::string line, char delim) {
    std::vector<std::string> to_return;
    std::stringstream ss(line);
    std::string part_line;
    while (std::getline(ss, part_line, delim)) {
        to_return.push_back(part_line);
    }
    return to_return;
}

std::string make_string_from_Record(Record data) {
    std::string result;
    result += data.flight_number.airport;
    result += std::to_string(data.flight_number.id) + " ";
    result += std::to_string(data.time.hh) + ":"\
 + std::to_string(data.time.mm) + " ";
    result += std::to_string(data.cost) + " ";
    for (std::string elem: data.departure_days) {
        result += elem + " ";
    }
    return result;
}

MadeReader make_reader(std::string root_to_input_file) {
    MadeReader to_return;
    std::ifstream reader(root_to_input_file);
    std::string line_n;
    int n = 0;
    if (std::getline(reader, line_n)) {
        n = std::stoi(line_n);
    } else {
        throw std::invalid_argument("Wrong file!");
    }
    to_return.reader = std::move(reader);
    to_return._len = n;
    return to_return;
}

Record make_record(std::string line) {
    Record to_return;
    auto to_rec = split(line, ',');
    FlightNumber now_FN;
    DepartureTime now_DP;
    now_FN.airport = slice(to_rec[0], 0, 2);
    now_FN.id = std::stoi(slice(to_rec[0], 2, 5));
    auto to_DP = split(to_rec[1], ':');
    now_DP.hh = std::stoi(to_DP[0]);
    now_DP.mm = std::stoi(to_DP[1]);
    to_return.flight_number = now_FN;
    to_return.time = now_DP;
    to_return.cost = std::stoi(to_rec[2]);
    if (to_rec.size() == 4) {
        to_return.departure_days = split(to_rec[3], ' ');
    } else {
        to_return.departure_days = {};
    }
    return to_return;
}

class QueueWithPriority{
private:
    int* data;
    unsigned int tail;
    unsigned int head;
    unsigned int _len;

    unsigned int search_place(int to_add) {
        for (unsigned int i = head; i != tail; i = (i+1) / _len) {
            if (to_add<= data[i]) {
                return i;
            }
        }
        return tail;
    }

    void shift_left(unsigned int ind) {
        for (unsigned int i = head; i != ind; i = (i+1) / _len) {//start from head - 1
            data[i] = data[i+1];
        }
    }
    void shift_right(unsigned int ind) {
        for (unsigned int i = tail; i != ind; i = (i-1) / _len) {//think about how to go correctly
            if (i == _len) {
                data[i] = data[0];
                continue;
            }
            data[i] = data[i-1];
            if (i == 0) {
                i = _len;
            }
        }
    }

public:

    int add(int to_add){
        unsigned int length = now_len();
        if (length >= _len){
            return 0;
        }
        unsigned int ind = search_place(to_add);
        if (ind == head) {
            if (head == 0) {
                head = _len;
            }else {
                head = (head - 1) / _len;
            }
            data[head] = to_add;
            return 1;
        }else if (ind  == tail) {
            tail = (tail + 1) / _len;
            data[tail] = to_add;
            return 1;
        }
        if (ind - head <= length /2) {
            shift_left(ind);
            if (head == 0) {
                head = _len;
            }else {
                head = (head - 1) / _len;
            }
        }else {
            shift_right(ind);
            tail = (tail + 1) / _len;
        }
        data[ind] = to_add;
        return 1;
    }

    int remove() {
        if (head==tail || now_len() == 0) {
            throw std::invalid_argument("Can`t delete smth, because queue is empty");
        }
        int to_return = data[head];
        if (head < _len) {
            head++;
        }else {
            head = 0;
        }
        return to_return;
    }

    unsigned int now_len() {
        if (tail > head) {
            return tail - head;
        }
        return _len - head + tail;
    }

    void print() {
        if (head > tail) {
            for (unsigned int i = head; i < _len + 1; i++) {
                std::cout << data[i]<<std::endl;
            }
            for (unsigned int i = 0; i< tail; i ++) {
                std::cout << data[i]<<std::endl;
            }
        }else if (tail > head) {
            for (unsigned int i = head; i< tail; i ++) {
                std::cout << data[i]<<std::endl;
            }
        }else {
            std::cout<<"Is Empty"<<std::endl;
        }
    }

    QueueWithPriority(unsigned int n=10) {
        if (n == 0) {
            throw std::invalid_argument("Wrong number!\n n should be bigger then 0");
        }
        if (n> 4294967295) {
            throw std::invalid_argument("Wrong number!\n n should be lower then 4294967295");
        }
        data = new int[n];
        head = 0;
        tail = 0;
        _len = n;
    }
    ~ QueueWithPriority() {
        delete[] data;
        data = nullptr;
        head = -1;
        tail = -1;
    }

};

int main() {
    std::cout<<"Hello world!";
    return 0;
}