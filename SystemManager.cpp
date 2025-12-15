#include "SystemManager.h"
#include <fstream> // for file I/O
#include <sstream> // for stringstream
using namespace std;

// ************************************ ADMIN-SIDE ************************************ //
// Initial seeding for easy data experementaion:
void SystemManager::seedData() {
    // Add some sample courses
    courses["CS101"] = Course("Programming Fundamentals", "CS101", 100, {}, 3);
    courses["CS102"] = Course("Data Structures", "CS102", 80, {"CS101"}, 3);
    courses["CS201"] = Course("Algorithms", "CS201", 60, {"CS102"}, 3);
    courses["CS301"] = Course("Artificial Intelligence", "CS301", 50, {"CS201"}, 3);

    // Add sample students
    Student s1("Raafat", "S1001", "pass123", 2);
    s1.finishedCourses = {"CS101"};
    s1.inProgressCourses = {"CS102"};
    students[s1.id] = s1;

    Student s2("Sarah", "S1002", "abc123", 3);
    s2.finishedCourses = {"CS101", "CS102"};
    students[s2.id] = s2;

    cout << "🫵  Sample data loaded successfully!!\n";
}

// Get by reference method:
Student& SystemManager::getStudent(string id) {
    return students[id];
}

// Admin course adding:
void SystemManager::addCourse() {
    string name, code;
    int maxStudents, hours, preCount;
    vector<string> pre;

    cout << "\n--- Add New Course ---\n";
    cout << "Enter Course Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Course Code: ";
    cin >> code;
    cout << "Enter Max Students: ";
    cin >> maxStudents;
    cout << "Enter Credit Hours: ";
    cin >> hours;

    cout << "How many prerequisite courses? ";
    cin >> preCount;
    pre.clear();
    for (int i = 0; i < preCount; i++) {
        string preCode;
        cout << "Enter prerequisite code #" << i + 1 << ": ";
        cin >> preCode;
        pre.push_back(preCode);
    }

    courses[code] = Course(name, code, maxStudents, pre, hours);
    cout << "🫵  Course added successfully!!\n";
}

// Admin student adding:
void SystemManager::addStudent() {
    string name, id, password;
    int year;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Student Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Student ID: ";
    cin >> id;
    cout << "Enter Password: ";
    cin >> password;
    cout << "Enter Academic Year: ";
    cin >> year;

    students[id] = Student(name, id, password, year);
    cout << "🫵  Student added successfully!!\n";
}

// Admin course listing:
void SystemManager::listCourses() {
    cout << "\n--- List of Courses ---\n";
    for (auto &c : courses) {
        c.second.displayCourse(); // second -> Course
        cout << "-----------------------\n";
    }
}

// Admin student listing:
void SystemManager::listStudents() {
    cout << "\n--- List of Students ---\n";
    for (auto &s : students) {
        s.second.displayStudent();  // second -> Student
        cout << "-----------------------\n";
    }
}

// Admin prerequistes setting:
void SystemManager::enterPrerequisites() {
    string code;
    cout << "Enter Course Code to modify prerequisites: ";
    cin >> code;

    if (courses.find(code) == courses.end()) {
        cout << "⚠️ Course not found...\n";
        return;
    }

    int count;
    cout << "How many prerequisites? ";
    cin >> count;
    vector<string> pre;
    for (int i = 0; i < count; i++) {
        string preCode;
        cout << "Enter prerequisite code #" << i + 1 << ": ";
        cin >> preCode;
        pre.push_back(preCode);
    }

    courses[code].prerequisites = pre;
    cout << "🫵  Prerequisites updated successfully!!\n";
}

