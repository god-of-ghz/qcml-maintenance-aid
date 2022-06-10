#include "Machines.h"

void command_center(vector <Machines>& equipment);								//main coordination center for the program
void prep_screen(string menu);													//clears the screen and gets the appropriate menu ready
void cc_add(vector <Machines>& equipment);										//function for adding a new machine
void cc_save(vector <Machines>& equipment);										//function for saving changes
void cc_print(vector <Machines>& equipment);									//function for listing all the machines
void cc_edit(vector <Machines>& equipment, int index);							//function for editing the machines and their fields
void cc_upcoming(vector <Machines>& equipment, int machine);					//function for viewing upcoming maintenance
void cc_maintenance(vector <Machines> &equipment);								//function for doing maintenance
void cc_search(vector <Machines> &equipment, int index);						//function for bringing up information for a single machine

void write_machine(ofstream& writefile, Machines& target);						//write a machine to a designated stream
void write_field(ofstream& writefile, Fields& target);							//write a field to a stream
void write_task(ofstream& writefile, Tasks& target);							//write a maintenance task to a stream

void read(vector <Machines>& equipment);										//read the data from the file

int find_index(vector <Machines>& equipment, string code_or_name);				//from a vector of Machines-type objects, return the index for the one with this barcode or name
int find_index(vector <Fields>& fieldlist, string name);						//from a vector of Fields-type objects, return the index for the one with this name
int find_index(vector <Tasks>& tasklist, string name);							//from a vector of Tasks-type objects, return the index for the one with this name

Tasks & find_latest_maintenance(Fields& field, vector <Tasks> & tasklist);		//returns the latest instance of maintenance matching the name of the field
bool ever_done(Fields field, vector <Tasks> & tasklist);						//checks if maintenance has ever been done for this field
vector <Tasks> build_upcoming(vector <Machines>& equipment, int machine);		//builds the list of the next tasks that need to be done next, if an index is given, builds the stuff only for that machine

bool is_used(vector <Machines>& equipment, string code_or_name);				//returns true if this Machine name or barcode is already used
bool is_used(vector <Fields>& fieldlist, string name);							//returns true if this Field name is already used
bool is_used(vector <Tasks>& tasklist, string name);							//returns true if this Task name is already used

bool is_int(string target);											//returns true if the target is an integer smaller than 100
bool is_double(string target);										//returns true if the target is a double

bool is_proper_freq(string target);									//makes sure the input is a positive integer followed by a unit of time (i.e 3 months, 4 days, 1 year, 2 weeks, etc)
bool is_valid_date(string a);										//checks to make sure the input date is a valid date
bool is_unit_of_time(string target);								//makes sure the unit of time is legitimate (days, weeks, months and years only, no fortnights and such)
bool is_before(string date1, string date2);							//returns true if date1 is before date2
int date_diff(string date1, string date2);							//returns the number of days between 1 and 2, negative if date2 is after date1


string next_date(string date, string frequency);					//put in a date and the frequency (like 4 weeks) and it'll give you the next date
vector <string> split_date(string target);							//splits the frequency (4 weeks) into its two halves, a string of '4' and a string of 'weeks'
int date_type(string word);											//takes in the unit of time, days, weeks etc and returns a value 1-4
int total_units_to_add(int date_type, int num_times);				//takes in the above 1-4 value, as well as the number from the split function, and returns a number that must be added to the date (ex, 2 months would be 200)
string add_days(string date, int& remaining_days);					//takes in a current string date and a number of days, intelligently adds the days to it, incrementing the month/year as necessary
string add_weeks(string date, int& remaining_weeks);				//takes in a current string date and a number of weeks, adding them (as days) and incrementing month and year as necessary
string add_months(string date, int& remaining_months);				//takes in a current string date and a number of months, adding the months, incrementing the year as necessary and altering the date (if necessary)
string add_years(string date, int& remaining_years);				//takes in a current string date and a number of years, adding the years, adjusting for leap years as necessary

void main() {
	vector <Machines> equipment;
	read(equipment);
	command_center(equipment);
	return;
}

void command_center(vector <Machines>& equipment) {
	string command;
	string temp;
	string freq;
	while (command != "exit" && command != "back") {
		prep_screen("main");
		if (equipment.size() == 0)
			cout << "\nThere was most likely an error while reading the database. No data has been loaded!\n";
		cc_upcoming(equipment, -1);
		cout << "\n Choose from one of the following commands.\n";
		cout << " 1. Add new machine\n 2. Save changes\n 3. List the machines\n 4. Edit a machine and its fields\n 5. Perform maintenance\n 6. Search for a single machine\n\n";
		cin >> command;
		if (command == "1" || command == "add") {
			cc_add(equipment);
		}
		else if (command == "2" || command == "save") {
			cc_save(equipment);
		}
		else if (command == "3" || command == "print" || command == "list") {
			cc_print(equipment);
		}
		else if (command == "4" || command == "edit" || command == "Edit")
			cc_edit(equipment, -1);
		else if (command == "5" || command == "maintenance" || command == "perform")
			cc_maintenance(equipment);
		else if (command == "6" || command == "search")
			cc_search(equipment, -1);
		else if (command == "X" || command == "beta") {											//hidden testing function, used for debugging
			while (1) {
				cout << "Enter a date, MMDDYYYY\n";
				cin >> temp;
				cout << "Enter a frequency.\n";
				cin.ignore(256, '\n');
				getline(cin, freq);
				if (is_proper_freq(freq) && is_valid_date(temp))
					cout << "Next date is: " << next_date(temp, freq) << endl;
				else {
					if (!is_proper_freq(freq))
						cout << "Frequency check failed!\n";
					if (!is_valid_date(temp))
						cout << "Date check failed!\n";
					cout << "You done fucked up, try again.\n";
				}
			}
		}
	}
	return;
}

