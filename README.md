# Mischiever 😈
### The Network Swiss Army Knife

**Mischiever** is a modular, multithreaded offensive cybersecurity **platform** built in C++ from scratch. Unlike script-based tools, Mischiever is engineered as a unified framework where attack modules share a central "brain" (Session State) to coordinate network manipulation.

It constructs network packets byte-by-byte using raw sockets, giving the user complete control over Layer 2 (Ethernet) and Layer 3 (IP) headers to bypass standard security filters.

---

### ⚠️ Legal Disclaimer
**This tool is created for educational purposes and authorized testing only.**
The author is not responsible for any misuse, damage, or illegal activity caused by this software. Use only on networks you own or have explicit permission to test.

---

### 🏛️ Platform Architecture
Mischiever is not just a collection of scripts; it is an Object-Oriented C2 (Command & Control) framework designed for scalability and state persistence.

* **🧠 Session-Based State Management:** Implements a central `Session` structure ("The Brain") that holds global network truths (Target IP, MAC, Gateway). Configuration is set once and instantly available to all loaded modules.
* **🔌 Polymorphic Design:** Utilizes an abstract `AttackModule` base class to enforce a unified interface (`run`, `stop`, `get_name`) across all tools. This allows the core engine to manage disparate attacks (ARP, TCP, ICMP) uniformly.
* **📊 Real-Time Dashboard:** Features a dynamic TUI (Text User Interface) that displays live interface status and target configurations at a glance.
* **🧵 Non-Blocking Execution:** Attacks run in detached background threads (`std::thread`), allowing the user to navigate the menu, reconfigure targets, or launch concurrent vectors without interrupting active operations.

---

### ⚔️ Capabilities

#### 1. ARP Cache Poisoning (Stealth Mode)
* **Technique:** Implements a **Unicast ARP Reply** injection to bypass modern switch security (Dynamic ARP Inspection) and virtualization filters (VMware/VirtualBox).
* **Impact:** Successfully intercepts traffic (Man-in-the-Middle) by poisoning the victim's ARP cache, mapping the Router's IP to the Attacker's MAC address.
* **Automation:** Includes an auto-discovery engine that resolves the Target MAC and Gateway IP automatically via ARP probes.

#### 2. SYN Flood (DoS)
* **Technique:** Uses **Raw Sockets (`IP_HDRINCL`)** to manually construct TCP headers with randomized spoofed Source IPs.
* **Impact:** Exhausts the target's TCB (Transmission Control Block) memory by flooding it with "HALF_OPEN" connections, rendering the service unreachable for legitimate users.

#### 3. ICMP Ping Flood (DoS)
* **Technique:** Generates a high-throughput stream of ICMP Echo Request (Type 8) packets with custom payloads.
* **Impact:** Consumes target CPU and bandwidth by forcing the OS kernel to process and reply to every packet interrupt.

---

### 💾 Tech Stack & Features
* **Language:** C++ (OOP, STL, Threading)
* **Networking:** Native Linux Raw Sockets (`SOCK_RAW`, `AF_PACKET`)
* **Database:** SQLite3 (Automatic logging of all attack sessions for auditing)
* **System:** Dynamic parsing of `/proc/net/route` and `ioctl` for hardware detection.

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