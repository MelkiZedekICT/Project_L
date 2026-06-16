# Project L 
Your trusted Kernel Operator
> **A custom x86 operating system built entirely from Bottom of OS Ocean.** > No standard libraries, no underlying host, no safety nets. Just raw C++ and Assembly.

Project L is a local-first, freestanding kernel environment. It was built to achieve uncompromised access to the hardware—running in Ring 0 (literal God Mode for the CPU). It serves as a foundation for hardware security experiments, Forcing driver development, and absolute control over system memory. 

*If you are a technical recruiter reading this: Yes, I wrote my own `memset` and `memcpy`. Yes, this project taught me more about computer architecture and hardware exception handling than any textbook ever could.*

---

### 🚀 The Tech Stack
* **Languages:** C++ (Freestanding / `-fno-exceptions`), x86 Assembly (NASM)
* **Architecture:** 32-bit x86 (`elf_i386`)
* **Tooling:** GNU Compiler Collection (`g++`, `ld`), GNU Make / Bash Scripts
* **Emulation:** QEMU (`qemu-system-i386`)

---

### 🛡️ Core Features & Engineering Feats

Building an OS means you can't rely on things you take for granted—like the `printf` function, or the keyboard working, or the CPU not panicking. Here is what is running under the hood:

* **Hardware Security Gates (IDT):** A custom Interrupt Descriptor Table. It actively traps CPU exceptions (like Page Faults or Divide-by-Zero errors), halting execution before a catastrophic triple-fault takes down the machine.
* **Hardware Intercepts (PIC Remapping):** The Programmable Interrupt Controllers are manually remapped and masked to isolate specific hardware signals, keeping the CPU stable and focused.
* **PS/2 Keyboard Driver:** A low-level driver listening directly on hardware Port `0x60`, translating raw scancodes into human-readable characters on the fly.
* **Direct VGA Buffer Manipulation:** Text rendering built from the ground up by writing hex values directly to memory address `0xB8000`.

---
🗺️ Future Roadmap
Project L is an evolving foundation for low-level engineering and hardware research. Upcoming milestones include:

* The Telemetry Bridge: Implementing a COM1 Serial Port driver to transmit crash data and telemetry out of the virtual machine to a host terminal.

* Interactive Command Shell: Upgrading the keyboard state-machine to handle Shift, Enter, and Backspace for true CLI execution.

* Embedded Security Interfacing: Expanding hardware monitoring capabilities to interface directly with ATmega32 microcontrollers for external anomaly and hardware trojan detection.

🤝 Contributing
Want to dive into the OS ocean? Fork it, break it, fix it, and open a Pull Request. Developers are absolutely welcome to pull this repository and contribute.

Whether you are porting a legacy floppy disk driver, optimizing the VGA rendering buffer, or building out the COM1 Telemetry Bridge, all PRs are reviewed and welcomed.

The Workflow:

Fork the Project

Create your Feature Branch (git checkout -b feature/TerminalUpgrade)

Commit your chaos (git commit -m 'Added Backspace support')

Push to the Branch (git push origin feature/TerminalUpgrade)

Open a Pull Request

Maintained by Bolli Yaswanth Melki
### 💻 Quick Start

Defaults work out of the box. You just need a Linux environment (or WSL) to compile and emulate.

```bash
# 1. Clone the repository
git clone [https://github.com/MelkiZedekICT/Project_L.git](https://github.com/MelkiZedekICT/Project_L.git)
cd Project_L

# 2. Build the OS binary and launch the QEMU Emulator
./build.sh

