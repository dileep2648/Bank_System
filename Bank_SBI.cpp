#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
using namespace std;

class Security
{
public:
  string generateSalt()
  {
    string salt = "";
    for (int i = 0; i < 8; i++)
    {
      char c = 'a' + rand() % 26;
      salt += c;
    }
    return salt;
  }

  string hashPassword(string password, string salt)
  {
    string combined = password + salt;
    unsigned long hash = 5381;

    for (char c : combined)
    {
      hash = ((hash << 5) + hash) + c;
    }

    stringstream ss;
    ss << hash;
    return ss.str();
  }

  bool verifyPassword(string inputPassword, string storedHash, string salt)
  {
    string newHash = hashPassword(inputPassword, salt);
    return newHash == storedHash;
  }
};

class Account
{
protected:
  string accNumber;
  int balance;
  int id;

public:
  virtual void Deposit(int amount) = 0;
  virtual void Withdraw(int amount) = 0;
  virtual void display() = 0;
  virtual ~Account() {}
};

class SavingsAccount : public Account
{
public:
  SavingsAccount()
  {
    balance = 0;
    id = rand() % 1000;
  }

  void Deposit(int amount)
  {
    balance += amount;
    cout << "Deposit successful! Current balance: " << balance << endl;
  };

  void Withdraw(int amount)
  {
    if (amount > balance)
    {
      cout << "Insufficient balance!" << endl;
    }
    else
    {
      balance -= amount;
      cout << "Withdrawal successful! Current balance: " << balance << endl;
    }
  }

  void display()
  {
    cout << "Account ID: " << id << endl;
    cout << "Balance: " << balance << endl;
  }
};

class CurrentAccount : public Account
{
public:
  CurrentAccount()
  {
    balance = 0;
    id = rand() % 1000;
  }

  void Deposit(int amount)
  {
    balance += amount;
    cout << "Deposit successful! Current balance: " << balance << endl;
  };

  void Withdraw(int amount)
  {
    if (amount > balance)
    {
      cout << "Insufficient balance!" << endl;
    }
    else
    {
      balance -= amount;
      cout << "Withdrawal successful! Current balance: " << balance << endl;
    }
  }

  void display()
  {
    cout << "Account ID: " << id << endl;
    cout << "Balance: " << balance << endl;
  }
};

class User
{
protected:
  string name;
  string hashedPassword;
  string salt;
  int id;
  string phone;
  vector<Account *> accounts;

public:
  User()
  {
  }

  User(string name, string hashedPassword, int id, string salt)
  {
    this->name = name;
    this->hashedPassword = hashedPassword;
    this->id = id;
    this->salt = salt;
  }

  void setPhone(string phone)
  {
    this->phone = phone;
  }

  void display()
  {
    cout << "Name: " << name << endl;
    cout << "ID: " << id << endl;
    cout << "Phone: " << phone << endl;
  }

  int getId()
  {
    return id;
  }

  string gethashedPassword()
  {
    return hashedPassword;
  }

  string getSalt()
  {
    return salt;
  }

  void addAccount(Account *acc)
  {
    accounts.push_back(acc);
  }

  void removeAccount(Account *acc)
  {
    for (auto it = accounts.begin(); it != accounts.end(); ++it)
    {
      if (*it == acc)
      {
        accounts.erase(it);
        break;
      }
    }
  }

  Account *chooseAccount()
  {
    if (accounts.empty())
    {
      cout << "No accounts available!\n";
      return nullptr;
    }
    cout << "Choose an account: " << endl;
    int choice = 0;
    for (int i = 0; i < accounts.size(); i++)
    {
      cout << "Account " << i + 1 << ": " << endl;
      accounts[i]->display();
    }
    cin >> choice;

    if (choice < 1 || choice > accounts.size())
    {
      cout << "Invalid choice!" << endl;
      return nullptr;
    }
    return accounts[choice - 1];
  }

  void displayAccounts()
  {
    int i = 0;
    for (Account *acc : accounts)
    {
      cout << "Account " << i + 1 << ": " << endl;
      i++;
      acc->display();
    }
  }

  ~User()
  {
    for (Account *acc : accounts)
    {
      delete acc;
    }
  }
};

