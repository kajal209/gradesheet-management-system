#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <map>
#include <sstream>
#include <vector>
#include <regex>
#include <ctime>

using namespace std;

class marksheet {
private:
    static const int MAX_MARKS = 100;
    static const int MIN_PASS_MARKS = 33;
    string name, fa_name, mo_name, s_name, dob;
    static int next_id;
    int GRADE, student_id;
    int phy_marks, eng_marks, chem_marks, math_marks, com_marks;

    string getGrade(int marks) const;
    string getTotalGrade(float percentage) const;
    void print_marksheet(ostream& os) const;
    bool validateDate(const string& date);
    bool validateName(const string& name);

public:
    marksheet(); 
    void take_user_input();
    void generate_marksheet();
    void save_marksheet() const;
    void save_to_file(ofstream& file) const; 
    static void load_marksheets(map<int, marksheet>& marksheets);
    
    int getStudentID() const { return student_id; }
    string getName() const { return name; }

    void update_marksheet();
    static void add_marksheet(map<int, marksheet>& marksheets);
    static void show_marksheet(const map<int, marksheet>& marksheets, int id);
    static void delete_marksheet(map<int, marksheet>& marksheets, int id);
    static void list_all_marksheets(const map<int, marksheet>& marksheets);
    static void search_by_name(const map<int, marksheet>& marksheets, const string& name);
    static void save_all_marksheets(const map<int, marksheet>& marksheets);
};

// Helper function to clear the console screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int marksheet::next_id = 1; 

marksheet::marksheet() : GRADE(0), student_id(next_id++), phy_marks(0), eng_marks(0), chem_marks(0), math_marks(0), com_marks(0) {}

string marksheet::getGrade(int marks) const {
    if (marks >= 91) return "A+";
    else if (marks >= 81) return "A";
    else if (marks >= 61) return "B";
    else if (marks >= 41) return "C";
    else if (marks >= 31) return "D";
    else return "F";
}

string marksheet::getTotalGrade(float percentage) const {
    if (phy_marks < MIN_PASS_MARKS || eng_marks < MIN_PASS_MARKS || chem_marks < MIN_PASS_MARKS ||
        math_marks < MIN_PASS_MARKS || com_marks < MIN_PASS_MARKS) {
        return "F";
    }

    if (percentage >= 91) return "A+";
    else if (percentage >= 81) return "A";
    else if (percentage >= 61) return "B";
    else if (percentage >= 41) return "C";
    else if (percentage >= 31) return "D";
    else return "F";
}

bool marksheet::validateDate(const string& date) {
    regex date_pattern("^\\d{2}/\\d{2}/\\d{4}$");

    if (!regex_match(date, date_pattern))
        return false;

    int day, month, year;
    sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100)
        return false;

    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (leap ? 29 : 28))
            return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }

    return true;
}

bool marksheet::validateName(const string& name) {
    return regex_match(name, regex("^[a-zA-Z ]+$"));
}

void marksheet::print_marksheet(ostream& os) const {
    int grand_total = phy_marks + eng_marks + chem_marks + math_marks + com_marks;
    float percentage = grand_total / 5.0f;

    os << "\n---------------------------------------------------------------------------------\n";
    os << "|                   EXAMINATION GRADE-SHEET                                         \n";
    os << "|-----------------------------------------------------------------------------------\n";
    os << "|STUDENT ID: " << student_id << endl;
    os << "|THE GRADE(S) SECURED BY: " << name << endl;
    os << "|Date of birth: " << dob << "\t\t" << "GRADE: " << GRADE << "th" << endl;
    os << "|Father name: " << fa_name << "\t\t\t" << "Mother name: " << mo_name << endl;
    os << "|School name: " << s_name << endl;
    os << "|------------------------------------------------------------------------------------\n";
    os << "|\tSUBJECTS      \tMAX MARKS\tMIN MARKS\tTHEORY MARKS\tGrade\n";
    os << "|------------------------------------------------------------------------------------\n";
    os << "|\tPHYSICS       \t  100      \t   " << MIN_PASS_MARKS << "       \t   " << phy_marks << "\t\t" << getGrade(phy_marks) << endl;
    os << "|\tENGLISH       \t  100      \t   " << MIN_PASS_MARKS << "       \t   " << eng_marks << "\t\t" << getGrade(eng_marks) << endl;
    os << "|\tCHEMISTRY     \t  100      \t   " << MIN_PASS_MARKS << "       \t   " << chem_marks << "\t\t" << getGrade(chem_marks) << endl;
    os << "|\tMATHEMATICS   \t  100      \t   " << MIN_PASS_MARKS << "       \t   " << math_marks << "\t\t" << getGrade(math_marks) << endl;
    os << "|\tCOMPUTER      \t  100      \t   " << MIN_PASS_MARKS << "       \t   " << com_marks << "\t\t" << getGrade(com_marks) << endl;
    os << "|------------------------------------------------------------------------------------\n";
    os << "|\t              \t  500         GRAND TOTAL\t    " << grand_total << "\t\t" << getTotalGrade(percentage) << endl;
    os << "|------------------------------------------------------------------------------------\n";
}

