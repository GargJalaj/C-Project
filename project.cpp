#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

class Date {
public:
    int day;
    int month;
    int year;
};

class Appointment {
public:
    int patientID;
    Date date;
    char description[100];
    Appointment* next;
};

class Patient {
public:
    int id;
    char name[50];
    int age;
    char address[100];
    char phoneNumber[15];
    Appointment* appointments;
    Patient* next;
    Patient() : appointments(nullptr), next(nullptr) {}
};

class HospitalManagementSystem {
private:
    Patient* head;
    int patientCount;

public:
    HospitalManagementSystem() : head(nullptr), patientCount(0) {}

    ~HospitalManagementSystem() {
        freeList();
    }

    void addPatient();
    void displayPatients();
    void searchPatient();
    void addAppointment();
    void cancelAppointment();
    void modifyPatient();
    void displayAppointments(int patientID);
    void freeList();
    void saveToFile();
    void loadFromFile();

    void run() {
        loadFromFile();

        int choice;
        do {
            cout << "\nHospital Management System\n";
            cout << "1. Add Patient\n";
            cout << "2. Display Patients\n";
            cout << "3. Search Patient\n";
            cout << "4. Add Appointment\n";
            cout << "5. Cancel Appointment\n";
            cout << "6. Modify Patient\n";
            cout << "7. Save Data\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addPatient();
                    break;
                case 2:
                    displayPatients();
                    break;
                case 3:
                    searchPatient();
                    break;
                case 4:
                    addAppointment();
                    break;
                case 5:
                    cancelAppointment();
                    break;
                case 6:
                    modifyPatient();
                    break;
                case 7:
                    saveToFile();
                    break;
                case 8:
                    cout << "Exiting the program. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }

        } while (choice != 8);
    }
};

void HospitalManagementSystem::addPatient() {
    Patient* newPatient = new Patient;

    if (newPatient == nullptr) {
        cout << "Memory allocation failed.\n";
        exit(EXIT_FAILURE);
    }

    cout << "Enter patient name: ";
    cin >> newPatient->name;

    cout << "Enter patient age: ";
    cin >> newPatient->age;

    cout << "Enter patient address: ";
    cin >> newPatient->address;

    cout << "Enter patient phone number: ";
    cin >> newPatient->phoneNumber;

    newPatient->id = patientCount + 1;
    ++patientCount;
    newPatient->next = head;
    head = newPatient;

    cout << "Patient added successfully!\n";
}

void HospitalManagementSystem::displayPatients() {
    cout << "\nPatient ID\tName\t\tAge\t\tAddress\t\tPhone Number\n";
    cout << "-----------------------------------------------------------------\n";

    Patient* current = head;

    while (current != nullptr) {
        cout << current->id << "\t\t" << current->name << "\t\t" << current->age << "\t\t"
             << current->address << "\t\t" << current->phoneNumber << endl;
        current = current->next;
    }
}

void HospitalManagementSystem::searchPatient() {
    int searchID;
    cout << "Enter patient ID to search: ";
    cin >> searchID;

    Patient* current = head;

    while (current != nullptr) {
        if (current->id == searchID) {
            cout << "\nPatient ID\tName\t\tAge\t\tAddress\t\tPhone Number\n";
            cout << "-----------------------------------------------------------------\n";
            cout << current->id << "\t\t" << current->name << "\t\t" << current->age << "\t\t"
                 << current->address << "\t\t" << current->phoneNumber << endl;

            displayAppointments(searchID);

            return;
        }
        current = current->next;
    }

    cout << "Patient with ID " << searchID << " not found.\n";
}

void HospitalManagementSystem::addAppointment() {
    int patientID, day, month, year;
    char description[100];

    cout << "Enter patient ID for the appointment: ";
    cin >> patientID;

    Patient* current = head;

    while (current != nullptr) {
        if (current->id == patientID) {
            cout << "Enter appointment date (day month year): ";
            cin >> day >> month >> year;

            Appointment* newAppointment = new Appointment;

            if (newAppointment == nullptr) {
                cout << "Memory allocation failed.\n";
                exit(EXIT_FAILURE);
            }

            newAppointment->patientID = patientID;
            newAppointment->date.day = day;
            newAppointment->date.month = month;
            newAppointment->date.year = year;

            cout << "Enter appointment description: ";
            cin >> newAppointment->description;

            newAppointment->next = current->appointments;
            current->appointments = newAppointment;

            cout << "Appointment added successfully!\n";
            return;
        }
        current = current->next;
    }

    cout << "Patient with ID " << patientID << " not found.\n";
}

void HospitalManagementSystem::cancelAppointment() {
    int patientID, day, month, year;
    cout << "Enter patient ID for the appointment: ";
    cin >> patientID;

    Patient* current = head;

    while (current != nullptr) {
        if (current->id == patientID) {
            cout << "Enter appointment date to cancel (day month year): ";
            cin >> day >> month >> year;

            Appointment* prev = nullptr;
            Appointment* appointments = current->appointments;

            while (appointments != nullptr) {
                if (appointments->patientID == patientID &&
                    appointments->date.day == day &&
                    appointments->date.month == month &&
                    appointments->date.year == year) {
                    if (prev == nullptr) {
                        current->appointments = appointments->next;
                    } else {
                        prev->next = appointments->next;
                    }
                    delete appointments;
                    cout << "Appointment canceled successfully!\n";
                    return;
                }
                prev = appointments;
                appointments = appointments->next;
            }

            cout << "Appointment not found for the specified date.\n";
            return;
        }
        current = current->next;
    }

    cout << "Patient with ID " << patientID << " not found.\n";
}

