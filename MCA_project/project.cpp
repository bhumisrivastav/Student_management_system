// project.cpp 
#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream> 
#include <limits> 
#include <algorithm> 
#include <cctype> 
#include <cstdlib> 
using namespace std; 
// ---------------- Color Functions ---------------- 
void Red()    { cout << "\033[1;31m"; } 
void Yellow() { cout << "\033[1;33m"; } 
void Green()  { cout << "\033[0;32m"; } 
void Purple() { cout << "\033[0;35m"; } 
void Reset()  { cout << "\033[0m"; } 
// Clear cin buffer 
11 
void clearInput() { 
cin.clear(); 
cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
} 
// ---------------- Student Class ---------------- 
class Student { 
private: 
string name; 
string rollNo;   // stored as string of digits (exactly 3) 
string branch; 
string course; 
string emailId; 
public: 
Student() {} 
Student(const string &n, const string &r, const string &b, const string &c, const string &e) 
: name(n), rollNo(r), branch(b), course(c), emailId(e) {} 
// Getters 
string getName() const    { return name; } 
string getRollNo() const  { return rollNo; } 
string getBranch() const  { return branch; } 
string getCourse() const  { return course; } 
12 
string getEmailId() const { return emailId; } 
// Setters 
void setName(const string &n)    { name = n; } 
void setRollNo(const string &r)  { rollNo = r; } 
void setBranch(const string &b)  { branch = b; } 
void setCourse(const string &c)  { course = c; } 
void setEmailId(const string &e) { emailId = e; } 
void printStudent() const { 
Purple(); 
cout << "\nStudent Name    : " << name << endl; 
cout << "Student RollNo  : " << rollNo << endl; 
cout << "Student Branch  : " << branch << endl; 
cout << "Student Course  : " << course << endl; 
cout << "Student EmailId : " << emailId << endl; 
cout << "-----------------------------------------\n"; 
Reset(); 
} 
}; 
// ---------------- Validation & Helper Functions ---------------- 
// Only alphabets and spaces, max length 
13 
bool isAlphaSpace(const string &s, int maxLen) { 
if (s.empty() || (int)s.length() > maxLen) return false; 
for (char c : s) 
if (!isalpha(static_cast<unsigned char>(c)) && c != ' ') 
return false; 
return true; 
} 
// Exactly fixedLen numeric digits (used for 3-digit roll) 
bool isNumericFixed(const string &s, int fixedLen) { 
if ((int)s.length() != fixedLen) return false; 
for (char c : s) 
if (!isdigit(static_cast<unsigned char>(c))) 
return false; 
return true; 
} 
// Basic email check: contains '@' and length <= maxLen 
bool isValidEmail(const string &s, int maxLen) { 
if (s.empty() || (int)s.length() > maxLen) return false; 
size_t at = s.find('@'); 
if (at == string::npos) return false; 
// simple: ensure something before and after '@' 
14 
if (at == 0 || at == s.length() - 1) return false; 
return true; 
} 
// Unique checks (Student fully defined above) 
bool isRollUnique(const vector<Student> &v, const string &roll) { 
for (const auto &s : v) 
if (s.getRollNo() == roll) 
return false; 
return true; 
} 
bool isEmailUnique(const vector<Student> &v, const string &email) { 
for (const auto &s : v) 
if (s.getEmailId() == email) 
return false; 
return true; 
} 
// ---------------- Student Management System ---------------- 
class StudentManagementSystem { 
private: 
vector<Student> students; 
15 
void readFromFile(); 
void writeToFile(); 
public: 
void menu(); 
void addStudent(); 
void displayStudents(); 
void searchStudent(); 
void editStudent(); 
void deleteStudent(); 
void feePayment(); 
}; 
// Read File 
void StudentManagementSystem::readFromFile() { 
ifstream fin("Student.txt"); 
if (!fin.is_open()) return; 
string name, roll, branch, course, email; 
while (getline(fin, name) && 
getline(fin, roll) && 
getline(fin, branch) && 
getline(fin, course) && 
16 
getline(fin, email)) 
{ 
students.push_back(Student(name, roll, branch, course, email)); 
} 
fin.close(); 
} 
// Write File 
void StudentManagementSystem::writeToFile() { 
ofstream fout("Student.txt"); 
if (!fout.is_open()) { 
Red(); cout << "Error saving file!\n"; Reset(); 
return; 
} 
for (const auto& stu : students) { 
fout << stu.getName() << "\n"; 
fout << stu.getRollNo() << "\n"; 
fout << stu.getBranch() << "\n"; 
fout << stu.getCourse() << "\n"; 
fout << stu.getEmailId() << "\n"; 
} 
fout.close(); 
} 
17 
// Menu 
void StudentManagementSystem::menu() { 
readFromFile(); 
int choice; 
while (true) { 
Yellow(); 
cout << "\n----------- STUDENT MANAGEMENT SYSTEM -----------\n"; 
cout << "1. Add Student\n"; 
cout << "2. Display All Students\n"; 
cout << "3. Search Student by Roll No\n"; 
cout << "4. Edit Student\n"; 
cout << "5. Delete Student\n"; 
cout << "6. Fee Payment (Email OTP)\n"; 
cout << "7. Exit & Save\n"; 
cout << "--------------------------------------------------\n"; 
cout << "Enter your choice: "; 
Reset(); 
cin >> choice; 
clearInput(); 
switch (choice) { 
18 
case 1: addStudent(); break; 
case 2: displayStudents(); break; 
case 3: searchStudent(); break; 
case 4: editStudent(); break; 
case 5: deleteStudent(); break; 
case 6: feePayment(); break; 
case 7: 
writeToFile(); 
Green(); 
cout << "Data saved successfully! Exiting...\n"; 
Reset(); 
return; 
default: 
Red(); 
cout << "Invalid choice! Try again.\n"; 
Reset(); 
} 
} 
} 
// Add Student (with constraints) 
void StudentManagementSystem::addStudent() { 
string name, roll, branch, course, email; 
19 
// NAME: alphabets + spaces, max 20 
do { 
cout << "Enter Student Name : "; 
getline(cin, name); 
if (!isAlphaSpace(name, 20)) { 
Red(); cout << "Invalid Name! Only letters & spaces (max 20).\n"; Reset(); 
} 
} while (!isAlphaSpace(name, 20)); 
// ROLL: exactly 3 digits, unique, not null 
do { 
cout << "Enter Roll No : "; 
getline(cin, roll); 
if (!isNumericFixed(roll, 3)) { 
Red(); cout << "Roll must be exactly 3 digits.\n"; Reset(); 
continue; 
} 
if (!isRollUnique(students, roll)) { 
Red(); cout << "Roll number already exists!\n"; Reset(); 
} 
} while (!isNumericFixed(roll, 3) || !isRollUnique(students, roll)); 
// BRANCH: alphabets + spaces, max 20 
20 
do { 
cout << "Enter Branch : "; 
getline(cin, branch); 
if (!isAlphaSpace(branch, 20)) { 
Red(); cout << "Invalid Branch! Only letters & spaces (max 20).\n"; Reset(); 
} 
} while (!isAlphaSpace(branch, 20)); 
// COURSE: alphabets + spaces, max 20 
do { 
cout << "Enter Course : "; 
getline(cin, course); 
if (!isAlphaSpace(course, 20)) { 
Red(); cout << "Invalid Course! Only letters & spaces (max 20).\n"; Reset(); 
} 
} while (!isAlphaSpace(course, 20)); 
// EMAIL: contains '@', max 30, unique 
do { 
cout << "Enter Email : "; 
getline(cin, email); 
if (!isValidEmail(email, 30)) { 
Red(); cout << "Invalid Email format!\n"; Reset(); 
21 
} 
continue; 
if (!isEmailUnique(students, email)) { 
Red(); cout << "Email already exists!\n"; Reset(); 
} 
} while (!isValidEmail(email, 30) || !isEmailUnique(students, email)); 
students.push_back(Student(name, roll, branch, course, email)); 
Green(); 
cout << "Student Added Successfully!\n"; 
Reset(); 
} 
// Display Students 
void StudentManagementSystem::displayStudents() { 
if (students.empty()) { 
Red(); cout << "No student records found.\n"; Reset(); 
return; 
} 
int count = 1; 
for (const auto& stu : students) { 
cout << "\nRecord No : " << count++ << endl; 
22 
} 
stu.printStudent(); 
} 
// Search Student 
void StudentManagementSystem::searchStudent() { 
string roll; 
cout << "Enter Roll No to search: "; 
getline(cin, roll); 
for (const auto& stu : students) { 
if (stu.getRollNo() == roll) { 
stu.printStudent(); 
return; 
} 
} 
Red(); cout << "Student not found!\n"; Reset(); 
} 
// Edit Student (with validation) 
void StudentManagementSystem::editStudent() { 
string roll; 
cout << "Enter Roll No to edit: "; 
23 
getline(cin, roll); 
for (auto& stu : students) { 
if (stu.getRollNo() == roll) { 
string name, branch, course, email; 
// NAME 
do { 
cout << "\nEnter New Name : "; 
getline(cin, name); 
if (!isAlphaSpace(name, 20)) { 
Red(); cout << "Invalid Name!\n"; Reset(); 
} 
} while (!isAlphaSpace(name, 20)); 
// BRANCH 
do { 
cout << "Enter New Branch : "; 
getline(cin, branch); 
if (!isAlphaSpace(branch, 20)) { 
Red(); cout << "Invalid Branch!\n"; Reset(); 
} 
} while (!isAlphaSpace(branch, 20)); 
// COURSE 
24 
do { 
cout << "Enter New Course : "; 
getline(cin, course); 
if (!isAlphaSpace(course, 20)) { 
Red(); cout << "Invalid Course!\n"; Reset(); 
} 
} while (!isAlphaSpace(course, 20)); 
// EMAIL (must be unique except current student's own) 
do { 
cout << "Enter New Email : "; 
getline(cin, email); 
if (!isValidEmail(email, 30)) { 
Red(); cout << "Invalid Email!\n"; Reset(); 
continue; 
} 
if (email != stu.getEmailId() && !isEmailUnique(students, email)) { 
Red(); cout << "Email already exists!\n"; Reset(); 
} 
} while (!isValidEmail(email, 30) || 
(email != stu.getEmailId() && !isEmailUnique(students, email))); 
// Apply changes 
25 
stu.setName(name); 
stu.setBranch(branch); 
stu.setCourse(course); 
stu.setEmailId(email); 
Green(); cout << "Record Updated Successfully!\n"; Reset(); 
return; 
} 
} 
Red(); cout << "Student not found!\n"; Reset(); 
} 
// Delete Student 
void StudentManagementSystem::deleteStudent() { 
string roll; 
cout << "Enter Roll No to delete: "; 
getline(cin, roll); 
for (auto it = students.begin(); it != students.end(); ++it) { 
if (it->getRollNo() == roll) { 
students.erase(it); 
Green(); cout << "Student Deleted Successfully!\n"; Reset(); 
return; 
26 
} 
} 
Red(); cout << "Student not found!\n"; Reset(); 
} 
// Fee Payment (calls Python Gmail OTP script) 
void StudentManagementSystem::feePayment() { 
string roll; 
cout << "Enter Roll No for fee payment: "; 
getline(cin, roll); 
for (auto &stu : students) { 
if (stu.getRollNo() == roll) { 
cout << "\nStudent Found:\n"; 
stu.printStudent(); 
string amount; 
cout << "Enter Fee Amount to Pay (Rs.): "; 
getline(cin, amount); 
// Temporary output file 
string tmpFile = "otp_output.txt"; 
27 
// Build command to call Python script (fee_payment.py sends OTP using Gmail app password) 
string command = "python fee_payment.py \"" + 
stu.getEmailId() + "\" \"" + 
stu.getName() + "\" \"" + 
stu.getRollNo() + "\" \"" + 
amount + "\" > " + tmpFile; 
// Execute command 
int ret = system(command.c_str()); 
(void)ret; // ignore return code 
// Read OTP from temporary file 
ifstream fin(tmpFile); 
string sentOtp; 
if (!fin.is_open()) { 
Red(); 
cout << "OTP sending failed (could not read output file).\n"; 
Reset(); 
return; 
} 
getline(fin, sentOtp); 
fin.close(); 
28 
// remove temp file 
remove(tmpFile.c_str()); 
// Trim newline and whitespace 
while (!sentOtp.empty() && (sentOtp.back() == '\n' || sentOtp.back() == '\r' || sentOtp.back() == ' ')) 
sentOtp.pop_back(); 
if (sentOtp.empty() || sentOtp.rfind("ERROR", 0) == 0) { 
Red(); 
cout << "OTP sending failed.\n"; 
Reset(); 
return; 
} 
string userOtp; 
cout << "Enter OTP sent to your email: "; 
getline(cin, userOtp); 
if (userOtp == sentOtp) { 
Green(); 
cout << "Payment of Rs." << amount << " successful!\n"; 
Reset(); 
} else { 
29 
Red(); 
cout << "Invalid OTP! Payment failed.\n"; 
Reset(); 
} 
return; 
} 
} 
Red(); 
cout << "Student not found!\n"; 
Reset(); 
} 
// MAIN 
int main() { 
StudentManagementSystem sms; 
sms.menu(); 
return 0; 
} 
