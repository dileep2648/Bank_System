# 🏦 Bank Management System

A feature-complete banking system built in C++ demonstrating real-world application of Object-Oriented Programming concepts, security implementation, and file persistence.

---

## 📌 About

This project simulates a backend banking system with user authentication, account management, transactions, and an admin panel. Built iteratively from v1.0 to v2.0 using a professional Git workflow with PR-based development and AI-assisted code review (CodeRabbit).

---

## ✨ Features

### User
- Register and login with hashed + salted password authentication
- Create and close Savings or Current accounts
- Deposit, withdraw, and transfer funds between users
- Minimum balance enforcement with penalty system
- Annual interest simulation for savings accounts
- Real-time transaction logging with timestamps

### Admin
- Separate admin access via controlled credentials
- View all users, accounts, and transaction history
- Delete users with automatic file persistence update
- Search users and accounts by ID

### System
- Collision-free ID generation via centralized Utility registry
- Full file persistence across sessions (users, accounts, transactions)
- Named constants for maintainability (MIN_BALANCE, PENALTY)

---

## 🛠️ Tech Stack

- **Language:** C++
- **Concepts:** OOP, Inheritance, Polymorphism, Abstraction, File I/O
- **Security:** Custom hashing algorithm with salt
- **Storage:** File-based persistence (.txt)
- **Workflow:** Git, GitHub, PR-based development, CodeRabbit AI review

---

## 🏗️ Architecture

```
Bank (main controller)
├── Authentication     → Register, Login, Password verification
├── Security           → Hashing + Salt generation
├── AccManager         → Create and close accounts
├── Transaction        → Deposit, Withdraw, Transfer + logging
├── Admin              → User and account management
├── Utility            → Centralized ID registry + timestamp
├── User               → User data and account ownership
└── Account (abstract)
    ├── SavingsAccount → 3% interest, min balance enforcement
    └── CurrentAccount → Overdraft with penalty system
```

---

## 🚀 How to Run

### Prerequisites
- C++ compiler (g++ recommended)

### Compile
```bash
g++ -o bank main.cpp
```

### Run
```bash
./bank
```

### Admin Access
Add admin credentials manually to `users.txt` with an ID greater than 1000:
```
1001 AdminName 9999999999 <hashedPassword> <salt>
```

---

## 📁 File Structure

```
Bank_System/
├── main.cpp           → Full source code
├── users.txt          → Persistent user data
├── accounts.txt       → Persistent account data
└── transactions.txt   → Transaction history log
```

---

## 📈 Version History

| Version | Changes |
|---------|---------|
| v1.0 | Initial release — core OOP structure |
| v1.1 | Added transfer feature, interest, overdraft and penalty |
| v1.2 | README, bug fixes, edge case handling |
| v2.0 | File persistence, transaction logging, timestamps, Admin class |
| v2.1 | Utility class, collision-free ID registry, named constants |

---

## 🔐 Security

- Passwords are never stored in plain text
- Custom hash function (djb2-based) with random 8-character salt
- Admin credentials manually provisioned — no public registration path
- All IDs generated through a centralized registry to prevent collisions

---

## 📚 Concepts Demonstrated

- Abstract classes and pure virtual functions
- Runtime polymorphism
- Dynamic memory management
- File I/O with persistent state
- Static member variables and methods
- Separation of concerns across classes
- PR-based Git workflow with semantic versioning

---

## 👨‍💻 Author

**Terapati Dileep Kumar**  
1st Year CSE @ CBIT Hyderabad  
[GitHub](https://github.com/dileep2648)