void HospitalManagementSystem::modifyPatient() {
    int patientID, choice;
    cout << "Enter patient ID to modify: ";
    cin >> patientID;

    Patient* current = head;

    while (current != nullptr) {
        if (current->id == patientID) {
            cout << "Patient found. Choose what to modify:\n";
            cout << "1. Name\n";
            cout << "2. Age\n";
            cout << "3. Address\n";
            cout << "4. Phone Number\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter new name: ";
                    cin >> current->name;
                    break;
                case 2:
                    cout << "Enter new age: ";
                    cin >> current->age;
                    break;
                case 3:
                    cout << "Enter new address: ";
                    cin >> current->address;
                    break;
                case 4:
                    cout << "Enter new phone number: ";
                    cin >> current->phoneNumber;
                    break;
                default:
                    cout << "Invalid choice.\n";
                    return;
            }

            cout << "Patient information modified successfully!\n";
            return;
        }
        current = current->next;
    }

    cout << "Patient with ID " << patientID << " not found.\n";
}

void HospitalManagementSystem::displayAppointments(int patientID) {
    Patient* current = head;

    while (current != nullptr) {
        if (current->id == patientID) {
            Appointment* appointments = current->appointments;

            if (appointments != nullptr) {
                cout << "\nAppointments for " << current->name << ":\n";
                cout << "Date\t\tDescription\n";
                cout << "---------------------------------\n";

                while (appointments != nullptr) {
                    cout << appointments->date.day << '/' << appointments->date.month << '/' << appointments->date.year
                         << "\t" << appointments->description << endl;
                    appointments = appointments->next;
                }
            } else {
                cout << "No appointments for " << current->name << ".\n";
            }

            return;
        }
        current = current->next;
    }

    cout << "Patient with ID " << patientID << " not found.\n";
}

void HospitalManagementSystem::freeList() {
    Patient* current = head;
    Patient* next;

    while (current != nullptr) {
        Appointment* appointments = current->appointments;
        Appointment* nextAppointment;

        while (appointments != nullptr) {
            nextAppointment = appointments->next;
            delete appointments;
            appointments = nextAppointment;
        }

        next = current->next;
        delete current;
        current = next;
    }
}

void HospitalManagementSystem::saveToFile() {
    ofstream file("patient_data.txt");

    if (!file.is_open()) {
        cout << "Error opening file for writing.\n";
        return;
    }

    Patient* current = head;

    while (current != nullptr) {
        file << "P\t" << current->id << '\t' << current->name << '\t' << current->age << '\t'
             << current->address << '\t' << current->phoneNumber << endl;

        Appointment* appointments = current->appointments;

        while (appointments != nullptr) {
            file << "A\t" << current->id << '\t' << appointments->date.day << '\t' << appointments->date.month << '\t'
                 << appointments->date.year << '\t' << appointments->description << endl;
            appointments = appointments->next;
        }

        current = current->next;
    }

    file.close();
    cout << "Data saved to file.\n";
}

void HospitalManagementSystem::loadFromFile() {
    ifstream file("patient_data.txt");

    if (!file.is_open()) {
        cout << "No previous data found.\n";
        return;
    }

    while (!file.eof()) {
        char type;
        int id, age, day, month, year;
        char name[50], address[100], phoneNumber[15], description[100];

        if (file >> type) {
            if (type == 'P') { 
                file >> id >> name >> age >> address >> phoneNumber;

                Patient* newPatient = new Patient;

                if (newPatient == nullptr) {
                    cout << "Memory allocation failed.\n";
                    exit(EXIT_FAILURE);
                }

                newPatient->id = id;
                strcpy(newPatient->name, name);
                newPatient->age = age;
                strcpy(newPatient->address, address);
                strcpy(newPatient->phoneNumber, phoneNumber);

                newPatient->appointments = nullptr;

                newPatient->next = head;
                head = newPatient;

                ++patientCount;
            } else if (type == 'A') { 
                file >> id >> day >> month >> year >> description;

                Patient* current = head;

                while (current != nullptr) {
                    if (current->id == id) {
                        Appointment* newAppointment = new Appointment;

                        if (newAppointment == nullptr) {
                            cout << "Memory allocation failed.\n";
                            exit(EXIT_FAILURE);
                        }

                        newAppointment->patientID = id;
                        newAppointment->date.day = day;
                        newAppointment->date.month = month;
                        newAppointment->date.year = year;
                        strcpy(newAppointment->description, description);

                        newAppointment->next = current->appointments;
                        current->appointments = newAppointment;

                        break;
                    }
                    current = current->next;
                }
            }
        }
    }

    file.close();
    cout << "Data loaded from file.\n";
}

int main() {
    HospitalManagementSystem system; 
    system.run();

    return 0;
}