// Admin user menu:
void SystemManager::adminMenu() {
    string adminPassword;
    cout << "Enter admin password: ";
    cin >> adminPassword;
    if (adminPassword == "Admin123") {
        int choice;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add New Student\n";
            cout << "2. Add New Course\n";
            cout << "3. View All Courses\n";
            cout << "4. View All Students\n";
            cout << "5. Edit Course Prerequisites\n";
            cout << "6. Display Course Graph\n";
            cout << "7. Back to Main Menu\n";

            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1: addStudent(); break;
            case 2: addCourse(); break;
            case 3: listCourses(); break;
            case 4: listStudents(); break;
            case 5: enterPrerequisites(); break;
            case 6: displayCourseGraph(); break;
            case 7: return;
            default: cout << "⚠️  Invalid choice..\n";
            }
        }
    }
    else {
        cout << "⚠️  Invalid login..\n";
    }
}



// ************************************ STUDENT-SIDE ************************************ //
// Student login:
bool SystemManager::studentLogin(string id, string pass) {
    if (students.find(id) == students.end()) {
        cout << "❌  Student ID not found.\n";
        return false;
    }

    if (students[id].password != pass) {
        cout << "❌  Incorrect password.\n";
        return false;
    }

    cout << "🫵  Welcome, " << students[id].name << "!\n";
    return true;
}

// DFS (Depth First Search):
bool SystemManager::canRegister(Student &s, string courseCode) {
    if (courses.find(courseCode) == courses.end()) {
        cout << "❌  Course not found.\n";
        return false;
    }

    // If already taking or finished
    if (find(s.finishedCourses.begin(), s.finishedCourses.end(), courseCode) != s.finishedCourses.end()) {
        cout << "⚠️  You have already completed this course.\n";
        return false;
    }
    if (find(s.inProgressCourses.begin(), s.inProgressCourses.end(), courseCode) != s.inProgressCourses.end()) {
        cout << "⚠️  You are already registered for this course.\n";
        return false;
    }

    // Check all prerequisites recursively
    vector<string> stack = courses[courseCode].prerequisites;
    while (!stack.empty()) {
        string pre = stack.back();
        stack.pop_back();

        // prerequisite not completed?
        if (find(s.finishedCourses.begin(), s.finishedCourses.end(), pre) == s.finishedCourses.end()) {
            cout << "❌  You must complete " << pre << " before taking " << courseCode << ".\n";
            return false;
        }

        // check if the prerequisite has its own prerequisites
        for (auto &p : courses[pre].prerequisites)
            stack.push_back(p);
    }

    return true; // all good
}

// Student registeration:
void SystemManager::registerCourse(Student &s) {
    string code;
    cout << "\nEnter course code to register: ";
    cin >> code;

    if (courses.find(code) == courses.end()) {
        cout << "❌  Course not found.\n";
        return;
    }

    if (!canRegister(s, code)) return;

    if (courses[code].maxStudents <= 0) {
        cout << "❌  Course is full.\n";
        return;
    }

    s.inProgressCourses.push_back(code);
    courses[code].maxStudents--; // one seat taken

    cout << "🫵  You have successfully registered for " << courses[code].name << "!\n";
}

// Student user menu:
void SystemManager::studentMenu(Student &s) {
    int choice;
    while (true) {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View All Courses\n";
        cout << "2. View Course Details\n";
        cout << "3. Register for a Course\n";
        cout << "4. View My Courses\n";
        cout << "5. Edit My Info\n";
        cout << "6. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            listCourses();
            break;
        case 2: {
            string code;
            cout << "Enter Course Code: ";
            cin >> code;
            if (courses.find(code) != courses.end())
                courses[code].displayCourse();
            else
                cout << "❌  Course not found.\n";
            break;
        }
        case 3:
            registerCourse(s);
            break;
        case 4:
            s.displayStudent();
            break;
        case 5: {
            cout << "\nEnter new name (or . to keep current): ";
            string newName;
            cin.ignore();
            getline(cin, newName);
            if (newName != ".") s.name = newName;
            cout << "🫵  Info updated.\n";
            break;
        }
        case 6:
            return;
        default:
            cout << "Invalid choice.\n";
        }
    }
}

