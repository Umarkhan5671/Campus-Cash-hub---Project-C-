#include <iostream>
#include <fstream>
#include <sstream> //for stringstream
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <windows.h>
#include <cctype>  // for isdigit check whether a character is a decimal digit (0-9)
#include <iomanip>


using namespace std;



void clearScreen() {
    system("CLS");
}

int strIn(const string& str) {
    stringstream ss(str);
  int num;
    ss >> num;  //The operator >> used in ss >> num; is called the extraction operator in C++.
	           // It is commonly used with input streams like cin, and in this context, it is used with a stringstream.
    return num;
}



bool dateCheck(const string& date) {
    if (date.size() != 10) return false;   //size() is a member function of the std::string class
    for (int i = 0; i < 10; ++i) {			// use for find the length of string
        if (i == 2 || i == 5) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false; //
        }
    }
    return true;
}

bool contNo(const string& contactNo) {
    if (contactNo.size() != 11) return false;
    for (int i = 0; i < contactNo.size(); ++i) {
        if (!isdigit(contactNo[i])) return false;
    }
    return true;
}


bool cnicv(const string& cnic) {
    if (cnic.size() != 13) return false;
    for (int i = 0; i < cnic.size(); ++i) {
        if (!isdigit(cnic[i])) return false;
    }
    return true;
}

bool cnicnu(const string& Cnic) {
    ifstream infile("students.txt");
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
        getline(ss, roll, ',');
        getline(ss, name, ',');
        getline(ss, className, ',');
        getline(ss, faculty, ',');
        getline(ss, contactNo, ',');
        getline(ss, cnic, ',');
        getline(ss, gender, ',');
        getline(ss, email, ',');
        getline(ss, dateOfAdmission, ',');
        getline(ss, session, ',');
        getline(ss, feeStatus, ',');
        if (Cnic == cnic) {
            infile.close();
            return false;
        }
    }
    infile.close();
    return true;
}

// Base Class for User
class User {
public:
    virtual bool login() = 0;  // Abstract method
};

// Derived Class for Admin User
class Admin : public User {
public:
    bool login() override {
        string em, pass;
        cout <<"\t\t\t\t\t\t "<<"WELLCOME TO CAMPUS CASH HUB"<<endl;
        cout << "\n\n\t\t\t\t\tEnter Email: ";
        cin >> em;
        cout << "\n\t\t\t\t\tEnter  Password: ";
        cin >> pass;
        if (em == "c" && pass == "p" ) {
            return true;
        } else {
            cout << "\n\t\t\t\t\tInvalid E-mail or Password!!! Try again." << endl;
            return false;
        }
    }
};

class FeeManagement {
private:
    static map<string, int> feeStr;
    static set<int> drn;

public:
    static void loadFeeInfo() ;


    static void saveFeeInfo() ;

    static int getClassFee(const string& className) ;

    static void viewFee() ;


    static void updateFee() ;


    static void submitFees() ;


    static void viewFeeRecord();


    static void viewPendingFees() ;


    static void addDeletedRollNo(int rollNo) ;


    static int getnrn()
	 {
        if (!drn.empty()) {
            int rollNo = *drn.begin();
            drn.erase(drn.begin());
            return rollNo;
        }
        return 0;
    }
};

