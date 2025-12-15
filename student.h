#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Student {
public:
    string name;
    string id;
    string password;
    int academicYear;
    vector<string> finishedCourses;
    vector<string> inProgressCourses;

    Student() {}
    Student(string n, string i, string p, int year)
        : name(n), id(i), password(p), academicYear(year) {}

    void displayStudent() const {
        cout << "Name: " << name << " (ID: " << id << ")\n";

        cout << "Academic Year: " << academicYear << "\n";

        cout << "Finished Courses: ";
        for (auto &c : finishedCourses) cout << c << " ";
        
        cout << "\nCourses in Progress: ";
        for (auto &c : inProgressCourses) cout << c << " ";
        cout << "\n";
    }
};
