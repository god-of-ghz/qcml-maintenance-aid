#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <fstream>

using namespace std;

void calc_spaces(string target, int num);
string return_spaces(string target, int num);
const string currentDate();
const string currentMonth();
const string currentYear();
const string currentMonth_word();
int process_date1(string target);		//converts a string date of the form MMDDYYYY to an integer that can be numerically compared
string process_date2(int target);		//converts that integer used for comparison back to its original form



class Fields {
	string name;
	string description;
	string frequency;
	vector <string> dates;
	vector <string> comments;
public:
	Fields() { ; }

	string get_name() { return name; }
	string get_des() { return description; }
	string get_freq() { return frequency; }
	vector <string> get_all_dates() { return dates; }
	vector <string> get_all_comm() { return comments; }
	string get_date(int index) { return dates[index]; }
	string get_comm(int index) { return comments[index]; }

	void set_name(string a) { name = a; }
	void set_des(string a) { description = a; }
	void set_freq(string a) { frequency = a; }
	void set_all_dates(vector <string> a) { dates = a; }
	void set_all_comm(vector <string> a) { comments = a; }
	void add_date(string a) { dates.push_back(a); }
	void add_comment(string a) { comments.push_back(a); }

	void change_date(int index, string a) { dates[index] = a; }
	void change_comment(int index, string a) { comments[index] = a; }

	void clear();

	void print();

};