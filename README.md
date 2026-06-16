# 💸 Transaction Minimizer System

> A C++ cash-flow minimization engine that computes the fewest possible settlements
> to resolve group debts — with cross-platform payment routing via 2-hop relay
> when parties share no common payment app.

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue)](https://isocpp.org/)
[![Algorithm](https://img.shields.io/badge/Algorithm-Greedy%20Net%20Flow-orange)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-green)](LICENSE)

---

## 📌 Problem Statement

In a group of N people, each person may owe money to several others.
Naively, this requires O(N²) individual payments. This system computes the
**minimum number of transactions** to settle all debts using a greedy net-flow
balancing algorithm.

**Real-world extension:** Payment platforms (UPI, PayPal, Paytm, etc.) constrain who
can pay whom directly. When two parties share no common platform, the system routes
through a compatible intermediary — a **2-hop relay** — ensuring full settlement
compatibility across any payment ecosystem.

---

## ✨ Features

| Feature | Description |
|---|---|
| Minimum cash flow | Greedy algorithm reduces up to N(N-1) payments to the theoretical minimum |
| Payment mode matching | `std::set_intersection` finds shared platforms between any two parties |
| 2-hop relay routing | Routes through a compatible third party when direct payment is impossible |
| Net position computation | O(N²) net-amount calculation per participant |
| Graph-based modeling | Adjacency matrix representation of the debt network |

---

## 📊 Algorithm

### Greedy Net Flow
1. Compute each person's **net amount** = (total received) − (total owed)
2. Find the **maximum debtor** (most negative) and the **maximum creditor** (most positive) who share a payment platform
3. Settle them with one transaction of `min(|debt|, credit)`
4. Repeat until all net amounts reach zero

### 2-Hop Relay (Cross-Platform)
When the maximum debtor and the best creditor share no common payment platform:
Debtor → [Relay Person] → Creditor
leg 1: Debtor → Relay via shared mode
leg 2: Relay → Creditor via shared mode

The relay bridges the payment gap without requiring a universal platform.

### Complexity
| Operation | Complexity |
|---|---|
| Net amount computation | O(N²) |
| Greedy settlement (per step) | O(N² · M log M) |
| Overall (N steps) | O(N³ · M log M) |
*where M = max payment modes per person*

---

## 🚀 Build & Run

### Prerequisites
- GCC 9+ with C++17 support (Linux / macOS)
- Or MinGW-w64 on Windows

### Compile
```bash
g++ -std=c++17 -O2 -o min-transact TransactionMinimizer.cpp
./min-transact
```

### Sample Session

Enter the number of friends: 4
Enter the names of the friends.
Friend 1: Alice
Friend 2: Bob
Friend 3: Charlie
Friend 4: Dave

Enter the number of transactions.
5
Enter the details of each transaction as stated:
Debtor Friend, Creditor Friend, and Amount
1st transaction: Alice Bob 700
2nd transaction: Alice Charlie 400
3rd transaction: Bob Charlie 200
4th transaction: Dave Bob 300
5th transaction: Dave Charlie 100

Payment modes for Alice (end with -1): UPI PayPal -1
Payment modes for Bob (end with -1): UPI Paytm -1
Payment modes for Charlie (end with -1): PayPal Paytm -1
Payment modes for Dave (end with -1): UPI PayPal -1

The transactions for minimum cash flow are as follows:

Alice pays Rs 800 to Bob via UPI
Alice pays Rs 300 to Charlie via PayPal
Dave pays Rs 400 to Charlie via PayPal

Original transactions: 5
Minimum settlements:   3
Reduction:             40%

---

## 📁 Project Structure

├── TransactionMinimizer.cpp   # Core algorithm: Friend class, greedy engine, relay routing
├── sample1.txt                # 4-person test case (direct settlements)
├── sample2.txt                # 4-person test case (relay routing required)
├── README.md
└── LICENSE

---

## 🔭 Future Enhancements

- [ ] Optimal relay selection — choose best intermediary, not first compatible
- [ ] File-based input (JSON / CSV format)
- [ ] Debt graph visualization (Graphviz DOT export)
- [ ] Google Test unit test suite
- [ ] Web/REST interface for integration into expense-splitting apps

---

## 👤 Author

**Sumit Kumar**
GitHub: [sumitxkothari](https://github.com/sumitxkothari)
LinkedIn: [linkedin/in/sumit kothari](https://www.linkedin.com/in/sumit-kothari-ba18002b7/)
