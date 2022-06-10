#include "Machines.h"

void Machines::clear() {
	code.clear();
	name.clear();
	description.clear();
	fields.clear();
	tasks.clear();
}

void Machines::print() {
	cout << endl << endl;
	cout << " ~~~~~~~~~~~~~~~MACHINE INFO~~~~~~~~~~~~~\n";
	cout << " CODE       NAME               \n";
	cout << " ------------------------------\n";
	cout << " " << get_code();
	calc_spaces(get_code(), 10);
	cout << get_name();
	calc_spaces(get_name(), 20);
	cout << "\n DESCRIPTION: ";
	cout << " "  << get_des() << endl;
	print_fields();
	print_maint_history();
	cout << endl << endl;
}

void Machines::print_simple() {
	cout << code;
	calc_spaces(code, 10);
	cout << name;
	calc_spaces(name, 20);
	cout << endl;
}

void Machines::print_fields() {
	cout << "\n MAINTENANCE FIELDS:\n";
	cout << " NAME               FREQUENCY  \n";
	cout << " ------------------------------\n";
	for (int i = 0; i < fields.size(); i++) {
		fields[i].print();
	}
	cout << endl;
}

void Machines::print_maint_history() {
	cout << "\n MAINTENANCE HISTORY:\n";
	cout << " TYPE                 DATE       INITIALS   COMMENTS         \n";
	cout << " ------------------------------------------------------------\n";
	if (tasks.size() == 0) {
		cout << " No maintenance history to show for this machine.\n";
	}
	else {
		int end;
		if (tasks.size() < 10)
			end = tasks.size();
		else
			end = 10;
		for (int i = 0; i < end; i++) {
			cout << " ";
			cout << tasks[i].field;
			calc_spaces(tasks[i].field, 20);
			cout << tasks[i].date;
			calc_spaces(tasks[i].date, 10);
			cout << tasks[i].intials;
			calc_spaces(tasks[i].intials, 10);
			cout << tasks[i].comment << endl << endl;
		}
	}
	cout << endl;
}