#include <iostream>
using namespace std;

int main() {
    int pin = 1234, enteredPin;
    int choice;
    float balance = 10000;
    float amount;

    cout << "----- Welcome to ATM -----"<<endl;
    cout << "Enter your PIN: "<<endl;
    cin >> enteredPin;

    if (enteredPin == pin) {
        do {
            cout << "--- ATM Menu ---"<<endl;
            cout << "1. Check Balance"<<endl;
            cout << "2. Deposit Money"<<endl;
            cout << "3. Withdraw Money"<<endl;
            cout << "4. Exit"<<endl;
            cout << "Choose option: "<<endl;
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Your Balance is: " << balance << endl;
                    break;

                case 2:
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    balance += amount;
                    cout << "Amount Deposited Successfully"<<endl;
                    break;

                case 3:
                    cout << "Enter amount to withdraw: ";
                    cin >> amount;
                    if (amount <= balance) {
                        balance -= amount;
                        cout << "Please collect your cash"<<endl;
                    } else {
                        cout << "Insufficient Balance"<<endl;
                    }
                    break;

                case 4:
                    cout << "Thank you for using ATM"<<endl;
                    break;

                default:
                    cout << "Invalid choice"<<endl;
            }

        } while (choice != 4);
    } else {
        cout << "Wrong PIN. Access Denied!"<<endl;
    }

    return 0;
}
