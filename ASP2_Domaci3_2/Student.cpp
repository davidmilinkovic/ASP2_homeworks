#include "Student.h"

int Student::getIndex() const {
    return index;
}

const std::string& Student::getName() const {
    return name;
}

const std::set<std::string>& Student::getCourses() const {
    return courses;
}

Student::~Student() {

}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.index << " : " << s.name << " (";
    bool first = true;
    for (const std::string& x : s.getCourses()) {
        if (!first) os << ", ";
        else first = false;
        os << x;
    }
    os << ")" << std::endl;
    return os;
}

void Student::addCourse(const std::string& course) {
    courses.insert(course);
}

void Student::removeCourse(const std::string& course) {
    courses.erase(course);
}

std::vector<Student*> Student::readFromCsv(const std::string& filePath) {
    std::vector<Student*> v;
    std::ifstream file(filePath);
    if (!file.is_open()) return v;
    std::string line;
    std::getline(file, line);
    while (!file.eof()) {
        std::getline(file, line);
        int comma1 = (int)line.find(',');
        int comma2 = (int)line.find(',', comma1 + 1);
        if (comma1 == -1 || comma2 == -1) continue;

        std::string sIndex = line.substr(0, comma1);
        std::string name = line.substr(comma1 + 1, comma2 - comma1 - 1);
        std::string courses = line.substr(comma2 + 1);

        auto s = new Student(stoi(sIndex), name);
        int lastSpace = -1;
        int space = (int)courses.find(' ');
        while (space != -1) {
            std::string course = courses.substr(lastSpace + 1, space - lastSpace - 1);
            s->addCourse(course);
            lastSpace = space;
            space = (int)courses.find(' ', space + 1);
        }
        if (courses.length()) s->addCourse(courses.substr(lastSpace + 1));
        // std::cout << *s;
        v.push_back(s);
    }
    return v;
}