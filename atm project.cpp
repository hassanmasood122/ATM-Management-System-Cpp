#include <iostream>
using namespace std;

struct Account
{
    string name;
    int pin;
    int balance;
};

int main()
{
    Account users[5] =
    {
        {"Ali", 1111, 50000},
        {"Amna", 2222, 40000},
        {"Hassan", 3333, 60000},
        {"Sara", 4444, 35000},
        {"Ayesha", 5555, 45000}
    };

    int enteredPin, attempts = 0;
    int currentUser = -1;
    while (attempts < 3)
    {
        cout << "Enter PIN: "<<endl;
        cin >> enteredPin;

        for (int i = 0; i < 5; i++)
        {
            if (enteredPin == users[i].pin)
            {
                currentUser = i;
                break;
            }
        }

        if (currentUser != -1)
            break;

        attempts++;
        cout << "Wrong PIN!"<<endl;
    }

    if (currentUser == -1)
    {
        cout << "ACCOUNT LOCKED! Restart program."<<endl;
        return 0;
    }
    cout << "Welcome, " << users[currentUser].name << "!"<<endl;

    int choice, amount;

    do
    {
        cout << "--- ATM MENU ---"<<endl;
        cout << "1. Withdraw"<<endl;
        cout << "2. Deposit"<<endl;
        cout << "3. Check Balance"<<endl;
        cout << "4. Logout"<<endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter amount to withdraw: ";
            cin >> amount;

            if (amount > 0 && amount <= users[currentUser].balance)
            {
                users[currentUser].balance -= amount;
                cout << "Withdraw successful!"<<endl;
            }
            else
            {
                cout << "Insufficient balance!"<<endl;
            }
            break;

        case 2:
            cout << "Enter amount to deposit: ";
            cin >> amount;

            if (amount > 0)
            {
                users[currentUser].balance += amount;
                cout << "Deposit successful!"<<endl;
            }
            break;

        case 3:
            cout << "Current Balance: Rs "
                 << users[currentUser].balance << endl;
            break;

        case 4:
            cout << "Logged out successfully."<<endl;
            break;

        default:
            cout << "Invalid choice!"<<endl;
        }

    } while (choice != 4);

    return 0;
}