void marksheet::save_marksheet() const {
    ofstream file("marksheet.txt", ios::app);
    if (file) {
        print_marksheet(file);
        file.close();
        cout << "Marksheet saved successfully to 'marksheet.txt'.\n";
    } else {
        cerr << "Error: Unable to open file to save marksheet.\n";
    }
}

void marksheet::save_to_file(ofstream& file) const {
    file << student_id << ',' << name << ',' << fa_name << ',' << mo_name << ',' << s_name << ',' << dob << ','
         << GRADE << ',' << phy_marks << ',' << eng_marks << ',' << chem_marks << ',' << math_marks << ',' << com_marks << '\n';
}

void marksheet::load_marksheets(map<int, marksheet>& marksheets) {
    ifstream file("marksheets_data.csv");
    if (!file) {
        cerr << "No existing data found. Starting fresh.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string token;
        marksheet m;

        getline(ss, token, ',');
        m.student_id = stoi(token);
        getline(ss, m.name, ',');
        getline(ss, m.fa_name, ',');
        getline(ss, m.mo_name, ',');
        getline(ss, m.s_name, ',');
        getline(ss, m.dob, ',');
        getline(ss, token, ',');
        m.GRADE = stoi(token);
        getline(ss, token, ',');
        m.phy_marks = stoi(token);
        getline(ss, token, ',');
        m.eng_marks = stoi(token);
        getline(ss, token, ',');
        m.chem_marks = stoi(token);
        getline(ss, token, ',');
        m.math_marks = stoi(token);
        getline(ss, token, ',');
        m.com_marks = stoi(token);

        marksheets[m.student_id] = m;
        next_id = max(next_id, m.student_id + 1);
    }

    file.close();
    cout << "Marksheets loaded successfully.\n";
}

void marksheet::save_all_marksheets(const map<int, marksheet>& marksheets) {
    ofstream file("marksheets_data.csv");
    if (!file) {
        cerr << "Error: Unable to open file to save marksheets.\n";
        return;
    }

    for (const auto& entry : marksheets) {
        entry.second.save_to_file(file);
    }

    file.close();
    cout << "All marksheets saved successfully.\n";
}

void marksheet::take_user_input() {
    cout << "Enter your name: ";
    getline(cin >> ws, name);
    while (!validateName(name)) {
        cerr << "Invalid name. Please enter only alphabets and spaces: ";
        getline(cin >> ws, name);
    }

    cout << "Enter your father name: ";
    getline(cin >> ws, fa_name);
    while (!validateName(fa_name)) {
        cerr << "Invalid father name. Please enter only alphabets and spaces: ";
        getline(cin >> ws, fa_name);
    }

    cout << "Enter your mother name: ";
    getline(cin >> ws, mo_name);
    while (!validateName(mo_name)) {
        cerr << "Invalid mother name. Please enter only alphabets and spaces: ";
        getline(cin >> ws, mo_name);
    }

    cout << "Enter school name: ";
    getline(cin >> ws, s_name);

    cout << "Enter your date of birth (dd/mm/yyyy): ";
    getline(cin >> ws, dob);
    while (!validateDate(dob)) {
        cerr << "Invalid date format. Please use the format dd/mm/yyyy: ";
        getline(cin >> ws, dob);
    }

    cout << "Enter your current grade (1-12): ";
    cin >> GRADE;
    while (GRADE < 1 || GRADE > 12) {
        cerr << "Invalid grade. Please enter a grade between 1 and 12: ";
        cin >> GRADE;
    }

    cout << "Enter your Physics marks: ";
    cin >> phy_marks;
    while (phy_marks < 0 || phy_marks > MAX_MARKS) {
        cerr << "Invalid marks. Please enter marks between 0 and " << MAX_MARKS << ": ";
        cin >> phy_marks;
    }

    cout << "Enter your English marks: ";
    cin >> eng_marks;
    while (eng_marks < 0 || eng_marks > MAX_MARKS) {
        cerr << "Invalid marks. Please enter marks between 0 and " << MAX_MARKS << ": ";
        cin >> eng_marks;
    }

    cout << "Enter your Chemistry marks: ";
    cin >> chem_marks;
    while (chem_marks < 0 || chem_marks > MAX_MARKS) {
        cerr << "Invalid marks. Please enter marks between 0 and " << MAX_MARKS << ": ";
        cin >> chem_marks;
    }

    cout << "Enter your Mathematics marks: ";
    cin >> math_marks;
    while (math_marks < 0 || math_marks > MAX_MARKS) {
        cerr << "Invalid marks. Please enter marks between 0 and " << MAX_MARKS << ": ";
        cin >> math_marks;
    }

    cout << "Enter your Computer marks: ";
    cin >> com_marks;
    while (com_marks < 0 || com_marks > MAX_MARKS) {
        cerr << "Invalid marks. Please enter marks between 0 and " << MAX_MARKS << ": ";
        cin >> com_marks;
    }
}

