#pragma once

#include <iostream>
#include <memory>
#include <shared_mutex>
#include <mutex>
using namespace std;

namespace buzzdb{
namespace utils{

// A class in C++ is a user-defined type that has data and functions 
// (also called member variables and member functions)
// The member variables are typically of built-in data types like integers
class Course{
public:
    int _course_id;
    std::string _course_name;

    // Constructor is used to create a "Course" object 
    // Default constructor
    Course();

    // There can be multiple constructors
    Course(int course_id, std::string course_name);

    // Copy constructor
	Course(const Course& course_being_copied);

    // Overloading << operator for pretty printing objects of this class
    friend std::ostream& operator<<(std::ostream& os, const Course& s);  

    // Overloading the comparison operator for objects of this class
    friend bool operator==(const Course& course1, const Course& course2);

    // Overloading the assignment operator
	Course& operator= (const Course& course_being_copied);
};

class Student{
public:
    int _student_id;
    std::string _student_name;
    Course* _courses;

    // Each student can only be enrolled in three courses
    static const int _max_number_of_courses = 3;
    int _current_number_of_courses;

    Student(int student_id, std::string student_name);

    // Destructor is responsible for releasing resources
    // like allocated memory
    ~Student();

    // function for adding a course
    // returns true or false
    bool add_course(const Course& course);

    // function for dropping a course
    // returns true or false
    bool drop_course(const Course& course);

    friend std::ostream& operator<<(std::ostream& os, const Student& s);
};

// Inheritance: Deriving new classes (sub classes) from existing ones such as super class or base class and then forming them into a hierarchy of classes
class UndergradStudent: public Student{
public:
    int _year_in_program;

    UndergradStudent(int student_id, std::string student_name, int year_in_program);

    friend std::ostream& operator<<(std::ostream& os, const UndergradStudent& s);
};

class GraduateStudent: public Student {
public:
    int _advisor_id;

    GraduateStudent(int student_id, std::string student_name, 
        int advisor_id);

    friend std::ostream& operator<<(std::ostream& os, const GraduateStudent& s);
};

// A class used to illustrate a reader-writer latch
class RWLatch {
private:
    size_t count;
    size_t seq;
    mutable shared_mutex mutex_;
public:
    RWLatch();
    ~RWLatch();
    size_t get() const;
    size_t increment();
};

}  
} 
