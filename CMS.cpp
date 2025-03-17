#include <iostream>
#include <string>
using namespace std;
const int maxcontact = 100;

int addnewcontact(string names[100], string phones[100], int& totalcontacts) {
    if (totalcontacts >= maxcontact) {
        cout << "Sorry. The contact list is full.\n\n";
        return 0;
    }

    cout<<"ENTER THE NAME OF CONTACT : ";
    cin.ignore();
    string name;
    getline(cin, name);

    for(char c : name) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            cout << "Invalid name. Use letters and spaces only.\n";
            return 0;
        }
    }
    names[totalcontacts] = name;
    cout<<"ENTER THE PHONE NUMBER: ";
    getline(cin, phones[totalcontacts]);
    totalcontacts++;
    cout<<"\nNEW CONTACT ADDED SUCCESSFULLY!\n\n";
    return 0;
}
int showallcontacts(string names[100], string phones[100], int totalcontacts) {
    if(totalcontacts == 0) {
        cout << "\nNO CONTACT TO DISPLAY.\n\n";
        return 0;
    }

    cout<<"\nALL CONTACTS : \n";
    for(int i = 0; i < totalcontacts; i++) {
        cout << i + 1 << " . " << names[i] << " - " << phones[i] << endl;
    }
    return 0;
}

int removecontact(string names[100], string phones[100], int& totalcontacts) {
    if(totalcontacts == 0) {
        cout << "\nTHERE ARE NO CONTACTS TO REMOVE.\n\n";
        return 0;
    }
    showallcontacts(names, phones, totalcontacts);
    int contacttoremove;
    cout<<"\nENTER THE CONTACT NUMBER TO DELETE (or 0 to cancel): ";
    cin>>contacttoremove;

    if(contacttoremove == 0) {
        cout << "\nOperation cancelled.\n\n";
        return 0;
    }

    if (contacttoremove < 1 || contacttoremove > totalcontacts) {
        cout << "\nINVALID SELECTION!\n\n";
        return 0;
    }

    for (int i = contacttoremove - 1; i < totalcontacts - 1; i++) {
        names[i] = names[i + 1];
        phones[i] = phones[i + 1];
    }

    totalcontacts--;
    cout<<"\nCONTACT REMOVED SUCCESSFULLY.\n\n";
    return 0;
}

int main() {
    string input;
    int choice;
    string contactnames[maxcontact];
    string contactphones[maxcontact];
    int totalcontacts = 0;
    int result;

    while (true) {
        cout << "\n=======================================\n";
        cout << "     Contact Management System\n";
        cout << "=======================================\n";
        cout << "1. Add a New Contact\n";
        cout << "2. View All Contacts\n";
        cout << "3. Remove a Contact\n";
        cout << "0. Exit\n";
        cout << "---------------------------------------\n";
        cout << "Please choose an option (0-3): ";

        cin >> input;
        bool is_valid = true;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] < '0' || input[i] > '9') {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            choice = stoi(input);
            switch (choice) {
                case 1:
                    result = addnewcontact(contactnames, contactphones, totalcontacts);
                    break;
                case 2:
                    result = showallcontacts(contactnames, contactphones, totalcontacts);
                    break;
                case 3:
                    result = removecontact(contactnames, contactphones, totalcontacts);
                    break;
                case 0:
                    cout << "Exiting the program. Goodbye!\n";
                    return 0;
                default:
                    cout << "Invalid option. Please try again.\n";
            }
        } else {
            cout << "Invalid input. Please enter a number between 0 and 3.\n";
        }
    }
}
