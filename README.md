# Mischiever 😈
### The Network Swiss Army Knife

**Mischiever** is a modular, multithreaded offensive cybersecurity **platform** built in C++ from scratch. Unlike script-based tools, Mischiever is engineered as a unified framework where attack modules share a central "brain" (Session State) to coordinate the entire kill chain—from reconnaissance to execution.

It constructs network packets byte-by-byte using raw sockets, giving the user complete control over Layer 2 (Ethernet) and Layer 3 (IP) headers to bypass standard security filters.

---

### ⚠️ Legal Disclaimer
**This tool is created for educational purposes and authorized testing only.**
The author is not responsible for any misuse, damage, or illegal activity caused by this software. Use only on networks you own or have explicit permission to test.

---

### 🏛️ Platform Architecture
Mischiever is not just a collection of scripts; it is an Object-Oriented C2 (Command & Control) framework designed for scalability and state persistence.

* **🧠 Session-Based State Management:** Implements a central `Session` structure that holds global network truths. Configuration is set once (manually or via auto-discovery) and is instantly available to all loaded modules.
* **🔄 Background Service Architecture:** Supports **"Toggleable Services."** Critical infrastructure attacks (like ARP Spoofing) run in detached background threads, allowing the user to simultaneously operate foreground intelligence tools (like the Sniffer).
* **🔌 Polymorphic Design:** Utilizes an abstract `AttackModule` base class to enforce a unified interface (`run`, `stop`, `get_name`) across all tools.
* **📊 Real-Time Dashboard:** Features a dynamic TUI (Text User Interface) that displays live interface status and target configurations at a glance.

---

### ⚔️ Capabilities

#### 1. Network Reconnaissance (Scanner)
* **Technique:** Implements a **Parallelized Ping Sweep** combined with Kernel ARP Table harvesting (`/proc/net/arp`).
* **Performance:** Maps an entire /24 subnet (254 hosts) in under 2 seconds without external dependencies like Nmap.
* **Integration:** Embedded directly into the configuration menu via the `find` command.

#### 2. Traffic Intelligence (Sniffer)
* **Technique:** Uses `libpcap` to inspect real-time network traffic on the wire.
* **Smart Filtering:** Automatically detects and **highlights** traffic belonging to the configured Target IP in <span style="color:yellow">Yellow</span>, visually isolating the victim's activity from background noise.
* **Usage:** Designed to run in tandem with ARP Spoofing to verify Man-in-the-Middle interception.

#### 3. ARP Cache Poisoning (Stealth Mode)
* **Technique:** Implements a **Unicast ARP Reply** injection to bypass modern switch security (Dynamic ARP Inspection) and virtualization filters (VMware/VirtualBox).
* **Impact:** Intercepts traffic (Man-in-the-Middle) by mapping the Router's IP to the Attacker's MAC address.
* **Feature:** Can be toggled as a **Background Service**, maintaining the poison loop while the user navigates other menus.

#### 4. SYN Flood (DoS)
* **Technique:** Uses **Raw Sockets (`IP_HDRINCL`)** to manually construct TCP headers with randomized spoofed Source IPs.
* **Impact:** Exhausts the target's TCB (Transmission Control Block) memory by flooding it with "HALF_OPEN" connections.

#### 5. ICMP Ping Flood (DoS)
* **Technique:** Generates a high-throughput stream of ICMP Echo Request (Type 8) packets with custom payloads.
* **Impact:** Consumes target CPU and bandwidth by forcing the OS kernel to process every packet interrupt.

---

### 🧠 Smart Configuration Features
Mischiever replaces manual lookups with context-aware automation:
* **Auto-Gateway Detection:** Parses `/proc/net/route` to identify the default gateway (Destination `0.0.0.0`) automatically.
* **MAC Resolution:** Instantly resolves MAC addresses for Targets and Gateways using local ARP lookups.
* **Lazy Config:** Type `find` in any IP field to launch the scanner or gateway detector instantly.

---

### 💾 Tech Stack
* **Language:** C++ (OOP, STL, Threading)
* **Networking:** Native Linux Raw Sockets (`SOCK_RAW`, `AF_PACKET`) & `libpcap`
* **Database:** SQLite3 (Automatic logging of all attack sessions)
* **System:** Direct parsing of Kernel interfaces (`/proc/net/*`) and `ioctl`.

---

### 🛠️ Installation & Usage

**Prerequisites:**
* System: Linux (Kali Linux, Ubuntu, or Arch Linux)
* Privileges: Root (Required for Raw Socket access)

```bash
# 1. Install Dependencies
# Debian/Kali/Ubuntu:
sudo apt-get update
sudo apt-get install libpcap-dev libsqlite3-dev build-essential

# 2. Compile
make

# 3. Run
sudo ./mischiever