void marksheet::generate_marksheet() {
    take_user_input();
    print_marksheet(cout);
    save_marksheet();
}

void marksheet::update_marksheet() {
    cout << "Updating marksheet for student ID: " << student_id << "\n";
    take_user_input();
    print_marksheet(cout);

    map<int, marksheet> marksheets;
    marksheet::load_marksheets(marksheets); // Load current data
    marksheets[student_id] = *this; // Update the current object in the map
    marksheet::save_all_marksheets(marksheets); // Save all marksheets back to the file

    cout << "Marksheet updated and saved successfully.\n";

}

void marksheet::add_marksheet(map<int, marksheet>& marksheets) {
    marksheet m;
    m.generate_marksheet();
    marksheets[m.getStudentID()] = m;
    save_all_marksheets(marksheets);
}

void marksheet::show_marksheet(const map<int, marksheet>& marksheets, int id) {
    auto it = marksheets.find(id);
    if (it != marksheets.end()) {
        it->second.print_marksheet(cout);
    } else {
        cout << "Marksheet with Student ID " << id << " not found.\n";
    }
}

void marksheet::delete_marksheet(map<int, marksheet>& marksheets, int id) {
    if (marksheets.erase(id)) {
        save_all_marksheets(marksheets);
        cout << "Marksheet with Student ID " << id << " deleted successfully.\n";
    } else {
        cout << "Marksheet with Student ID " << id << " not found.\n";
    }
}

void marksheet::list_all_marksheets(const map<int, marksheet>& marksheets) {
    for (const auto& entry : marksheets) {
        entry.second.print_marksheet(cout);
    }
}

void marksheet::search_by_name(const map<int, marksheet>& marksheets, const string& name) {
    bool found = false;
    for (const auto& entry : marksheets) {
        if (entry.second.getName() == name) {
            entry.second.print_marksheet(cout);
            found = true;
        }
    }
    if (!found) {
        cout << "No marksheet found for name " << name << ".\n";
    }
}

int main() {
    map<int, marksheet> marksheets;
    marksheet::load_marksheets(marksheets);

    while (true) {
        clear_screen();
        cout << "Marksheet Management System\n";
        cout << "----------------------------\n";
        cout << "1. Add a new marksheet\n";
        cout << "2. Show a marksheet by Student ID\n";
        cout << "3. update marksheet by Student ID\n";
        cout << "4. Delete a marksheet by Student ID\n";
        cout << "5. List all marksheets\n";
        cout << "6. Search marksheet by student name\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        int choice, id;
        string name;
        cin >> choice;

        switch (choice) {
            case 1:
                marksheet::add_marksheet(marksheets);
                break;
            case 2:
                cout << "Enter Student ID: ";
                cin >> id;
                marksheet::show_marksheet(marksheets, id);
                //break;
            case 3:
                cout << "Enter student ID to update: ";
                cin >> id;
                if (marksheets.find(id) != marksheets.end()) {
                    marksheets[id].update_marksheet();
                    marksheet::save_all_marksheets(marksheets);
                } else {
                    cout << "Marksheet with ID " << id << " not found.\n";
                }
                break;
             case 4:
                cout << "Enter Student ID: ";
                cin >> id;
                marksheet::delete_marksheet(marksheets, id);
                break;
            case 5:
                marksheet::list_all_marksheets(marksheets);
                break;
            case 6:
                cout << "Enter Student Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, name);
                marksheet::search_by_name(marksheets, name);
                break;
            case 7:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        if (choice != 7) {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

    }

    return 0;
}