void cc_search(vector <Machines>& equipment, int index) {
	prep_screen("list");
	string temp;
	cin.ignore(256, '\n');
	while (temp != "back") {
		prep_screen("list");
		cout << " #     CODE       NAME               \n";
		cout << " ------------------------------------\n";
		for (int i = 0; i < equipment.size(); i++) {
			cout << " " << i + 1;
			calc_spaces(to_string(i + 1), 5);
			equipment[i].print_simple();
		}
		if (index >= 0 && index < equipment.size())
			equipment[index].print();
		index = -1;
		while (index == -1) {	
			cout << "Enter the number, name or barcode of the machine you would like to see the information for.\n";
			getline(cin, temp);
			if (temp == "back")
				return;
			else if (is_int(temp))
				index = stoi(temp) - 1;
			else
				index = find_index(equipment, temp);
			if (index < 0 || index >= equipment.size()) {
				index = -1;
				cout << "That is not a valid option. Please try again.\n";
			}
		}
	}
}

void cc_maintenance(vector <Machines>& equipment) {
	string temp;
	int index = -1;
	int f_index = -1;
	Tasks temp_task;
	cin.ignore(256, '\n');
	while (temp != "back") {
		prep_screen("maintenance");
		cout << " Please select the machine you would like to perform maintenance on.\n\n";
		cout << " #     CODE       NAME               \n";
		cout << " ------------------------------------\n";
		for (int i = 0; i < equipment.size(); i++) {
			cout << " " << i + 1;
			calc_spaces(to_string(i + 1), 5);
			equipment[i].print_simple();
		}
		cout << endl;
		while (1) {
			getline(cin, temp);
			if (temp == "back" || temp == "done")
				return;
			if (is_int(temp)) {
				index = stoi(temp) - 1;
			}
			else {
				index = find_index(equipment, temp);
			}
			if (index < 0 || index >= equipment.size())
				cout << "That machine was not found. Please try again.\n";
			else
				break;
		}
		while (temp != "back") {
			prep_screen("maintenance");
			if (f_index != -1) {
				cout << equipment[index].get_field(f_index).get_name() << " maintenance for " << equipment[index].get_name() << " completed.\n\n";
				f_index = -1;
				cin.ignore(256, '\n');
			}
			cout << "Data for " << equipment[index].get_name() << " loaded.\n";
			cc_upcoming(equipment, index);
			equipment[index].print_maint_history();
			cout << "\nPlease select the type of maintenance you want to perform.\n\n";
			for (int i = 0; i < equipment[index].get_all_fields().size(); i++) {
				cout << i + 1;
				calc_spaces(to_string(i + 1), 5);
				cout << equipment[index].get_field(i).get_name();
				calc_spaces(equipment[index].get_field(i).get_name(), 20);
				cout << equipment[index].get_field(i).get_des() << endl;
			}
			while (temp != "back") {
				getline(cin, temp);
				if (temp == "back" || temp == "done")
					break;
				if (is_int(temp))
					f_index = stoi(temp) - 1;
				else {
					f_index = find_index(equipment[index].get_all_fields(), temp);
				}
				if (f_index < 0 || f_index >= equipment[index].get_all_fields().size())
					cout << "That is not a valid maintenance type. Please try again.\n";
				else
					break;
			}
			while (temp != "back" && temp != "done") {
				cout << "Enter your initials.\n";
				cin >> temp;
				if (temp == "done" || temp == "bacK")
					break;
				else if (temp.size() != 2 && temp.size() != 3)
					cout << "Be sure to enter JUST your initials with no spaces. Try again.\n";
				else {
					temp_task.intials = temp;
					break;
				}
			}
			while (temp != "back" && temp != "done") {
				cout << "Enter the date of the maintenance, MMDDYYYY. Entering 'today' will put in today's date.\n";
				cin >> temp;
				if (temp == "today")
					temp = currentDate();
				if (temp == "back" || temp == "done")
					break;
				else if (!is_valid_date(temp))
					cout << "There was a problem with the date you entered, please try again.\n";
				else {
					temp_task.date = temp;
					break;
				}
			}
			while (temp != "back" && temp != "done") {
				cout << "Enter any comments you have about the maintenance done.\n";
				cin.ignore(256, '\n');
				getline(cin, temp);
				if (temp == "back")
					break;
				temp_task.comment = temp;
				cout << "You entered: " << temp_task.comment << "\nIs this okay?\n";
				cin >> temp;
				if (temp == "y" || temp == "yes")
					break;
			}
			if (temp != "back" && temp != "done") {
				temp_task.field = equipment[index].get_field(f_index).get_name();
				equipment[index].add_task(temp_task);
			}
			else
				break;
		}
		index = -1;
		cin.ignore(256, '\n');
	}
}