class AccManager
{
public:
  void createAccount(User &user)
  {
    cout << "choose the type of account you want to create: " << endl;
    cout << "1. Savings Account" << endl;
    cout << "2. Current Account" << endl;
    int choice;
    cin >> choice;
    if (choice == 1)
    {
      SavingsAccount *acc = new SavingsAccount();
      user.addAccount(acc);
      cout << "Savings Account created successfully!" << endl;
    }
    else if (choice == 2)
    {
      cout << "Current Account created successfully!" << endl;
      CurrentAccount *acc = new CurrentAccount();
      user.addAccount(acc);
    }
    else
    {
      cout << "Invalid choice!" << endl;
    }
  }

  void closeAccount(User &user)
  {

    Account *acc = user.chooseAccount();
    if (acc != nullptr)
    {
      user.removeAccount(acc);
      delete acc;
      cout << "Account closed successfully!" << endl;
    }
    else
    {
      cout << "No accounts to close!" << endl;
    }
  }
};

class Transaction
{
public:
  void deposit(int amount, Account &acc)
  {

    if (amount <= 0)
    {
      cout << "Invalid amount!" << endl;
      return;
    }

    else
    {
      acc.Deposit(amount);
    }
  }

  void withdraw(int amount, Account &acc)
  {
    acc.Withdraw(amount);
  }
};

class Authentication
{

  Security security;

public:
  User Register()
  {
    string name;
    string Password;
    int id;
    string hashedPassword;
    string phone;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your password: ";
    cin >> Password;
    id = rand() % 1000;
    string salt = security.generateSalt();
    hashedPassword = security.hashPassword(Password, salt);
    User user(name, hashedPassword, id, salt);
    cout << "Enter your phone number: ";
    cin >> phone;
    user.setPhone(phone);
    cout << "Signup successful! Your ID is " << id << endl;
    return user;
  }
  User *login(vector<User> &users)
  {
    string Password;
    int id;
    cout << "Enter your ID: ";
    cin >> id;
    cout << "Enter your password: ";
    cin >> Password;
    for (User &user : users)
    {
      if (user.getId() == id && security.verifyPassword(Password, user.gethashedPassword(), user.getSalt()))
      {
        cout << "Login successful! Welcome " << endl;
        user.display();
        return &user;
      }

      else if (user.getId() == id)
      {
        cout << "Incorrect password!" << endl;
        return nullptr;
      }
    }

    cout << "User not found!" << endl;
    return nullptr;
  }
};

class Bank
{
private:
  Authentication Auth;
  AccManager accManager;
  Transaction transaction;
  vector<User> users;

public:
  void Register()
  {
    User user = Auth.Register();
    users.push_back(user);
  }

  void login()
  {
    User *user = Auth.login(users);
    if (user != nullptr)
    {
      userMenu(*user);
    }
  }

  void userMenu(User &user)
  {
    int choice = 0;
    while (choice != 5)
    {
      cout << "1. Create Account" << endl;
      cout << "2. Close Account" << endl;
      cout << "3. Deposit" << endl;
      cout << "4. Withdraw" << endl;
      cout << "5. Logout" << endl;
      cin >> choice;
      switch (choice)
      {
      case 1:
      {
        accManager.createAccount(user);
        break;
      }
      case 2:
      {
        accManager.closeAccount(user);
        break;
      }
      case 3:
      {
        int amount = 0;
        Account *acc = user.chooseAccount();
        if (acc == nullptr)
        {
          cout << "No accounts available for deposit!" << endl;
          break;
        }
        cout << "Enter the amount to deposit: ";
        cin >> amount;
        transaction.deposit(amount, *acc);
        break;
      }

      case 4:
      {

        int amount2 = 0;
        Account *acc2 = user.chooseAccount();
        if (acc2 == nullptr)
        {
          cout << "No accounts available for withdrawal!" << endl;
          break;
        }

        cout << "Enter the amount to withdraw: ";
        cin >> amount2;
        transaction.withdraw(amount2, *acc2);

        break;
      }
      case 5:
      {
        cout << "Logout successful!" << endl;
        break;
      }
      default:
        cout << "Invalid choice!" << endl;
      }
    }
  }
};

int main()
{
  srand(time(0));
  Bank sbi;
  int choice;
  while (true)
  {
    cout << "1. register" << endl;
    cout << "2. login" << endl;
    cout << "3. exit" << endl;
    cin >> choice;
    switch (choice)
    {
    case 1:
      sbi.Register();
      break;
    case 2:
      sbi.login();
      break;
    case 3:
      cout << "Thank you for using SBI Bank!" << endl;
      return 0;
    default:
      cout << "Invalid choice!" << endl;
    }
  }
}