map<string, int> FeeManagement::feeStr;
set<int> FeeManagement::drn;

 void FeeManagement::loadFeeInfo() {
        ifstream infile("fees_structure.txt");
        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            string className;
            int fee;
            ss >> className >> fee;
            feeStr[className] = fee;
        }
        infile.close();
    }
    
   void FeeManagement::saveFeeInfo() {
        ofstream outfile("fees_structure.txt");
        for (map<string, int>::iterator it = feeStr.begin(); it != feeStr.end(); ++it) {
            outfile << it->first << " " << it->second << endl;
        }
        outfile.close();
    }

    int FeeManagement:: getClassFee(const string& className) {
        return feeStr[className];
    }

    void FeeManagement::viewFee() {
        clearScreen();
        for (map<string, int>::iterator it = feeStr.begin(); it != feeStr.end(); ++it) {
            cout << "Class " << it->first << ": " << it->second << endl;
        }
    }

   void FeeManagement::updateFee() {
        clearScreen();
        string className;
        int newFee;
        cout << "Enter Class to update fee: ";
        cin >> className;
        cout << "Enter New Fee: ";
        cin >> newFee;
        feeStr[className] = newFee;
        saveFeeInfo() ;
        cout << "Fee for class " << className << " updated to " << newFee << endl;
    }

    void FeeManagement::submitFees() {
        clearScreen();
        int rollNo;
        cout << "Enter Roll No for fee submission: ";
        cin >> rollNo;
        ifstream infile("students.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
           getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');

            if (strIn(roll) == rollNo && feeStatus == "Pending") {
                found = true;
                feeStatus = "Paid";
                cout << "Fee submitted for student: " << name << ", Class: " << className << ", Fee: " << FeeManagement::getClassFee(className) << endl;
            }
            tempFile << roll << "," << name << "," << className << "," << faculty << "," << contactNo << "," << cnic << "," << gender << "," << email << "," << dateOfAdmission << "," << session << "," << feeStatus << "\n";
        }
        infile.close();
        tempFile.close();
        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (found) {
            cout << "Fee Submitted Successfully." << endl;
        } else {
            cout << "Fee Already Paid or Student Not Found." << endl;
        }
    }

    void FeeManagement:: viewFeeRecord() {
        clearScreen();
        ifstream infile("students.txt");
        string line;
        cout << "Roll No\t\tName\t\tClass\t\tFaculty\t\tContact\t\tDate of Admission\tSession\t\tFee Status" << endl;
        cout << "<<-------------------------------------------------------------------------------------------------------------------------------------->>" << endl;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
           getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');
            cout <<"   "<< roll << "  \t\t" << name << "\t" <<" "<< className <<"\t\t" <<" "<<faculty <<"\t\t" << contactNo << " \t" << dateOfAdmission <<"\t\t " << session <<"\t\t" << feeStatus << endl;
        }
        infile.close();
    }

   void FeeManagement::viewPendingFees() {
        clearScreen();
        ifstream infile("students.txt");
        string line;
        cout << "Roll No\t\tName\t\tClass\t\tFaculty\t\tContact\t\tDate of Admission\tSession\t\tFee Status" << endl;
        cout << "<<-------------------------------------------------------------------------------------------------------------------------------------->>" << endl;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
            getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');
            if (feeStatus == "Pending") {
                cout <<"   "<< roll << "  \t\t" << name << "\t" <<" "<< className <<"\t\t" <<" "<<faculty <<"\t\t" << contactNo << " \t" << dateOfAdmission <<"\t\t " << session <<"\t\t" << feeStatus << endl;
            }
        }
        infile.close();
    }

    void FeeManagement:: addDeletedRollNo(int rollNo) {
        drn.insert(rollNo);
    }

  



class Student {
private:
    int rollNo;
    string name;
    string className;
    string faculty;
    string contactNo;
//    string dateOfBirth;
string cnic;
    string gender;
    string email;
    string dateOfAdmission;
    string session;
    string feeStatus;

public:
    Student(int r, const string& n, const string& c, const string& f, const string& con, const string& ci, const string& g, const string& e, const string& doa, const string& s, const string& fs)
        : rollNo(r), name(n), className(c), faculty(f), contactNo(con), cnic(ci), gender(g), email(e), dateOfAdmission(doa), session(s), feeStatus(fs) {}