// --------------------------------- GRAPH-REPRESNTATION ---------------------------------//
// Graph searching
void SystemManager::displayCourseGraph() {
    cout << "\n🌳 Course Dependency Graph:\n";

    unordered_set<string> visited;
    vector<string> roots;

    // Find courses that have no prerequisites (the roots)
    for (auto &pair : courses) {
        if (pair.second.prerequisites.empty()) {
            roots.push_back(pair.first);
        }
    }

    // Sort alphabetically by course code (so order is consistent)
    sort(roots.begin(), roots.end());

    // Perform DFS for each root
    for (auto &root : roots) {
        dfsDisplay(root, 0, visited);
        cout << endl; // space between different root chains
    }
}

// Graph sorting:
void SystemManager::dfsDisplay(const string &code, int depth, unordered_set<string> &visited) {
    if (visited.count(code)) return;
    visited.insert(code);

    // Indent based on depth
    for (int i = 0; i < depth; ++i) cout << "    ";
    cout << "└── " << courses[code].name << " [" << code << "]\n";


    for (auto &next : courses) {
        for (auto &pre : next.second.prerequisites) {
            if (pre == code) {
                dfsDisplay(next.first, depth + 1, visited);
            }
        }
    }
}



// ************************************ FILE-SAVING ************************************ //
// Saving data:
void SystemManager::saveData() {
    ofstream courseFile("courses.txt");
    ofstream studentFile("students.txt");

    if (!courseFile || !studentFile) {
        cout << "⚠️ Error saving files.\n";
        return;
    }

    // --- Save Courses ---
    for (auto &pair : courses) {
        Course c = pair.second;
        courseFile << c.code << "|" << c.name << "|" << c.maxStudents << "|" << c.hours << "|";
        for (auto &p : c.prerequisites) courseFile << p << ",";
        courseFile << "\n";
    }

    // --- Save Students ---
    for (auto &pair : students) {
        Student s = pair.second;
        studentFile << s.id << "|" << s.name << "|" << s.password << "|" << s.academicYear << "|";
        for (auto &f : s.finishedCourses) studentFile << f << ",";
        studentFile << "|";
        for (auto &ip : s.inProgressCourses) studentFile << ip << ",";
        studentFile << "\n";
    }

    courseFile.close();
    studentFile.close();

    cout << "💾 Data saved successfully!\n";
}

// Loading Data:
void SystemManager::loadData() {
    ifstream courseFile("courses.txt");
    ifstream studentFile("students.txt");

    if (!courseFile || !studentFile) {
        cout << "⚠️  No previous data found. Starting fresh.\n";
        return;
    }

    // --- Load Courses ---
    courses.clear();
    string line;
    while (getline(courseFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string code, name, prePart, temp;
        int maxStudents, hours;

        getline(ss, code, '|');
        getline(ss, name, '|');
        ss >> maxStudents;
        ss.ignore(1, '|');
        ss >> hours;
        ss.ignore(1, '|');
        getline(ss, prePart);

        vector<string> pre;
        stringstream preSS(prePart);
        while (getline(preSS, temp, ',')) {
            if (!temp.empty()) pre.push_back(temp);
        }

        courses[code] = Course(name, code, maxStudents, pre, hours);
    }

    // --- Load Students ---
    students.clear();
    while (getline(studentFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, password, finishedPart, inProgressPart, temp;
        int year;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, password, '|');
        ss >> year;
        ss.ignore(1, '|');
        getline(ss, finishedPart, '|');
        getline(ss, inProgressPart);

        Student s(name, id, password, year);

        stringstream fs(finishedPart);
        while (getline(fs, temp, ',')) {
            if (!temp.empty()) s.finishedCourses.push_back(temp);
        }

        stringstream ips(inProgressPart);
        while (getline(ips, temp, ',')) {
            if (!temp.empty()) s.inProgressCourses.push_back(temp);
        }

        students[id] = s;
    }

    courseFile.close();
    studentFile.close();

    cout << "🫵  Data loaded successfully.\n";
}

// Checking empty file
bool SystemManager::isEmpty() {
    return students.empty() && courses.empty();
}