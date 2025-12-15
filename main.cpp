#include <iostream>
#include "SystemManager.cpp"
using namespace std;

int main() {
    SystemManager sys;
    sys.loadData(); // Load data from previous run
    if (sys.isEmpty()) sys.seedData(); // Don't wanna keep adding demo data

    int choice;
    while (true) {
        cout << "\n--- Student Prerequisite Management System ---\n";
        cout << "1. Admin Login\n";
        cout << "2. Student Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) sys.adminMenu();
        else if (choice == 2) {
            string id, pass;
            cout << "Enter ID: "; cin >> id;
            cout << "Enter Password: "; cin >> pass;
            if (sys.studentLogin(id, pass)) {
                sys.studentMenu(sys.getStudent(id)); // get student object
            } else {
                cout << "⚠️  Invalid login..\n";
            }
        } 
        else if (choice == 3) {
            sys.saveData(); // Save before exiting
            break;
        }
        else cout << "⚠️  Invalid choice..\n";
    }

    return 0;
}