    static int GRN() {
        int rollNo = FeeManagement::getnrn();
        if (rollNo > 0) {
            return rollNo;
        }

        ifstream infile("students.txt");
        string line;
        int lastRollNo = 0;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll;
            getline(ss, roll, ',');
            lastRollNo = strIn(roll);
        }
        infile.close();
        return lastRollNo + 1;
    }

    static void addStudent() {
        clearScreen();
        string name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session;
        cout<<"\t\t\t<< == Welcome to Registration Desk == >>";
        cout << "\n\n\t\t\tEnter Candidate's Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "\t\t\tEnter Class: ";
        getline(cin, className);
        cout << "\t\t\tEnter Faculty: ";
        getline(cin, faculty);
        cout << "\t\t\tEnter Contact No (03*********): ";
        getline(cin, contactNo);
        while (!contNo(contactNo)) {
            cout << "\t\t\tInvalid Format! Please re-enter Contact No (03*********): ";
            getline(cin, contactNo);
        }
        cout << "\t\t\tEnter CNIC No. (1***********0): ";
        getline(cin, cnic);
        while (!cnicv( cnic)) {
            cout << "\t\t\tInvalid Format! Please Re-Enter CNIC No (1***********0):  ";
            getline(cin, cnic);
        }
        if (!cnicnu(cnic)) {
            cout << "\n\t\t\t"<<"A Student Already Registered on this CNIC NO" << endl;
            return;
        }
        
        cout << "\t\t\tEnter Gender: ";
        getline(cin, gender);
        cout << "\t\t\tEnter Email: ";
        getline(cin, email);
        cout << "\t\t\tEnter Date of Registration (dd-mm-yyyy): ";
        getline(cin, dateOfAdmission);
        while (!dateCheck(dateOfAdmission)) {
            cout << "\t\t\tInvalid Date Format! Please re-enter (dd-mm-yyyy): ";
            getline(cin, dateOfAdmission);
        }
        cout << "\t\t\tEnter Session: ";
        getline(cin, session);
        string feeStatus = "Pending";

        int rollNo = GRN();
        Student student(rollNo, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus);
        ofstream outfile("students.txt", ios::app);
        outfile << rollNo << "," << name << "," << className << "," << faculty << "," << contactNo << "," << cnic << "," << gender << "," << email << "," << dateOfAdmission << "," << session << "," << feeStatus << "\n";
        outfile.close();

        cout << "\t\t\t\nCONGRATULATIONS!!!\nStudent added successfully. Roll No: " << rollNo << endl;
    }

    static void deleteStudent() {
        clearScreen();
        int rollNo;
        cout << "Enter Roll No to delete: ";
        cin >> rollNo;
        ifstream infile("students.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
            getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');

            if (strIn(roll) == rollNo) {
                found = true;
                cout << "Student " << name << " with Roll No " << rollNo << " has been deleted." << endl;
                FeeManagement::addDeletedRollNo(rollNo);
            } else {
                tempFile << roll << "," << name << "," << className << "," << faculty << "," << contactNo << "," << cnic << "," << gender << "," << email << "," << dateOfAdmission << "," << session << "," << feeStatus << "\n";
            }
        }
        infile.close();
        tempFile.close();
        remove("students.txt");
        rename("temp.txt", "students.txt");

        if (!found) {
            cout << "Student with Roll No " << rollNo << " not found." << endl;
        }
    }

   static void editStudent() {
    clearScreen();
    int rollNo;
    cout << "Enter Roll No to edit: ";
    cin >> rollNo;
    ifstream infile("students.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;
    while (getline(infile, line)) {
        stringstream ss(line);
        string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
        getline(ss, roll, ',');
        getline(ss, name, ',');
        getline(ss, className, ',');
        getline(ss, faculty, ',');
        getline(ss, contactNo, ',');
        getline(ss, cnic, ',');
        getline(ss, gender, ',');
        getline(ss, email, ',');
        getline(ss, dateOfAdmission, ',');
        getline(ss, session, ',');
        getline(ss, feeStatus, ',');

        if (strIn(roll) == rollNo) {
            found = true;
            cout << "Enter new details for Roll No " << rollNo << " (leave blank to keep current value):\n";
            cout << "Enter Name (" << name << "): ";
            cin.ignore();
            string newName;
            getline(cin, newName);
            if (newName != "") name = newName;

            string originalClass = className; // Store the original class

            cout << "Enter Class (" << className << "): ";
            string newClass;
            getline(cin, newClass);
            if (newClass != "") className = newClass;

            cout << "Enter Faculty (" << faculty << "): ";
            string newFaculty;
            getline(cin, newFaculty);
            if (newFaculty != "") faculty = newFaculty;

            cout << "Enter Contact No (" << contactNo << "): ";
            string newContact;
            getline(cin, newContact);
            if (newContact != "") contactNo = newContact;



            cout << "Enter Gender (" << gender << "): ";
            string newGender;
            getline(cin, newGender);
            if (newGender != "") gender = newGender;

            cout << "Enter Email (" << email << "): ";
            string newEmail;
            getline(cin, newEmail);
            if (newEmail != "") email = newEmail;

            cout << "Enter Date of Admission (" << dateOfAdmission << "): ";
            string newDOA;
            getline(cin, newDOA);
            if (newDOA != "") dateOfAdmission = newDOA;

            cout << "Enter Session (" << session << "): ";
            string newSession;
            getline(cin, newSession);
            if (newSession != "") session = newSession;

            // Check if the class has been changed
            if (className != originalClass) {
                feeStatus = "Pending"; // Update the fee status to "Pending"
            }

            cout << "Enter Fee Status (" << feeStatus << "): ";
            string newFeeStatus;
            getline(cin, newFeeStatus);
            if (newFeeStatus != "") feeStatus = newFeeStatus;

            cout << "Student record updated." << endl;
        }

        tempFile << roll << "," << name << "," << className << "," << faculty << "," << contactNo << "," << cnic << "," << gender << "," << email << "," << dateOfAdmission << "," << session << "," << feeStatus << "\n";
    }
    infile.close();
    tempFile.close();
    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (!found) {
        cout << "Student with Roll No " << rollNo << " not found." << endl;
    }
}


    static void viewStudents() {
        clearScreen();
        ifstream infile("students.txt");
        string line;
        cout << " Roll No   Name\t\tClass\t  Faculty     Contact\t\tCNIC NO\t\tGender\t  Email\t Date of Admission   Session\t  Fee Status" << endl;
        cout << "<<----------------------------------------------------------------------------------------------------------------------------------------->>" << endl;
        while (getline(infile, line)) {
            stringstream ss(line);
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
            getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');
            cout <<"   "<< roll << "\t " << name << "\t  " << className <<"\t    " << faculty << "\t    " << contactNo << "\t    " << cnic << "\t " << gender << "\t  " << email << "\t   " << dateOfAdmission << "\t\t" << session << "\t   " << feeStatus << endl;
        }
        infile.close();
    }

    static void searchStudent() {
        clearScreen();
        int rollNo;
        cout << "Enter Roll No to search: ";
        cin >> rollNo;
        ifstream infile("students.txt");
        string line;
        bool found = false;
        while (getline(infile, line)) {
            stringstream ss(line); // convert string to int
            string roll, name, className, faculty, contactNo, cnic, gender, email, dateOfAdmission, session, feeStatus;
            getline(ss, roll, ',');
            getline(ss, name, ',');
            getline(ss, className, ',');
            getline(ss, faculty, ',');
            getline(ss, contactNo, ',');
            getline(ss, cnic, ',');
            getline(ss, gender, ',');
            getline(ss, email, ',');
            getline(ss, dateOfAdmission, ',');
            getline(ss, session, ',');
            getline(ss, feeStatus, ',');

            if (strIn(roll) == rollNo) {
                found = true;
                cout << "Roll No: " << roll << endl;
                cout << "Name: " << name << endl;
                cout << "Class: " << className << endl;
                cout << "Faculty: " << faculty << endl;
                cout << "Contact No: " << contactNo << endl;
                cout << "CNIC NO: " << cnic << endl;
                cout << "Gender: " << gender << endl;
                cout << "Email: " << email << endl;
                cout << "Date of Admission: " << dateOfAdmission << endl;
                cout << "Session: " << session << endl;
                cout << "Fee Status: " << feeStatus << endl;
                break;
            }
        }
        infile.close();
        if (!found) {
            cout << "Student with Roll No " << rollNo << " not found." << endl;
        }
    }
};

