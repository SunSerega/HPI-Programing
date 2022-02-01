#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

class Person {
	string full_name;
	int age;

protected:
	Person(string full_name, int age)
		: full_name{ full_name }
		, age{ age }
	{}

public:
	const string& get_full_name() const { return full_name; }
	int get_age() const { return age; }

public:
	virtual int get_person_id() = 0;
	virtual int ask() = 0;


protected:
	void PrintCore() {
		cout << "Full name: " << full_name << endl;
		cout << "Age: " << age << endl;
	}
public:
	virtual void print() = 0;

};

enum class TeacherDegree {
	None = 0,
	DTS = 1, // Doctor of Technical Sciences
	CTS = 2, // Candidate of Technical Sciences
};
class Teacher : public Person {
	string subject;
	TeacherDegree degree;

	const char* get_degree_string() {
		switch (degree)
		{
#define degree_case_def(val) case TeacherDegree::val: return #val;
			degree_case_def(None);
			degree_case_def(DTS);
			degree_case_def(CTS);
#undef degree_case_def
		default:
			throw degree;
		}
	}

public:
	static const int person_id;
	Teacher(string full_name, int age, string subject, TeacherDegree degree)
		: Person{ full_name, age }
		, subject{ subject }
		, degree{ degree }
	{}

public:
	virtual int get_person_id() override { return person_id; };
	virtual int ask() override { return (int)degree; };

protected:
	void PrintCore() {
		Person::PrintCore();
		cout << "Subject: " << subject << endl;
		cout << "Degree: " << get_degree_string() << endl;
	}
public:
	virtual void print() {
		cout << "Teacher:" << endl;
		Teacher::PrintCore();
	}

};

class DepartmentHead : public Teacher {
	string post;

public:
	static const int person_id;
	DepartmentHead(Teacher&& base, string post)
		: Teacher{ base }
		, post{ post }
	{}

public:
	virtual int get_person_id() override { return person_id; };
	virtual int ask() override { return get_age(); };

public:
	virtual void print() {
		cout << "DepartmentHead:" << endl;
		Teacher::PrintCore();
		cout << "Current post: " << post << endl;
	}

};

class Student : public Person {
	array<int, 5> marks;

public:
	static const int person_id;
	Student(string full_name, int age, array<int, 5>&& marks)
		: Person{ full_name, age }
		, marks{ marks }
	{}

public:
	virtual int get_person_id() override { return person_id; };
	virtual int ask() override { return (int)count(marks.begin(), marks.end(), 2); };

public:
	virtual void print() {
		cout << "Student:" << endl;
		Person::PrintCore();
		cout << "Marks: ";
		copy(marks.begin(), marks.end(), ostream_iterator<int>{cout, " "});
		cout << endl;
	}

};

int person_id_count = 0;
const int Teacher::person_id = ++person_id_count;
const int DepartmentHead::person_id = ++person_id_count;
const int Student::person_id = ++person_id_count;

class University {
	vector<Person*> people{};

public:
	void AddPerson(Person* p) {
		people.push_back(p);
	}

	auto begin() { return people.begin(); }
	auto end() { return people.end(); }

	void print() {
		for (auto p : people)
		{
			p->print();
			cout << "Person of type " << p->get_person_id() << " sais: " << p->ask() << endl;
			cout << string(30, '~') << endl;
		}
	}

};

int main()
{
	University u{};

	u.AddPerson(new Teacher{ "Old Teacher",40,"subj1",TeacherDegree::DTS });
	u.AddPerson(new Teacher{ "Random Teacher",30,"subj2",TeacherDegree::CTS });

	u.AddPerson(new DepartmentHead{ Teacher{"Young Teacher",5,"subj2",TeacherDegree::CTS}, "post1" });

	u.AddPerson(new Student{ "Jack", 18, {2,2,3,4,5} });
	u.AddPerson(new Student{ "Jack2", 18, {5,5,5,5,5} });
	u.AddPerson(new Student{ "Jack3", 18, {2,2,2,2,2} });

	u.print();

	cout << "Teachers under age of 50:" << endl;
	for (auto p : u)
		if (p->get_person_id() == Teacher::person_id && p->get_age() < 50)
			p->print();
	cout << string(30, '~') << endl;

	cout << "Students with marks of 2:" << endl;
	for (auto p : u)
		if (p->get_person_id() == Student::person_id && p->ask() != 0)
			cout << p->get_full_name() << endl;
	cout << string(30, '~') << endl;

};