void cc_upcoming(vector <Machines>& equipment, int machine) {
	vector <Tasks> tasklist = build_upcoming(equipment, machine);
	cout << "\n TODAY'S MAINTENANCE\n";
	cout << " MACHINE NAME         FIELD                \n";
	cout << " ------------------------------------------\n";
	vector <Tasks> todaylist;
	for (int i = 0; i < tasklist.size(); i++) {
		if (tasklist[i].date == currentDate())
			todaylist.push_back(tasklist[i]);
	}
	if (todaylist.size() == 0)
		cout << " No maintenance scheduled for today.\n";
	else {
		for (int i = 0; i < todaylist.size(); i++) {
			cout << " ";
			cout << todaylist[i].machine;
			calc_spaces(todaylist[i].machine, 20);
			cout << todaylist[i].field;
			calc_spaces(todaylist[i].field, 20);
			cout << endl;
		}
		cout << endl;
	}
	cout << "\n UPCOMING MAINTENANCE\n";
	cout << " MACHINE NAME         FIELD                DONE BY DATE\n";
	cout << " ------------------------------------------------------\n";
	if (tasklist.size() == 0) {
		cout << " No upcoming maintenance to report.\n";
		return;
	}
	int end;
	if (tasklist.size() < 10)
		end = tasklist.size();
	else
		end = 10;
	for (int i = 0; i < end; i++) {
		cout << " ";
		cout << tasklist[i].machine;
		calc_spaces(tasklist[i].machine, 20);
		cout << tasklist[i].field;
		calc_spaces(tasklist[i].field, 20);
		cout << tasklist[i].date;
		if (is_before(tasklist[i].date, currentDate())) {
			calc_spaces(tasklist[i].date, 10);
			cout << "*OVERDUE*";
		}
		cout << endl;
	}
	cout << endl << endl;
}

vector <Tasks> build_upcoming(vector <Machines>& equipment, int machine) {
	vector <Tasks> upcoming;
	Tasks temp_task;
	if (machine == -1) {
		for (int i = 0; i < equipment.size(); i++) {
			for (int j = 0; j < equipment[i].get_all_fields().size(); j++) {
				if (ever_done(equipment[i].get_field(j), equipment[i].get_all_tasks())) {
					temp_task = find_latest_maintenance(equipment[i].get_field(j), equipment[i].get_all_tasks());
					temp_task.machine = equipment[i].get_name();
					temp_task.date = next_date(temp_task.date, equipment[i].get_field(j).get_freq());
					upcoming.push_back(temp_task);
				}
			}
		}
	}
	else {
		for (int j = 0; j < equipment[machine].get_all_fields().size(); j++) {
			if (ever_done(equipment[machine].get_field(j), equipment[machine].get_all_tasks())) {
				temp_task = find_latest_maintenance(equipment[machine].get_field(j), equipment[machine].get_all_tasks());
				temp_task.machine = equipment[machine].get_name();
				temp_task.date = next_date(temp_task.date, equipment[machine].get_field(j).get_freq());
				upcoming.push_back(temp_task);
			}
		}
	}
	//cout << "List assembled.\n";
	vector <Tasks> upcoming_ordered;
	int index;
	for (int i = 0; i < upcoming.size(); i++) {
		index = i;
		for (int j = 0; j < upcoming.size(); j++) {
			if (is_before(upcoming[j].date, upcoming[index].date))
				index = j;
		}
		temp_task = upcoming[index];
		upcoming_ordered.push_back(temp_task);
		upcoming[index].date = "999999999";
	}
	//cout << "Build successful.\n";
	return upcoming_ordered;
}