void mainMenu() {
    int choice = 0;
    do {
        clearScreen();
        cout << "\t\t\t\t<< == DASHBOARD == >>\n";
        cout << "\n\t\t\tPress : 1 : Student Management\n";
        cout << "\t\t\tPress : 2 : Fees Submission\n";
        cout << "\t\t\tPress : 3 : View Fees Structure\n";
        cout << "\t\t\tPress : 4 : View Students List\n";
        cout << "\t\t\tPress : 5 : Search Any Student\n";
        cout << "\n\t\t\tPress : 6 : Logout\n";
        cout << "\n\n\t\t\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                int smChoice;
                do {
                    clearScreen();
                    cout << "\t\t\t<< == Student Management == >>\n";
                    cout << "\n\t\t\tPress : 1 : Register a New Student\n";
                    cout << "\t\t\tPress : 2 : Remove a Student\n";
                    cout << "\t\t\tPress : 3 : Update a Student\n";
                    cout << "\n\t\t\tPress : 4 : Return to Home Page\n";
                    cout << "\n\t\t\tEnter your choice: ";
                    cin >> smChoice;
                    switch (smChoice) {
                        case 1:
                            Student::addStudent();
                            break;
                        case 2:
                            Student::deleteStudent();
                            break;
                        case 3:
                            Student::editStudent();
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Invalid choice! Try again.\n";
                            break;
                    }
                    if (smChoice != 4) {
                        cout << "Press any key to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (smChoice != 4);
                break;
            case 2:
                int fmChoice;
                do {
                    clearScreen();
                    cout << "\t\t\t<< == Fee Transaction == >>\n";
cout << "\n\t\t\tPress : 1 : Submit Fees\n";
cout << "\t\t\tPress : 2 : View Record of Each Student\n";
cout << "\t\t\tPress : 3 : View Pending Students\n";
cout << "\t\t\tPress : 4 : View Fees Record of Any Student\n";
cout << "\n\t\t\tPress : 5 : Back to Home Page\n";
cout << "\n\t\t\tEnter your choice: ";

                    cin >> fmChoice;
                    switch (fmChoice) {
                        case 1:
                            FeeManagement::submitFees();
                            break;
                        case 2:
                            FeeManagement::viewFeeRecord();
                            break;
                        case 3:
                            FeeManagement::viewPendingFees();
                            break;
                        case 4:
                            Student::searchStudent();
                            break;
                        case 5:
                            break;
                        default:
                            cout << "Invalid choice! Try again.\n";
                            break;
                    }
                    if (fmChoice != 5) {
                        cout << "Press any key to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (fmChoice != 5);
                break;
            case 3:
                int vfsChoice;
                do {
                    clearScreen();
                    cout << "\t\t\t<< == View Fees Structure == >>\n";
cout << "\n\t\t\tPress : 1 : View Fees Structure\n";
cout << "\t\t\tPress : 2 : Update Fees Structure\n";
cout << "\n\t\t\tPress : 3 : Back to Home Page\n";
cout << "\n\t\t\tEnter your choice: ";

                    cin >> vfsChoice;
                    switch (vfsChoice) {
                        case 1:
                            FeeManagement::viewFee();
                            break;
                        case 2:
                            FeeManagement::updateFee();
                            break;
                        case 3:
                            break;
                        default:
                            cout << "Invalid choice! Try again.\n";
                            break;
                    }
                    if (vfsChoice != 3) {
                        cout << "Press any key to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (vfsChoice != 3);
                break;
            case 4:
                Student::viewStudents();
                cout << "Press any key to continue...";
                cin.ignore();
                cin.get();
                break;
            case 5:
                Student::searchStudent();
                cout << "Press any key to continue...";
                cin.ignore();
                cin.get();
                break;
            case 6:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice! Try again.\n";
                cout << "Press any key to continue...";
                cin.ignore();
                cin.get();
                break;
        }
    } while (choice != 6);
}

int main() {
	
	system("COLOR 70 ");
    FeeManagement::loadFeeInfo();
    Admin user;
    if (user.login()) {
        mainMenu();
    } else {
        cout << "\t\t\t\t\tLogin failed. Exiting...\n";
    }
    return 0;
}


