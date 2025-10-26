# 🏦 Bank Management System

## 📘 Abstract

The **Bank Management System** is a comprehensive **C-based console application** designed to simulate essential banking operations such as **account creation**, **fund management**, **interest calculation**, **loan processing**, and **transaction tracking**.
It ensures data persistence through **file handling** and maintains security using **user authentication**.
This project demonstrates core C programming concepts applied in real-world scenarios while providing an efficient, user-friendly banking simulation.

---

## 🏁 Introduction

### 🔹 Background

Modern banks handle massive amounts of data daily. Manual or outdated systems often lead to **errors**, **delays**, and **data inconsistency**.
This project addresses those issues by implementing an **automated, centralized, and secure** banking system that handles accounts, transactions, and records efficiently.

### 🔹 Problem Statement

Many banking systems lack **automation**, **security**, and **persistent data storage**.
This project fills that gap by developing a secure, file-based management system to handle all essential banking tasks with accuracy and reliability.

### 🔹 Objectives

* Efficient **account creation**, **updating**, and **management**
* Simplified **deposit**, **withdrawal**, and **balance inquiry**
* Implementation of **security measures** for data protection
* Automated **interest and loan management**
* Reduced manual workload and improved efficiency

---

## 📚 Literature Review

### 🔹 Related Work

Existing academic banking systems often focus on isolated functionalities such as deposits or transactions.
Few integrate **interest**, **loan**, and **authentication** features together in one **file-based C application**.

### 🔹 Gap Analysis

Most student-level projects lack:

* **Persistent file storage**
* **Integrated functionalities** (interest, loans, authentication)
* **Security features**

This project bridges that gap with a **complete, secure, and persistent** file-based C solution.

---

## ⚙️ Methodology

### 🔹 Approach — Agile Development Framework

* **Iterative Sprints:** Each 2-week sprint focuses on key modules (e.g., account creation, authentication).
* **User Feedback:** Regular validation by sample users.
* **Adaptive Planning:** Continuous improvement based on test feedback.

**Key Practices:**

* Daily standups to fix blockers (e.g., file I/O errors)
* Sprint retrospectives to review test outcomes

**User Stories:**

* *As a customer, I want to deposit funds so that my balance updates instantly.*
* *As an admin, I need to view transaction logs for auditing.*

### 🔹 Tools and Technologies

| Category             | Tools/Technologies                              |
| -------------------- | ----------------------------------------------- |
| Programming Language | C                                               |
| Storage              | Text files (`accounts.txt`, `transactions.txt`) |
| Compiler             | GCC                                             |
| IDE                  | VS Code                                         |
| Documentation        | Doxygen                                         |

### 🔹 Implementation Workflow

1. **Requirement Analysis** – Mapping features to code modules
2. **Prototyping** – Building and testing CLI interface
3. **Validation** – Unit testing using `assert()`
4. **Integration** – Combining modules with robust error handling

---

## 🔧 Functional Requirements

1. **Account Creation:** Unique ID, user details, initial deposit
2. **User Authentication:** Secure login via account number and PIN
3. **Deposit Funds:** Real-time balance update and transaction record
4. **Withdraw Funds:** Validation for sufficient balance
5. **Balance Inquiry:** Instant balance check
6. **Update Account:** Modify personal info after authentication
7. **Transaction History:** View all transactions with timestamps
8. **Interest Calculation:** Automatic monthly interest addition
9. **Loan Management:** Apply, approve/reject, track loan details
10. **Account Closure:** Withdraw remaining funds and mark inactive

---

## 🧠 Non-Functional Requirements

### 🔐 Security

* AES-256 encryption for sensitive data
* Role-based access (Customer/Admin)
* Session timeout and immutable audit logs

### ⚡ Performance

* Response time < 0.3s for balance check
* Batch interest calculation for 10,000 accounts < 5 mins
* ≤70% CPU usage under peak load

### 🧱 Reliability

* 99.9% system uptime
* Transaction rollback on crashes
* Full restore within 1 hour from last backup

### 🧩 Usability

* Accessible CLI interface (supports screen readers)
* Learn basic transactions within 2 minutes
* Clear, descriptive error messages

### 📈 Scalability

* Support for 1M+ accounts
* Modular design for easy feature addition

### 🧹 Maintainability

* Complies with **MISRA C** coding standards
* ≥80% documentation coverage

---

## 🧪 Testing

* **Unit Testing:** Core modules (withdrawal, deposit, interest)
* **Integration Testing:** File I/O, transaction history consistency
* **Security Testing:** Unauthorized access handling
* **Performance Testing:** Stress test on file read/write operations

---

## 🗂️ File Structure

```
BankManagementSystem/
│
├── src/
│   ├── main.c
│   ├── account.c
│   ├── transaction.c
│   ├── loan.c
│   └── security.c
│
├── data/
│   ├── accounts.txt
│   └── transactions.txt
│
├── docs/
│   └── Doxyfile
│
├── README.md
└── Makefile
```

---

## 🚀 How to Run

```bash
# Compile the program
gcc src/main.c -o bank

# Run the executable
./bank
```

---

## 🧑‍💻 Contributors

* **Saheen Alam Shuvo** – Developer & Designer
* Special thanks to instructors and peers for guidance.

---

## 🏆 License

This project is developed for **educational purposes** under the MIT License.
Free to use, modify, and distribute with proper credit.

Thanks
