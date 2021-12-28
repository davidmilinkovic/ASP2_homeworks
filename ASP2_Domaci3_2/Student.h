#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>

class Student {
    int index;
    std::string name;
    std::set<std::string> courses;
public:
    Student(int _index, std::string _name) : index(_index), name(_name) {};
    Student(const Student& s) = delete;
    Student(Student&& s) = delete;
    ~Student();

    const std::string& getName() const;
    const std::set<std::string>& getCourses() const;
    int getIndex() const;
    friend std::ostream& operator<<(std::ostream& os, const Student& s);
    void addCourse(const std::string& course);
    void removeCourse(const std::string& course);
    static std::vector<Student*> readFromCsv(const std::string& filePath);
};