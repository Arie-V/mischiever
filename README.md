# Mischiever 😈
### The Network Swiss Army Knife

**Mischiever** is a lightweight, multithreaded offensive cybersecurity tool built in C++ from scratch. Unlike script-based tools, Mischiever constructs network packets byte-by-byte using raw sockets, giving the user complete control over Layer 2 (Ethernet) and Layer 3 (IP) headers.

It is designed to demonstrate low-level network manipulation, protocol stress-testing, and security filter evasion in a controlled environment.

---

### ⚠️ Legal Disclaimer
**This tool is created for educational purposes and authorized testing only.**
The author is not responsible for any misuse, damage, or illegal activity caused by this software. Use only on networks you own or have explicit permission to test.

---

### ⚔️ Capabilities

#### 1. ARP Cache Poisoning (Stealth Mode)
* **Technique:** Implements a **Unicast ARP Reply** attack to bypass modern switch security (Dynamic ARP Inspection) and virtualization filters (VMware/VirtualBox).
* **Impact:** successfully intercepts traffic (Man-in-the-Middle) by tricking the victim into mapping the Router's IP to the Attacker's MAC address.
* **Automation:** Auto-detects the target's MAC address and the local Gateway IP, falling back to heuristic detection in Host-Only networks.

#### 2. SYN Flood (DoS)
* **Technique:** Uses **Raw Sockets (`IP_HDRINCL`)** to generate thousands of TCP packets with randomized spoofed Source IPs.
* **Impact:** Exhausts the target's TCB (Transmission Control Block) memory by leaving connections in a "HALF_OPEN" state, preventing legitimate users from connecting.

#### 3. ICMP Ping Flood (DoS)
* **Technique:** Blasts the target with ICMP Echo Request (Type 8) packets at maximum throughput.
* **Impact:** Consumes target CPU and bandwidth by forcing the OS to process and reply to every packet.

---

### 🏛️ Architecture & Features
* **Raw Socket Engineering:** All packet headers (Ethernet, ARP, IP, TCP, ICMP) are manually constructed in C++ structs, allowing for precise header manipulation.
* **Multithreaded Core:** Attacks run in non-blocking background threads (`std::thread`), allowing the user to manage multiple simultaneous vectors via the TUI.
* **SQLite Logging:** Integrated database automatically logs every attack session (Target IP, Source IP, Timestamp, Type) for auditing purposes.
* **Dynamic Analysis:** Includes helper functions to parse `/proc/net/route` and query kernel interfaces (`ioctl`) for real-time network configuration.

---

### 🛠️ Installation & Usage

**Prerequisites:**
* System: Linux (Kali Linux or Ubuntu recommended)
* Privileges: Root (Required for Raw Socket access)

```bash
# 1. Install Dependencies
sudo apt-get update
sudo apt-get install libpcap-dev libsqlite3-dev build-essential

# 2. Compile
make

# 3. Run
sudo ./mischiever