#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

const int MIN_BALANCE=5000;
const int PENALTY=200;


class Utility{
  private:
  static vector <int> userIds;
  static vector <int> accountIds;
  public:


  static string getTime() {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        return asctime(tm);
    }

  static int  userIdGenerator(){
    int id;
    bool isDuplicate;
    do{
         isDuplicate=false;
         id=rand()%1000;
         for(int existingIds : userIds){
          if(id==existingIds){
            isDuplicate=true;
            break;
          }
         } 
    }while(isDuplicate);
    userIds.push_back(id);
    return id;
  }

  static int accIdGenerator(){
    int id;
    bool isDuplicate;
    do{
         isDuplicate=false;
         id=rand()%1000;
         for(int existingIds : accountIds){
          if(id==existingIds){
            isDuplicate=true;
            break;
          }
         }
    }while(isDuplicate);
    accountIds.push_back(id);
    return id;
  }

  static void loadUserId(int id) {
        userIds.push_back(id);
    }
  static void loadAccountId(int id) {
        accountIds.push_back(id);
    }
};

vector<int> Utility::userIds;
vector<int> Utility::accountIds;


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
  virtual void setBalance(int balance) = 0;
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
    id = Utility::accIdGenerator();
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
    cout << "Deposit successful! Current balance: " << balance << endl;
    return true;
  };

  bool Withdraw(int amount)
  {

    if (amount > balance)
    {
      cout << "Insufficient balance!" << endl;
      return false;
    }
    else if ((balance - amount) < MIN_BALANCE)
    {
      cout << "Balance goes below minimum! You will be charged Rs 200 penalty\n";
      cout << "Enter 1 to continue or 2 to cancel: ";
      int confirm;
      cin >> confirm;
      if (confirm == 1)
      {
        balance -= (amount + PENALTY);
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
      balance -= (amount);
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
    id = Utility::accIdGenerator();
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
    cout << "Deposit successful! Current balance: " << balance << endl;
    return true;
  };

  bool Withdraw(int amount)
  {
    int choice = 0;
    if ((balance - amount) < MIN_BALANCE)
    {
      cout << "your balance goes less than the minimum balance\n";
      cout << "u will be charged rs200 penalty \n";
      cout << "enter 1 if u want to continue and 2 to stop the withdrawal\n";
      cin >> choice;

      if (choice == 1)
      {
        if (amount > (balance + MIN_BALANCE))
        {
          cout << "Insufficient balance!" << endl;
          return false;
        }
        else
        {
          balance -= (amount + PENALTY);
          cout << "Withdrawal successful! Current balance: " << balance << endl;
          return true;
        }
      }

      else
      {
        cout << "no withdrawal made\n";
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

  void deleteAccounts()
  {
    for (auto acc = accounts.begin(); acc != accounts.end(); acc++)
    {
      delete *acc;
    }
    accounts.clear();
    accounts.shrink_to_fit();
  }

  void setPhone(string phone)
  {
    this->phone = phone;
  }

  string getPhone()
  {
    return phone;
  }

  string getName()
  {
    return name;
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

class Admin
{
public:
  void displayAllUsers(vector<User> &users)
  {
    for (User &user : users)
    {
      user.display();
      cout << "------------------" << endl;
    }
  }

  void displayAllAccounts(vector<User> &users)
  {
    for (User &user : users)
    {
      cout << "User ID: " << user.getId() << endl;
      user.displayAccounts();
      cout << "------------------" << endl;
    }
  }

  void displayAllTransactions()
  {
    ifstream file("transactions.txt");
    string line;
    if (file.is_open())
    {
      while (getline(file, line))
      {
        cout << line << endl;
      }
      file.close();
    }
  }

  bool deleteUser(vector<User> &users, int id)
  {
    for (auto it = users.begin(); it != users.end(); ++it)
    {
      if (it->getId() == id)
      {
        it->display();
        it->deleteAccounts();
        users.erase(it);
        return true;
      }
   
    }

    cout << "User not found!" << endl;
    return false;
  }
  void findUserById(vector<User> &users, int id)
  {
    for (User &user : users)
    {
      if (user.getId() == id)
      {
        user.display();
        return;
      }
    }

      cout << "User not found!" << endl;
    return;
  }

  void findAccById(int id)
  {
    ifstream file("accounts.txt");
    int userId, accId;
    string type;
    int balance;
    if(file.is_open())
    {
    while (file >> userId >> type >> accId >> balance)
    {
      if (accId == id)
      {
        cout << "Account found: " << endl;
        cout << "User ID: " << userId << endl;
        cout << "Account Type: " << type << endl;
        cout << "Balance: " << balance << endl;
        break;
      }
    }
    file.close();
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
      if (file.is_open())
      {
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
  {
    ofstream file("transactions.txt", ios::app);

    if (amount <= 0)
    {
      cout << "Invalid amount!" << endl;
      return false;
    }

    else
    {
      if (acc.Deposit(amount))
      {
        if (file.is_open())
        {
          file << "Deposit of Rs " << amount << " to account ID " << acc.getId() << " at " << Utility::getTime().c_str();
          file.close();
        }
      }
      return true;
    }
  }

  bool withdraw(int amount, Account &acc)
  {
    ofstream file("transactions.txt", ios::app);

    if (amount <= 0)
    {
      cout << "Invalid amount!" << endl;
      return false;
    }
    if (acc.Withdraw(amount))
    {
      if (file.is_open())
      {
        file << "Withdrawal of Rs " << amount << " from account ID " << acc.getId() << " at " << Utility::getTime().c_str();
        file.close();
      }
      return true;
    }
    else
    {
      return false;
    }
    
  }

  bool Transfer(int amount, Account &acc1, Account &acc2)
  {
    ofstream file("transactions.txt", ios::app);
    if (amount <= 0)
    {
      cout << "invalid amount\n";
      return false;
    }
    else
    {
      if (acc1.Withdraw(amount) && acc2.Deposit(amount))
      {
        if (file.is_open())
        {
          file << "Transfer of Rs " << amount << " from account ID " << acc1.getId() << " to account ID " << acc2.getId() << " at " << Utility::getTime().c_str();
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
    id = Utility::userIdGenerator();

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
    { Utility::loadUserId(id);
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
      for (User &u : users)
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
          Utility::loadAccountId(accId);
          break;
        }
      }
    }

    file2.close();
  }

  void updateAccountsFile()
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

  void updateUsersFile(){
      ofstream file("users.txt");
      for (User &user : users)    {
        if (file.is_open())
        {
          file << user.getId() << " " << user.getName() << " " <<user.getPhone() << " " << user.gethashedPassword() << " " << user.getSalt() << endl;
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

    if (user != nullptr && user->getId() > 1000)
    {
      Admin admin;
      adminMenu(admin);
    }
    else if (user != nullptr)
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
          updateAccountsFile();
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
          updateAccountsFile();
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
          updateAccountsFile();
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
            updateAccountsFile();
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
        cout << "choose the receiver by id\n";

        for (User &user : users)
        {
          if (user.getId() != thisID)
          {
            user.display();
            cout << "------------------" << endl;
          }
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
                updateAccountsFile();
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

  void adminMenu(Admin &admin)
  {
    int choice = 0;
    while (choice != 7)
    {
      cout << "1. Display all users" << endl;
      cout << "2. Display all accounts" << endl;
      cout << "3. Display all transactions" << endl;
      cout << "4. Delete a user" << endl;
      cout << "5. Find user by ID" << endl;
      cout << "6. Find account by ID" << endl;
      cout << "7. Logout" << endl;
      cin >> choice;
      switch (choice)
      {
      case 1:
        admin.displayAllUsers(users);
        break;
      case 2:
        admin.displayAllAccounts(users);
        break;
      case 3:
        admin.displayAllTransactions();
        break;
      case 4:
        int id;
        cout << "Enter user ID to delete: ";
        cin >> id;
        if (admin.deleteUser(users, id))
        { 
          updateAccountsFile();
          updateUsersFile();
          cout << "User deleted successfully!" << endl;
        }
        break;
      case 5:
        int userId;
        cout << "Enter user ID to find: ";
        cin >> userId;
        admin.findUserById(users, userId);
        break;
      case 6:
        int accId;
        cout << "Enter account ID to find: ";
        cin >> accId;
        admin.findAccById(accId);
        break;
      case 7:
        cout << "Logout successful!" << endl;
        break;
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

  printf(" %s", Utility::getTime().c_str());

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