void cc_edit(vector <Machines>& equipment, int index) {
	string temp;
	int num = index;
	int fnum = -1;
	Fields temp_field;
	while (temp != "back") {
		if (num == -1) {
			cin.ignore(256, '\n');
			while (num == -1) {
				cout << "Please enter the name or barcode of the machine you are trying to edit.\n";
				getline(cin, temp);
				if (temp == "back")
					return;
				num = find_index(equipment, temp);
				if (num == -1) {
					cout << "That barcode or name was not found. Please enter a valid machine name or code.\n";
				}
			}
		}
		while (temp != "back") {
			prep_screen("edit");
			cout << "Which property of " << equipment[num].get_name() << " would you like to edit? Enter 'done' to change machines or 'back' to exit entirely.\n";
			cout << "\n1. Barcode\n2. Name\n3. Description\n4. A maintenance field\n\n";
			cin >> temp;
			if (temp == "back" || temp == "done")
				break;
			else if (temp == "1" || temp == "code" || temp == "barcode" || temp == "Barcode") {
				while (1) {
					cout << "Please enter the new barcode for this machine, less than 10 characters.\nThe current value is " << equipment[num].get_code() << endl;
					cin >> temp;
					if (is_used(equipment, temp))
						cout << "That barcode is already in use. Please enter an unused code.\n";
					else if (temp.size() > 10)
						cout << "That barcode is too large, please enter one less than 10 characters.\n";
					else if (temp == "done" || temp == "back")
						break;
					else {
						equipment[num].set_code(temp);
						break;
					}
				}
			}
			else if (temp == "2" || temp == "name" || temp == "Name") {
				while (1) {
					cout << "Please enter the new name for this machine, less than 20 characters.\nThe current name is " << equipment[num].get_name() << endl;
					cin.ignore(256, '\n');
					getline(cin, temp);
					if (is_used(equipment, temp))
						cout << "That name is already in use. Please enter an unused name.\n";
					else if (temp.size() > 20)
						cout << "That name is too large, please enter one less than 20 characters.\n";
					else if (temp == "done" || temp == "back")
						break;
					else {
						equipment[num].set_name(temp);
						break;
					}
				}
			}
			else if (temp == "3" || temp == "des" || temp == "description" || temp == "Description") {
				while (temp != "back") {
					cout << "Please enter the new description for this machine.\nThe current description is: " << equipment[num].get_des() << endl;
					cin.ignore(256, '\n');
					getline(cin, temp);
					if (temp == "done" || temp == "back")
						break;
					equipment[num].set_des(temp);
					cout << "You entered: \n\n" << equipment[num].get_des() << endl << endl;
					cout << "Is this ok? Yes or no?\n";
					cin >> temp;
					if (temp == "yes" || temp == "y")
						break;
				}
			}
			else if (temp == "4" || temp == "field" || temp == "fields") {
				while (temp != "back") {
					prep_screen("edit");
					cout << "Editing fields for: " << equipment[num].get_name() << endl;
					cout << "\n1. Add new field\n";
					for (int i = 0; i < equipment[num].get_all_fields().size(); i++) {
						cout << i + 2 << ". " << equipment[num].get_field(i).get_name() << endl;
					}
					cout << "Select the field you would like to edit.\n";
					cin >> temp;
					if (temp == "back" || temp == "done")
						break;
					else if (temp == "1" || temp == "Add" || temp == "add") {
						cin.ignore(256, '\n');
						while (temp != "back") {
							temp_field.clear();
							cout << "\nPlease enter a name for the new maintenance type, less than 20 characters. Or type done if you don't want to add any more fields.\n";
							getline(cin, temp);
							if (temp == "done" || temp == "DONE" || temp == "back")
								break;
							if (temp != "back") {
								if (temp.size() > 20) {
									cout << "That name is too long, please enter one less than 10 characters.\n";
									continue;
								}
								temp_field.set_name(temp);
								break;
							}
						}
						if (temp == "done" || temp == "DONE")
							break;
						while (temp != "back") {
							cout << "Please enter a short description for this maintenance type.\n";
							getline(cin, temp);
							if (temp != "back") {
								temp_field.set_des(temp);
								cout << "You entered: \n\n" << temp_field.get_des() << endl << "\nIs this ok? Yes or no.\n";
								cin >> temp;
								cin.ignore(256, '\n');
								if (temp == "y" || temp == "yes")
									break;
							}
						}
						while (temp != "back") {
							cout << "Please enter how often this maintenance must be done with a positive integer and a word (day(s), week(s), month(s) or year(s))\n";
							cout << "For example: 3 days, 4 weeks, 1 month, 2 years are all valid inputs, while -5.4 fortnights is not.\n\n";
							//cin.ignore(256, '\n');
							getline(cin, temp);
							if (temp == "back" || temp == "done")
								break;
							else if (is_proper_freq(temp)) {
								temp_field.set_freq(temp);
								break;
							}
							else
								cout << "That is not a properly formatted date- please try again.\n";
						}
						equipment[num].add_field(temp_field);
						continue;
					}
					else if (is_int(temp))
						fnum = stoi(temp) - 2;
					else 
						fnum = find_index(equipment[num].get_all_fields(), temp);
					if (fnum < 0 || fnum >= equipment[num].get_all_fields().size()) {
						cout << "Please select a valid option.\n";
						continue;
					}
					while (temp != "back") {
						cout << "What part of " << equipment[num].get_all_fields()[fnum].get_name() << " would you like to edit?\n";
						cout << "\n1. Name\n2. Description\n3. Frequency\n\n";
						cin >> temp;
						cin.ignore(256, '\n');
						if (temp == "back" || temp == "done")
							break;
						else if (temp == "1" || temp == "Name" || temp == "name") {
							while (1) {
								cout << "Please enter the new name for this maintenance type, less than 20 characters.\n";
								cout << "Current name is: " << equipment[num].get_all_fields()[fnum].get_name() << endl;
								getline(cin, temp);
								if (temp == "back" || temp == "done")
									break;
								if (is_used(equipment[num].get_all_fields(), temp))
									cout << "That name is already in use, please enter a name that is not being currently used";
								else if (temp.size() > 20)
									cout << "That name is too long, please enter one less than 20 characters.\n";
								else {
									equipment[num].get_all_fields()[fnum].set_name(temp);
									break;
								}
							}
						}
						else if (temp == "2" || temp == "des" || temp == "description" || temp == "Description") {
							while (1) {
								cout << "Please enter the new description for this maintenance type.\n";
								cout << "Current description is: " << equipment[num].get_field(fnum).get_des() << endl;
								getline(cin, temp);
								if (temp == "back" || temp == "done")
									break;
								equipment[num].get_field(fnum).set_des(temp);
								cout << "You entered: \n\n" << equipment[num].get_field(fnum).get_des() << endl << endl;
								cout << "Is this ok? Yes or no?\n";
								cin >> temp;
								cin.ignore(256, '\n');
								if (temp == "yes" || temp == "y")
									break;
							}
						}
						else if (temp == "3" || temp == "freq" || temp == "frequency" || temp == "Frequency") {
							while (1) {
								cout << "Please enter the new frequency for this maintenance type.\n";
								cout << "Current frequency is: " << equipment[num].get_field(fnum).get_freq() << endl;
								getline(cin, temp);
								if (temp == "back" || temp == "done")
									break;
								else if (is_proper_freq(temp)) {
									equipment[num].get_field(fnum).set_freq(temp);
									break;
								}
								else
									cout << "That is not a properly formatted date, please try again.\n";
							}
						}
					}
				}
				fnum = -1;
			}
			else {
				cout << "That is not a valid option; please make a choice from the provided menu.\n";
			}
		}
		num = -1;
	}
}

