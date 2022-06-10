#include "Fields.h"

void Fields::clear() {
	name.clear();
	description.clear();
	frequency.clear();
	dates.clear();
	comments.clear();
}

void Fields::print() {
	cout << " " << get_name();
	calc_spaces(get_name(), 20);
	cout << get_freq() << endl;
	cout << " DESCRIPTION: " << get_des() << endl << endl;
}

void calc_spaces(string target, int num) {
	cout << return_spaces(target, num);
}

string return_spaces(string target, int num) {
	string spaces;
	for (int i = target.size(); i <= num; i++) {
		spaces.append(" ");
	}
	return spaces;
}

const string currentDate() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%m%d%Y", &tstruct);
	return buf;
}

const string currentMonth() {
	string month;
	month.append(currentDate(), 0, 2);
	return month;
}

const string currentYear() {
	string year;
	year.append(currentDate(), 4, 4);
	return year;
}

const string currentMonth_word() {
	string month = currentMonth();
	if (month == "01")
		return "January";
	else if (month == "02")
		return "February";
	else if (month == "03")
		return "March";
	else if (month == "04")
		return "April";
	else if (month == "05")
		return "May";
	else if (month == "06")
		return "June";
	else if (month == "07")
		return "July";
	else if (month == "08")
		return "August";
	else if (month == "09")
		return "September";
	else if (month == "10")
		return "October";
	else if (month == "11")
		return "November";
	else if (month == "12")
		return "December";
}

int process_date1(string target) {
	string temp = target;
	for (int i = 0; i < 4; i++) {
		temp[i] = temp[i + 4];
	}
	for (int i = 0; i < 4; i++) {
		temp[i + 4] = target[i];
	}
	return stoi(temp);
}

string process_date2(int target) {
	string temp = to_string(target);
	string result = temp;
	for (int i = 0; i < 4; i++) {
		result[i] = result[i + 4];
	}
	for (int i = 0; i < 4; i++) {
		result[i + 4] = temp[i];
	}
	return result;
}