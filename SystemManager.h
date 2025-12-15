#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Course.h"
#include "Student.h"
using namespace std;

class SystemManager {
private:
    unordered_map<string, Course> courses;  // courseCode -> Course
    unordered_map<string, Student> students; // studentID -> Student

public:
    // Admin functions
    void addCourse();
    void addStudent();
    void listCourses();
    void listStudents();
    void enterPrerequisites();

    // Student functions
    bool studentLogin(string id, string pass);
    void studentMenu(Student &s);
    bool canRegister(Student &s, string courseCode);
    void registerCourse(Student &s);

    void displayCourseGraph();           // Main entry
    void dfsDisplay(const string &code, int depth, unordered_set<string> &visited);  // Helper

    // Utility
    Student& getStudent(string id); // IMPORTANT returning by reference
    void adminMenu();
    void seedData(); // load demo data

    // File handeling
    void saveData();  // Save all data to files
    void loadData();  // Load data from files
    bool isEmpty();
};