void cc_save(vector <Machines>& equipment) {
	ofstream writefile;
	string temp;
	prep_screen("save");
	cout << "Would you like to save the changes?\n";
	cin >> temp;
	if (temp != "y" && temp != "yes")
		return;
	writefile.open("machines.qcml");
	if (!writefile.is_open()) {
		cout << "There was an error while writing the database. No changes have been saved! Press any key to go back.\n";
		cin >> temp;
		return;
	}
	for (int i = 0; i < equipment.size(); i++) {
		write_machine(writefile, equipment[i]);
	}
	writefile.close();
	return;
}

void cc_print(vector <Machines>& equipment) {
	prep_screen("list");
	for (int i = 0; i < equipment.size(); i++) {
		equipment[i].print();
	}
	cout << "Press any key to go back.\n";
	string temp;
	cin >> temp;
	return;
}

void cc_add(vector <Machines>& equipment) {
	string temp;
	Machines temp_machine;
	Fields temp_field;
	Tasks temp_task;
	while (temp != "back") {
		temp_machine.clear();
		while (temp != "back") {
			prep_screen("add");
			cout << "Please enter the barcode of the new machine you are trying to add, less than 10 characters.\n";
			cin >> temp;
			if (temp != "back") {
				if (temp.size() > 10) {
					cout << "That barcode is too long, please enter one less than 10 characters.\n";
					continue;
				}
				temp_machine.set_code(temp);
				break;
			}
		}
		cin.ignore(256, '\n');
		while (temp != "back") {
			cout << "Please enter the name of the new machine you are trying to add, less than 15 characters.\n";
			getline(cin, temp);
			if (temp != "back") {
				if (temp.size() > 15) {
					cout << "That name is too long, please enter one less than 15 characters.\n";
					continue;
				}
				temp_machine.set_name(temp);
				break;
			}
		}
		while (temp != "back") {
			cout << "Please enter a short description of the machine.\n";
			getline(cin, temp);
			if (temp != "back") {
				temp_machine.set_des(temp);
				cout << "You entered: \n\n" << temp_machine.get_des() << endl << "\nIs this ok? Yes or no.\n";
				cin >> temp;
				cin.ignore(256, '\n');
				if (temp == "y" || temp == "yes")
					break;
			}
		}
		if (temp != "back") {
			cout << "\nPlease begin entering the different types of maintenance needed for this machine.\n";
			cout << "You will enter the name for each necessary task, a brief description and how often it must be performed.\n\n";
		}
		while (temp != "back") {
			temp_field.clear();
			while (temp != "back") {
				cout << "\nPlease enter a name for this maintenance type, less than 20 characters. Or type done if you don't want to add any more fields.\n";
				getline(cin, temp);
				if (temp == "done" || temp == "DONE" || temp == "back")
					break;
				if (temp != "back") {
					if (temp.size() > 20) {
						cout << "That name is too long, please enter one less than 10 characters.\n";
						continue;
					}
					temp_field.set_name(temp);
					break;
				}
			}
			if (temp == "done" || temp == "DONE")
				break;
			while (temp != "back") {
				cout << "Please enter a short description for this maintenance type.\n";
				getline(cin, temp);
				if (temp != "back") {
					temp_field.set_des(temp);
					cout << "You entered: \n\n" << temp_field.get_des() << endl << "\nIs this ok? Yes or no.\n";
					cin >> temp;
					cin.ignore(256, '\n');
					if (temp == "y" || temp == "yes")
						break;
				}
			}
			while (temp != "back") {
				cout << "Please enter how often this maintenance must be done with a positive integer and a word (day(s), week(s), month(s) or year(s))\n";
				cout << "For example: 3 days, 4 weeks, 1 month, 2 years are all valid inputs, while -5.4 fortnights is not.\n\n";
				getline(cin, temp);
				if (temp == "back" || temp == "done")
					break;
				else if (is_proper_freq(temp)) {
					temp_field.set_freq(temp);
					break;
				}
				else
					cout << "That is not a properly formatted date, please try again.\n";
			}
			temp_machine.add_field(temp_field);
		}
		if (temp == "back")
			return;
		equipment.push_back(temp_machine);
	}
	return;
}

void prep_screen(string menu) {
	system("cls");
	cout << "+---------------------------------------+\n";
	cout << "|        QCML Maintenance Aid v1.0      |\n";
	cout << "+---------------------------------------+\n\n";

	if (menu == "main")
		cout << "~~~~~~~~~~~~~~~~MAIN MENU~~~~~~~~~~~~~~~\n\n";
	else if (menu == "add")
		cout << "~~~~~~~~~~~~~ADD NEW MACHINE~~~~~~~~~~~~\n\n";
	else if (menu == "save")
		cout << "~~~~~~~~~~~~~~~SAVE CHANGES?~~~~~~~~~~~~\n\n";
	else if (menu == "list")
		cout << "~~~~~~~~~~~~~~~MACHINE LIST~~~~~~~~~~~~~\n\n";
	else if (menu == "edit")
		cout << "~~~~~~~~~~~~~~~~~EDITING~~~~~~~~~~~~~~~~\n\n";
	else if (menu == "maintenance")
		cout << "~~~~~~~~~~~~~MAINTENANCE MENU~~~~~~~~~~~\n\n";
	else
		cout << "~~~~~~~~~~~~~~~COMING SOON!~~~~~~~~~~~~~\n\n";

	return;
}

