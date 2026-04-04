#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <fstream>
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
  virtual bool Deposit(int amount) = 0;
  virtual bool Withdraw(int amount) = 0;
  virtual int getBalance() = 0;
  virtual void setBalance(int balance)=0;
  virtual void showBalance() = 0;
  virtual void display() = 0;
  virtual string getType() = 0;
  virtual int getId() = 0;
  virtual void setId(int id)
  {
    this->id = id;
  }
  virtual void applyInterest() {};
};

class SavingsAccount : public Account
{
public:
  SavingsAccount()
  {
    balance = 0;
    id = rand() % 1000;
  }

  int getId()
  {
    return id;
  }

  string getType()
  {

    return "savings";
  }

  int getBalance()
  {
    return balance;
  }

  void setBalance(int balance)
  {
    this->balance = balance;
  }

  void showBalance()
  {
    cout << "your current balance is " << balance << endl;
  }

  void applyInterest()
  {
    balance += (balance * 3) / 100;
  }

  bool Deposit(int amount)
  {
    balance += amount;
    cout << "Deposit successful! Current balance: \n"
         << balance << endl;
    return true;
  };

  bool Withdraw(int amount)
  {
   
    if (amount > balance)
    {
      cout << "Insufficient balance!" << endl;
      return false;
    }
    else if ((balance - amount) < 5000)
    {
      cout << "Balance goes below minimum! You will be charged Rs 200 penalty\n";
      cout << "Enter 1 to continue or 2 to cancel: ";
      int confirm;
      cin >> confirm;
      if (confirm == 1)
      {
        balance -= (amount + 200 );
        cout << "Withdrawal successful! Current balance: " << balance << endl;
        return true;
      }
      else
      {
        cout << "Withdrawal cancelled\n";
        return false;
      }
    }

    else
    {
      balance -= (amount );
      cout << "withdrawl succesfull";
      return true;
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

  string getType()
  {
    return "current";
  }

  int getId()
  {
    return id;
  }

  int getBalance()
  {
    cout << "your current balance is " << balance << endl;
    return balance;
  }

  void setBalance(int balance)
  {
    this->balance = balance;
  }

  void showBalance()
  {
    cout << "your current balance is " << balance << endl;
  }

  bool Deposit(int amount)
  {
    balance += amount;
    cout << "Deposit successful! Current balance: \n"
         << balance << endl;
    return true;
  };

  bool Withdraw(int amount)
  {
    int choice = 0;
    if ((balance - amount) < 5000)
    {
      cout << "your balance goes less than the mimimum balnce\n";
      cout << "u will be charged rs200 penalty \n";
      cout << "enter 1 if u want to continu and 2 to stop the withdrawl\n";
      cin >> choice;

      if (choice == 1)
      {
        if (amount > (balance + 5000))
        {
          cout << "Insufficient balance!" << endl;
          return false;
        }
        else
        {
          balance -= (amount + 200 );
          cout << "Withdrawal successful! Current balance: " << balance << endl;
          return true;
        }
      }

      else
      {
        cout << "no withdrawl made\n";
        return false;
      }
    }

    else
    {
      balance -= (amount);
      return true;
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

  vector<Account *> &getAccounts()
  { // & means return by reference, no copy
    return accounts;
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
      ofstream file("accounts.txt", ios::app);
      if (file.is_open())
      {
        file << user.getId() << " " << acc->getType() << " " << acc->getId() << " " << 0 << endl;
        file.close();
      }
    }
    else if (choice == 2)
    {
      cout << "Current Account created successfully!" << endl;
      CurrentAccount *acc = new CurrentAccount();
      user.addAccount(acc);
      ofstream file("accounts.txt", ios::app);
      if (file.is_open())      {
        file << user.getId() << " " << acc->getType() << " " << acc->getId() << " " << 0 << endl;
        file.close();
      }
    }
    else
    {
      cout << "Invalid choice!" << endl;
    }
  }

  bool closeAccount(User &user)
  {

    Account *acc = user.chooseAccount();
    if (acc != nullptr)
    {
      user.removeAccount(acc);
      delete acc;
      cout << "Account closed successfully!" << endl;
      return true;
    }
    else
    {
      cout << "No accounts to close!" << endl;
      return false;
    }
  }
};

class Transaction
{
public:
  bool deposit(int amount, Account &acc)
  { ofstream file("transactions.txt", ios::app);

    if (amount <= 0)
    {
      cout << "Invalid amount!" << endl;
      return false;
    }

    else
    {
      if(acc.Deposit(amount))
      {
        if (file.is_open())
        {
          file << "Deposit of Rs " << amount << " to account ID " << acc.getId() << endl;
          file.close();
        } 
      }
      return true;
    }
  }

  bool withdraw(int amount, Account &acc)
  { ofstream file("transactions.txt", ios::app);

    if (amount <= 0)
    {
      cout << "Invalid amount!" << endl;
      return false;
    }
    if(acc.Withdraw(amount))
    {
      if (file.is_open())
      {
        file << "Withdrawal of Rs " << amount << " from account ID " << acc.getId() << endl;
        file.close();
      } 
    }
    return true;
  }

  bool Transfer(int amount, Account &acc1, Account &acc2)
  {  ofstream file("transactions.txt", ios::app);
    if (amount < 0)
    {
      cout << "invalid amount\n";
      return false;
    }
    else
    {
      if (acc1.Withdraw(amount) && acc2.Deposit(amount))
      {  if (file.is_open())
        {
          file << "Transfer of Rs " << amount << " from account ID " << acc1.getId() << " to account ID " << acc2.getId() << endl;
          file.close();
        }
        cout << "transfer successful\n";
        return true;
      }
      else
      {
        cout << "transfer failed\n";
        return false;
      }

      return true;
    }
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
    string hashedPassword;
    string phone;
    int id;

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

    ofstream file("users.txt", ios::app);
    if (file.is_open())
    {
      file << id << " " << name << " " << phone << " " << hashedPassword << " " << salt << endl;
      file.close();
    }

    cout << "Signup successful! Your ID is " << id << endl;
    return user;
  }
  User *login(vector<User> &users)
  {
    int id;
    string Password;

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
  Bank()
  {
    loadUsers();
    loadAccounts();
  }

  void loadUsers()
  {
    ifstream file("users.txt");
    string name, phone, hashedPassword, salt;
    int id;

    while (file >> id >> name >> phone >> hashedPassword >> salt)
    {
      User user(name, hashedPassword, id, salt);
      user.setPhone(phone);
      users.push_back(user);
    }
    file.close();
  }

  void loadAccounts()
  { 

    ifstream file2("accounts.txt");
    int userId, accId;
    string type;
    int balance;
    while (file2 >> userId >> type >> accId >> balance)
    {
        for(User &u : users)
        {
          if (u.getId() == userId)
          {
            Account *acc;
            if (type == "savings")
            {
              acc = new SavingsAccount();
              acc->setBalance(balance);
              acc->setId(accId);
            }
            else
            {
              acc = new CurrentAccount();
              acc->setBalance(balance);
              acc->setId(accId);
            }
            u.addAccount(acc);
          }
        } 
     
    }

    file2.close();
  }

  void SaveToFile()
  {
    ofstream file("accounts.txt");
    for (User &user : users)
    {
      if (file.is_open())
      {
        for (Account *acc : user.getAccounts())
        {
          file << user.getId() << " " << acc->getType() << " " << acc->getId() << " " << acc->getBalance() << endl;
        }
      }
    }
    file.close();
  }


  void Register()
  {
    User user = Auth.Register();
    users.push_back(std::move(user));
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
    while (choice != 8)
    {
      cout << "1. Create Account" << endl;
      cout << "2. Close Account" << endl;
      cout << "3. Deposit" << endl;
      cout << "4. Withdraw" << endl;
      cout << "5. Simulate one year" << endl;
      cout << "6. Show balance" << endl;
      cout << "7. Transfer money" << endl;
      cout << "8. Logout" << endl;
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
        if (accManager.closeAccount(user))
        {
          SaveToFile();
        }
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
        if (transaction.deposit(amount, *acc))
        {
          SaveToFile();
        }
        break;
      }

      case 4:
      {

        int amount2 = 0;
        Account *acc2 = user.chooseAccount();
        if (acc2 == nullptr)
        {
          cout << "Account not found!" << endl;
          break;
        }

        cout << "Enter the amount to withdraw: ";
        cin >> amount2;
        if (transaction.withdraw(amount2, *acc2))
        {
          SaveToFile();
        }

        break;
      }
      case 5:
      {
        for (Account *acc : user.getAccounts())
        {
          if (acc->getType() == "savings")
          {
            acc->applyInterest();
            SaveToFile();
          }
        }
        break;
      }

      case 6:
      {
        Account *acc2 = user.chooseAccount();
        if (acc2 == nullptr)
        {
          cout << "Account not found!" << endl;
          break;
        }
        else
        {
          acc2->showBalance();
        }

        break;
      }

      case 7:
      {
        int amount;
        int choice;
        int thisID = user.getId();
        Account *acc1 = user.chooseAccount();

        if (acc1 == nullptr)
        {
          cout << "Account not found!" << endl;
          break;
        }

        cout << "enter amount\n";
        cin >> amount;
        cout << "chose the reciver by id\n";

        for (User &user : users)
        { if(user.getId() != thisID){
          user.display();
          cout << "------------------" << endl;}
        }
        cin >> choice;
        Account *acc2 = NULL;

        for (User &u : users)
        {
          if (u.getId() == choice)
          {
            acc2 = u.chooseAccount();
            if (acc2 != NULL)
            {
              if (transaction.Transfer(amount, *acc1, *acc2))
              {
                SaveToFile();
              }
            }
            else
            {
              cout << "account not found\n";
            }
          }
        }

        break;
      }
      case 8:
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
  };

  return 0;
}
