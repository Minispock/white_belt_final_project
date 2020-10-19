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

class Date
{
public:
	Date(unsigned int n_year, unsigned int n_month, unsigned int n_day)
	{
		try
		{
			year = n_year;

			if (n_month > 12)
			{
				throw logic_error("Month value is invalid: " + to_string(n_month));
			}
			month = n_month;
			if (n_day > 31)
			{
				throw logic_error("Day value is invalid: " + to_string(n_day));
			}
			day = n_day;
		}
		catch (const exception& e)
		{
			cout << e.what() << endl;
		}
	}

	unsigned int GetYear() const
	{
		return year;
	}
	unsigned int GetMonth() const
	{
		return month;
	}
	unsigned int GetDay() const
	{
		return day;
	}

private:
	unsigned int year = 0;
	unsigned int month = 0;
	unsigned int day = 0;
};

class Database
{
public:
	//do it if you want your functions to have access to class private members
	friend ostream& operator<<(ostream& sstream, const Date& date);
	friend ostream& operator<<(ostream& stream, Database datebase);
	friend bool operator<(const Date& d1, const Date& d2);

	void AddEvent(const Date& date, const string& event)
	{
		if (date.GetDay() > 0 && date.GetMonth() > 0)
		{
			events[date].insert(event);
		}
	}

	//set<string> Find(const Date& date) const 
	//{
	//	if (events.count(date) > 0) 
	//	{
	//		return events.at(date);
	//	}
	//	else 
	//	{			
	//		//Return an object of the function's return type initialized with an empty list-initializer. In a return statement with braced-init-list 
	//		//used as the return expression and list-initialization initializes the returned object			
	//		return {};			
	//	}
	//}

	void Find(const Date& date) const
	{
		if (events.count(date) > 0)
		{
			for (string s : events.at(date))
			{
				cout << s << endl;
			}
		}
	}

	bool DeleteEvent(const Date& date, const string& event)
	{
		if (events.count(date) > 0 && events[date].count(event) > 0)
		{
			events[date].erase(event);
			return true;
		}

		for (auto it = events.begin(); it != events.end();)
		{
			if (it->second.empty() == true)
				it = events.erase(it);
			else
				++it;
		}
		return false;
	}

	unsigned int DeleteDate(const Date& date)
	{
		if (events.count(date) > 0)
		{
			const unsigned int event_count = events[date].size();
			events.erase(date);
			return event_count;
		}
		return 0;
	}

private:
	map<Date, set<string>> events;
};

void EnsureNextCharAndSkip(stringstream& s, const string& date)
{
	if (s.peek() != '-')
	{
		throw logic_error("Wrong date format: " + date);
	}
	s.ignore(1);
}

Date ParseDate(const string& date)
{
	try
	{
		stringstream date_stream(date);
		int year;
		unsigned int iyear, month, day;
		date_stream >> year;
		if (year < 0)
		{
			year = abs(year);
		}
		iyear = (unsigned int)year;
		EnsureNextCharAndSkip(date_stream, date);
		date_stream >> month;
		EnsureNextCharAndSkip(date_stream, date);
		date_stream >> day;
		if (!date_stream.eof() || !date_stream)
		{
			throw logic_error("Wrong date format: " + date);
		}
		return { iyear, month, day };
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}

//standar output operators don't have defenitions for constant streams. P.S. Getted const parameter can not be changed in the function body
//The overloads of operator>> and operator<< that take a std::istream& or std::ostream& as the left hand argument are known 
//as insertion and extraction operators. Since they take the user-defined type as the right argument (b in a@b), they must be implemented as non-members.
ostream& operator<<(ostream& sstream, const Date& date)
{
	sstream << setw(4) << setfill('0') << date.GetYear() << "-"
		<< setw(2) << setfill('0') << date.GetMonth() << "-"
		<< setw(2) << setfill('0') << date.GetDay();
	return sstream;
}

bool operator<(const Date& d1, const Date& d2)
{
	if (d1.GetYear() == d2.GetYear())
	{
		if (d1.GetMonth() == d2.GetMonth())
		{
			return d1.GetDay() < d2.GetDay();
		}
		return d1.GetMonth() < d2.GetMonth();
	}
	return d1.GetYear() < d2.GetYear();
}

ostream& operator<<(ostream& stream, Database datebase)
{
	for (const auto& item : datebase.events)
	{
		for (const auto& event : item.second)
		{
			stream << item.first << " " << event << endl;
		}
	}
	return stream;
}

int main() {

	Database db;
	string command_line;
	while (getline(cin, command_line))
	{
		stringstream ss(command_line);
		string command;
		ss >> command;
		if (command.empty())
		{
			continue;
		}
		else if (command == "Add")
		{
			string date, event;
			ss >> date >> event;
			const Date d = ParseDate(date);
			db.AddEvent(d, event);
		}
		else if (command == "Del")
		{
			string date, event;
			ss >> date;
			if (!ss.eof())
			{
				ss >> event;
			}
			const Date d = ParseDate(date);
			if (event.empty())
			{
				cout << "Deleted " << db.DeleteDate(d) << " events" << endl;
			}
			else
			{
				if (db.DeleteEvent(d, event))
				{
					cout << "Deleted successfully" << endl;
				}
				else
				{
					cout << "Event not found" << endl;
				}
			}
		}
		else if (command == "Find")
		{
			string date;
			ss >> date;
			const Date d = ParseDate(date);
			db.Find(d);
		}
		else if (command == "Print")
		{
			cout << db;
		}
		else if (command == "Exit")
		{
			exit(0);
		}
		else
		{
			cout << "Unknown command: " << command << endl;
			continue;
		}
	}
	return 0;
}