void write_machine(ofstream& writefile, Machines& target) {
	writefile << target.get_code() << endl;
	writefile << target.get_name() << endl;
	writefile << target.get_des() << endl;
	writefile << target.get_all_fields().size() << endl;
	for (int i = 0; i < target.get_all_fields().size(); i++) {
		write_field(writefile, target.get_field(i));
	}
	writefile << target.get_all_tasks().size() << endl;
	for (int i = 0; i < target.get_all_tasks().size(); i++) {
		write_task(writefile, target.get_task(i));
	}
	return;
}

void write_field(ofstream& writefile, Fields& target) {
	writefile << target.get_name() << endl;
	writefile << target.get_des() << endl;
	writefile << target.get_freq() << endl;
}

void write_task(ofstream& writefile, Tasks& target) {
	writefile << target.field << endl;
	writefile << target.date << endl;
	writefile << target.intials << endl;
	writefile << target.comment << endl;
}

void read(vector <Machines>& equipment) {
	ifstream readfile;
	readfile.open("machines.qcml");
	if (!readfile.is_open()) {
		cout << "There was a problem loading the database, please check that the file is not in use.\n";
		return;
	}
	Machines temp_machine;
	Fields temp_field;
	Tasks temp_task;
	string temp;
	int num = 0;
	while (!readfile.eof()) {
		temp_machine.clear();
		getline(readfile, temp); temp_machine.set_code(temp);
		getline(readfile, temp); temp_machine.set_name(temp);
		getline(readfile, temp); temp_machine.set_des(temp);
		readfile >> num;
		readfile.ignore(256, '\n');
		for (int i = 0; i < num; i++) {
			temp_field.clear();
			getline(readfile, temp); temp_field.set_name(temp);
			getline(readfile, temp); temp_field.set_des(temp);
			getline(readfile, temp); temp_field.set_freq(temp);
			temp_machine.add_field(temp_field);
		}
		readfile >> num;
		readfile.ignore(256, '\n');
		for (int i = 0; i < num; i++) {
			getline(readfile, temp); temp_task.field = temp;
			getline(readfile, temp); temp_task.date = temp;
			getline(readfile, temp); temp_task.intials = temp;
			getline(readfile, temp); temp_task.comment = temp;
			temp_machine.add_task(temp_task);
		}
		if (readfile.eof())			// check to make sure we read in a complete set of data (don't want to add garbage)
			break;
		equipment.push_back(temp_machine);
	}
	readfile.close();
	return;
}

int find_index(vector <Machines>& equipment, string code_or_name) {
	for (int i = 0; i < equipment.size(); i++) {
		if (equipment[i].get_code() == code_or_name || equipment[i].get_name() == code_or_name)
			return i;
	}
	return -1;
}

int find_index(vector <Fields>& fieldlist, string name) {
	for (int i = 0; i < fieldlist.size(); i++) {
		if (fieldlist[i].get_name() == name)
			return i;
	}
	return -1;
}

int find_index(vector <Tasks>& tasklist, string name) {
	for (int i = 0; i < tasklist.size(); i++) {
		if (tasklist[i].field == name)
			return i;
	}
	return -1;
}

Tasks & find_latest_maintenance(Fields& field, vector <Tasks> & tasklist) {
	int highest = -1;
	for (int i = 0; i < tasklist.size() ; i++) {
		if (tasklist[i].field == field.get_name()) {
			if (highest = -1)
				highest = i;
			else if (is_before(tasklist[highest].date, tasklist[i].date))
				highest = i;
		}
	}
	//cout << "Latest maintenance: " << tasklist[highest].field << " " << tasklist[highest].date << endl;
	return tasklist[highest];
}

bool ever_done(Fields field, vector <Tasks> & tasklist) {
	for (int i = 0; i < tasklist.size(); i++) {
		if (field.get_name() == tasklist[i].field)
			return true;
	}
	return false;
}

bool is_used(vector <Machines>& equipment, string code_or_name) {
	if (find_index(equipment, code_or_name) == -1)
		return false;
	else 
		return true;
}

bool is_used(vector <Fields>& fieldlist, string name) {
	if (find_index(fieldlist, name) == -1)
		return false;
	else
		return true;
}

bool is_used(vector <Tasks>& tasklist, string name) {
	if (find_index(tasklist, name) == -1)
		return false;
	else
		return true;
}

bool is_int(string target) {								//used for menus mostly, but returns true if the target is a simple integer less than 100
	//cout << "Target: " << target << endl;
	try {
		stoi(target);
	}
	catch (invalid_argument ia) {
		return false;
	}
	if (stoi(target) < 0)
		return false;
	bool has_spaces = false;
	for (int i = 0; i < target.size(); i++) {
		//cout << target[i];
		if (target[i] == 32)
			has_spaces = true;
	}
	if (stoi(target) > 100 || has_spaces)
		return false;
	return true;
}

bool is_double(string target) {
	try {
		stod(target);
	}
	catch (invalid_argument ia) {
		return false;
	}
	return true;
}

