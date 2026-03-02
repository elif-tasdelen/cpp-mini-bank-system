# C++ Mini Bank System

This is a single-file console-based banking system written in C++ to practice core Object-Oriented Programming principles.

The system allows users to create Savings and Checking accounts, deposit and withdraw money, transfer funds between accounts, and apply end-of-month operations such as interest (Savings) or overdraft fees (Checking). It also prevents duplicate account IDs and includes basic exception handling for invalid operations.

The design is centered around an abstract base class `Account`, from which `SavingsAccount` and `CheckingAccount` are derived. Runtime polymorphism is implemented using virtual functions, and all accounts are stored in a `vector<unique_ptr<Account>>` to ensure dynamic behavior and safe memory management without manual `new/delete`.

This project demonstrates:
- Abstract classes
- Inheritance
- Runtime polymorphism
- Encapsulation
- Virtual destructors
- Smart pointers (`std::unique_ptr`)
- Dynamic containers (`std::vector`)



Example usage:

1
101 1000 0.05
2
202 300 500
7
101 202 200

This project was built to strengthen practical understanding of C++ OOP design and memory-safe programming.