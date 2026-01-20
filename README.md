# Mischiever 😈
**The Network Swiss Army Knife**

Mischiever is a lightweight, multithreaded offensive cybersecurity tool built in C++. It is designed to demonstrate various "script kiddie" style network attacks and stress-testing methods in a controlled environment.

> **⚠️ Legal Disclaimer:** This tool is created for **educational purposes and authorized testing only**. The author is not responsible for any misuse, damage, or illegal activity caused by this software. Use only on networks you own or have explicit permission to test.

## ⚔️ Capabilities
* **SYN Flood:** Performs high-speed TCP SYN flooding using raw sockets to stress-test target ports.
* **ARP Spoofing:** Implements ARP cache poisoning to intercept traffic (Man-in-the-Middle) between a target and the gateway.
* **Multithreaded Architecture:** All attacks run in non-blocking background threads, allowing the user to manage multiple active attacks simultaneously via the TUI.
* **Attack History:** Integrated SQLite3 database automatically logs all attack details (Target IP, Port, Time, Type) for review.

## 🛠️ Installation

### Prerequisites
System: Linux (Kali/Ubuntu recommended)
```bash
sudo apt-get update
sudo apt-get install libpcap-dev libsqlite3-dev build-essential