bool is_proper_freq(string target) {
	string number;
	string type;
	int index = 0;
	for (int i = 0; i < target.size(); i++) {
		if (target[i] == *" ") {
			index = i;
			break;
		}
	}
	if (index <= 0)
		return false;
	number.append(target, 0, index);
	type.append(target, index + 1, target.size() - (index + 1));
	return (is_int(number) && is_unit_of_time(type));
}

bool is_valid_date(string a) {
	string num = "0123456789";
	string month; month.append(a, 0, 2);
	string day; day.append(a, 2, 2);
	string year; year.append(a, 4, 4);
	//check ????????
	if (a == "????????")
		return true;
	//check size
	if (a.size() != 8)
		return false;
	//check that they're actually numbers
	try {
		stoi(month);
	}
	catch (invalid_argument ia) {
		return false;
	}
	try {
		stoi(day);
	}
	catch (invalid_argument ia) {
		return false;
	}
	try {
		stoi(year);
	}
	catch (invalid_argument ia) {
		return false;
	}
	//check valid month
	if (stoi(month) < 1 || stoi(month) > 12)
		return false;
	//check to make sure its not in the future
	/*if (process_date1(a) > process_date1(currentDate()))
		return false;*/
	//check sure its a reasonable year
	/*if (stoi(year) < 2009)
		return false;*/
	//check to see if its a valid day for each month (i.e, can't have 35th of May, or 31st of February)
	if (a[0] == num[0]) {
		if (a[1] == num[1] || a[1] == num[3] || a[1] == num[5] || a[1] == num[7] || a[1] == num[8]) {		//Jan, March, May, July, August have 31 days
			if (stoi(day) > 31)
				return false;
		}
		else if (a[1] == num[4] || a[1] == num[6] || a[1] == num[9]) {						//April, June, September have 30 days
			if (stoi(day) > 30)
				return false;
		}
		else if (a[1] == num[2]) {							//February has 28 days, but 29 on a leap year
			if (stoi(year) % 4 == 0) {
				if (stoi(day) > 29)
					return false;
			}
			else {
				if (stoi(day) > 28)
					return false;
			}
		}
	}
	else if (a[0] == num[1]) {
		if (a[0] == num[0] || a[0] == num[2]) {				//October and December have 31 days									
			if (stoi(day) > 31)
				return false;
		}
		else if (a[0] == num[1]) {							//November has 30 days
			if (stoi(day) > 30)
				return false;
		}
	}
	return true;
}

bool is_unit_of_time(string target) {
	int type = date_type(target);
	if (type > 0)
		return true;
	else
		return false;
}

bool is_before(string date1, string date2) {
	return (process_date1(date1) < process_date1(date2));
}

string next_date(string date, string frequency) {
	int num;
	int type;
	num = stoi(split_date(frequency)[0]);
	type = date_type(split_date(frequency)[1]);
	if (type == 1)
		date = add_days(date, num);
	else if (type == 2)
		date = add_weeks(date, num);
	else if (type == 3)
		date = add_months(date, num);
	else if (type == 4)
		date = add_years(date, num);
	return date;
}

int date_diff(string date1, string date2) {
	string day1, day2;
	string month1, month2;
	string year1, year2;
	
	month1.append(date1, 0, 2); month2.append(date2, 0, 2);
	day1.append(date1, 2, 2); day2.append(date2, 2, 2);
	year1.append(date1, 4, 4); year2.append(date2, 4, 4);
	
	//int temp_date1, temp_date2;
	//temp_date1 = process_date1(date1); temp_date2 = process_date1(date2);
	
	int normal_values[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 301, 334 };
	int leap_values[12] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 302, 333 };
	int value1 = 0; int value2 = 0;

	for (int i = 2000; i < stoi(year1); i++) {
		if (i % 4 == 0)
			value1 += 366;
		else
			value1 += 365;
	}
	for (int i = 2000; i < stoi(year2); i++) {
		if (i % 4 == 0)
			value2 += 366;
		else
			value2 += 365;
	}

	if (stoi(year1) % 4 == 0)
		value1 += leap_values[stoi(month1) - 1];
	else
		value1 += normal_values[stoi(month1) - 1];
	if (stoi(year2) % 4 == 0)
		value2 += leap_values[stoi(month2) - 1];
	else
		value2 += normal_values[stoi(month2) - 1];

	value1 += stoi(day1);
	value2 += stoi(day2);

	return value1 - value2;
}

vector <string> split_date(string target) {
	int index;
	string number;
	string type;
	for (int i = 0; i < target.size(); i++) {
		if (target[i] == *" ") {
			index = i;
			break;
		}
	}
	number.append(target, 0, index);
	type.append(target, index + 1, target.size() - (index + 1));
	vector <string> results;
	results.push_back(number);
	results.push_back(type);
	return results;
}

