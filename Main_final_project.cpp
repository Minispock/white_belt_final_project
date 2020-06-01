#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Date {
public:
    Date(int n_year, int n_month, int n_day) {
        year = abs(n_year);
        if (n_month < 1 || n_month > 12) {
            throw logic_error("Month value is invalid: " + to_string(n_month));
        }
        month = n_month;
        if (n_day < 1 || n_day > 31) {
            throw logic_error("Day value is invalid: " + to_string(n_day));
        }
        day = n_day;
    }

    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }

private:
    int year = 0;
    int month = 0;
    int day = 0;
};

bool operator<(const Date& d1, const Date& d2) {
    return vector<int>{d1.GetYear(), d1.GetMonth(), d1.GetDay()} <
        vector<int>{d2.GetYear(), d2.GetMonth(), d2.GetDay()};
}

ostream& operator<<(ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() << "-"
        << setw(2) << setfill('0') << date.GetMonth() << "-"
        << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

class datebase {
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
    }

    set<string> Find(const Date& date) const {
        if (events.count(date) > 0) {
            return events.at(date);
        }
        else {
            return {};
        }
    }

    bool DeleteEvent(const Date& date, const string& s) {
        if (events.count(date) > 0 && events[date].count(s) > 0) {
            events[date].erase(s);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date& date) {
        if (events.count(date) > 0) {
            const unsigned int event_count = events[date].size();
            events.erase(date);
            return event_count;
        }
        return 0;
    }

    void Print() const {
        for (const auto& item : events) {
            for (const auto& event : item.second) {
                cout << item.first << " " << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> events;
};

void EnsureNextCharAndSkip(stringstream& s, const string& date) {
    if (s.peek() != '-') {
        throw logic_error("Wrong date format: " + date);
    }
    s.ignore(1);
}

Date ParseDate(const string& date) {
    stringstream date_stream(date);
    int year, month, day;
    date_stream >> year;
    EnsureNextCharAndSkip(date_stream, date);
    date_stream >> month;
    EnsureNextCharAndSkip(date_stream, date);
    date_stream >> day;
    if (!date_stream.eof() || !date_stream) {
        throw logic_error("Wrong date format: " + date);
    }
    return { year, month, day };
}

int main() {
    try {
        datebase db;
        string command_line;
        while (getline(cin, command_line)) {
            stringstream ss(command_line);
            string command;
            ss >> command;
            if (command.empty()) {
                continue;
            }
            else if (command == "Add") {
                string date, event;
                ss >> date >> event;
                const Date d = ParseDate(date);
                db.AddEvent(d, event);
            }
            else if (command == "Del") {
                string date, event;
                ss >> date;
                if (!ss.eof()) {
                    ss >> event;
                }
                const Date d = ParseDate(date);
                if (event.empty()) {
                    cout << "Deleted " << db.DeleteDate(d) << " events" << endl;
                }
                else {
                    if (db.DeleteEvent(d, event)) {
                        cout << "Deleted successfully" << endl;
                    }
                    else {
                        cout << "Event not found" << endl;
                    }
                }
            }
            else if (command == "Find") {
                string date;
                ss >> date;
                const Date d = ParseDate(date);
                for (const auto& event : db.Find(d)) {
                    cout << event << endl;
                }
            }
            else if (command == "Print") {
                db.Print();
            }
            else {
                throw logic_error("Unknown command: " + command);
            }
        }
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}