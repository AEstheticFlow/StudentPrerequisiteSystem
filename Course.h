#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Course {
public:
    string name;
    string code;
    int maxStudents;
    vector<string> prerequisites;
    int hours;

    Course() {}
    Course(string n, string c, int m, vector<string> pre, int h)
        : name(n), code(c), maxStudents(m), prerequisites(pre), hours(h) {}

    void displayCourse() const {
        cout << "Course: " << name << " (" << code << ")\n";

        cout << "Hours: " << hours << "\n";

        cout << "Max Students: " << maxStudents << "\n";
        
        cout << "Prerequisites: ";
        if (prerequisites.empty()) cout << "None";
        else {
            for (auto &p : prerequisites) cout << p << " ";
        }
        cout << "\n";
    }
};
