#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include "Fields.h"
using namespace std;

struct Tasks {
	string field;
	string date;
	string comment;
	string intials;
	string machine;			//mostly unused, except when building tasks to be done
};

class Machines {
	string code;
	string name;
	string description;
	vector <Fields> fields;
	vector <Tasks> tasks;
public:
	Machines() { ; }

	string get_code() { return code; }
	string get_name() { return name; }
	string get_des() { return description; }
	vector <Fields> & get_all_fields() { return fields; }
	vector <Tasks> & get_all_tasks() { return tasks; }
	Fields & get_field(int index) { return fields[index]; }
	Tasks & get_task(int index) { return tasks[index]; }

	void set_code(string a) { code = a; }
	void set_name(string a) { name = a; }
	void set_des(string a) { description = a; }
	void set_all_fields(vector <Fields> a) { fields = a; }
	void set_all_tasks(vector <Tasks> a) { tasks = a; }
	void add_field(Fields a) { fields.push_back(a); }
	void add_task(Tasks a) { tasks.push_back(a); }

	void clear();

	void print();
	void print_simple();
	void print_fields();
	void print_maint_history();

};