string add_days(string date, int& remaining_days) {
	string month;
	string day;
	string year;
	int temp_date;
	while (remaining_days > 0) {
		day.clear();
		month.clear();
		year.clear();
		day.append(date, 2, 2);
		month.append(date, 0, 2);
		year.append(date, 4, 4);
		temp_date = process_date1(date);
		int end_day = 0;
		if (month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12")
			end_day = 31;
		else if (month == "04" || month == "06" || month == "09" || month == "11")
			end_day = 30;
		else if (month == "02" && (stoi(year) % 4 == 0))
			end_day = 29;
		else
			end_day = 28;
		for (int i = stoi(day); i < end_day; i++) {			//subtract days from remaining_days, add them to the date, until either the end of the month is reached or the days run out, whichever comes first
			if (remaining_days - 1 >= 0) {
				remaining_days--;
			}
			else
				break;
			temp_date++;
		}
		if (remaining_days > 0) {								//if the end of the month is reached and we have days left over, increment the month if any days are left (or reset the months and add a year)
			temp_date -= end_day;
			temp_date++;
			remaining_days--;
			if (month == "12") {
				temp_date -= 1100;
				temp_date += 10000;
			}
			else
				temp_date += 100;
		}
		date = process_date2(temp_date);
	}
	return date;
}

string add_weeks(string date, int& remaining_weeks) {
	int remaining_days = remaining_weeks * 7;
	date = add_days(date, remaining_days);
	return date;
}

int date_type(string word) {
	if (word == "day" || word == "days" || word == "d" || word == "Day" || word == "Days" || word == "DAY" || word == "DAYS" || word == "D")
		return 1;
	else if (word == "week" || word == "weeks" || word == "w" || word == "Week" || word == "Weeks" || word == "WEEK" || word == "WEEKS" || word == "W")
		return 2;
	else if (word == "month" || word == "months" || word == "m" || word == "Month" || word == "Months" || word == "MONTH" || word == "MONTHS" || word == "M")
		return 3;
	else if (word == "year" || word == "years" || word == "y" || word == "Year" || word == "Years" || word == "YEAR" || word == "YEARS" || word == "Y")
		return 4;
	else
		return -1;
}

int total_units_to_add(int date_type, int num_times) {
	if (date_type == 1)
		return num_times * 1;
	else if (date_type == 2)
		return num_times * 7;
	else if (date_type == 3)
		return num_times * 100;
	else if (date_type == 4)
		return num_times * 10000;
}

string add_months(string date, int& remaining_months) {
	string month;
	string day;
	string year;
	bool end_of_month;
	while (remaining_months > 0) {
		month.clear();									//clear the date information out of memory
		day.clear();
		year.clear();
		month.append(date, 0, 2);						//identify the components of the date (month, day and year)
		day.append(date, 2, 2);
		year.append(date, 4, 4);
		int temp_date = process_date1(date);			//convert the string date (MMDDYYY) to an incrementable integer date (YYYYMMDD)
		if ((month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12") && day == "31") //if its the end of the month, just set the date to be the end of the next month
			end_of_month = true;
		else if ((month == "04" || month == "06" || month == "09" || month == "11") && day == "30")
			end_of_month = true;
		else if (month == "02" && (stoi(year) % 4 == 0) && day == "29")
			end_of_month = true;
		else if (month == "02" && (stoi(year) % 4 != 0) && day == "28")
			end_of_month = true;
		else
			end_of_month = false;
		for (int i = stoi(month); i < 12; i++) {		//start adding months, up until the end of the year
			if (remaining_months - 1 >= 0)				//make sure we have a month to add (cancel if we don't have any left)
				remaining_months--;
			else
				break;
			temp_date += 100;							
		}
		if (remaining_months > 0) {						//if we hit the end of the year, but still have months to add, go to next year
			temp_date += 10000;
			temp_date -= 1100;
			remaining_months--;
		}
		month.clear();											//prepare to rebuild the date
		month.append(process_date2(temp_date), 0, 2);
		day.clear();
		day.append(process_date2(temp_date), 2, 2);
		year.clear();
		year.append(process_date2(temp_date), 4, 4);
		if (end_of_month) {										//if it IS the end of the month, just set the date to be the end of the approriate month
			if (month == "01" || month == "03" || month == "05" || month == "07" || month == "08" || month == "10" || month == "12")
				day = "31";
			else if (month == "04" || month == "06" || month == "09" || month == "11")
				day = "30";
			else if (month == "02" && (stoi(year) % 4 == 0))
				day = "29";
			else if (month == "02" && (stoi(year) % 4 != 0))
				day = "28";
		}
		else {													//if not, just make sure the date is ok for february (i.e if its August 30th, the date will adjust accordingly if the next maintenance is in February of next year)
			if (month == "02" && (stoi(year) % 4 == 0)) {
				if (stoi(day) > 29)
					day = "29";
			}
			else if (month == "02" && (stoi(year) % 4 != 0)) {
				if (stoi(day) > 28)
					day = "28";
			}
		}
		date.clear();
		date.append(month);
		date.append(day);
		date.append(year);
	}
	return date;
}

string add_years(string date, int& remaining_years) {
	bool leap_year_adjust;
	string month;
	month.append(date, 0, 2);
	string day;
	day.append(date, 2, 2);
	string year;
	year.append(date, 4, 4);
	if (month == "02") {
		if ((stoi(year) % 4 == 0) && day == "29")
			leap_year_adjust = true;
		else if ((stoi(year) % 4 != 0) && day == "28")
			leap_year_adjust = true;
		else
			leap_year_adjust = false;
	}
	else
		leap_year_adjust = false;
	int temp_date = process_date1(date);
	temp_date += total_units_to_add(4, remaining_years);
	if (leap_year_adjust) {
		string result = process_date2(temp_date);
		month.clear();
		month.append(result, 0, 2);
		day.clear();
		year.clear();
		year.append(result, 4, 4);
		if (stoi(year) % 4 == 0)
			day = "29";
		else
			day = "28";
		result.clear();
		result.append(month);
		result.append(day);
		result.append(year);
		return result;
	}
	else
		return process_date2(temp_date);
}