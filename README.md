<div align="center">

# Needle

![C++](https://img.shields.io/badge/C++-ptrace_System_Call-blue?style=flat&logo=c%2B%2B)
![License](https://img.shields.io/badge/License-MIT-green)
![Type](https://img.shields.io/badge/Type-Process_Injection-red)

<p>
  <strong>A Proof-of-Concept (PoC) Linux tool for injecting arbitrary shellcode into a running process using ptrace.</strong>
</p>

[Report Bug](https://github.com/egetones/needle/issues) · [Request Feature](https://github.com/egetones/needle/issues)

</div>

---

## Description

**Needle** demonstrates a fundamental post-exploitation technique: **Process Injection**. Written in C++, it utilizes the `ptrace()` system call (the same mechanism used by debuggers like GDB) to attach to a running process, modify its Instruction Pointer (RIP), and inject custom machine code (shellcode) directly into its memory space.

This technique allows malware to execute within the context of a legitimate process (like `bash` or `firefox`), aiding in defense evasion and persistence.

### Key Technical Concepts

  **Process Injection:** Overwriting the memory and execution flow of a live program.
  **ptrace API:** Direct interaction with the Linux kernel to control a process's registers (RIP) and memory (`PTRACE_POKETEXT`).
  **Assembly Payload:** Utilizes raw shellcode bytes (e.g., executing `/bin/sh`) for payload delivery.
  **Stealth Analysis:** Understanding how fileless malware operates within a system.

---

## Usage

### 1. Compile the Tools
The project consists of three files: `target.c` (The Victim), `shellcode.h` (The Payload), and `needle.cpp` (The Injector).
```bash
make
# This compiles 'target' and 'needle'
```
### 2. Run the Victim
Open a terminal and start the vulnerable application. Note its PID (Process ID).
```bash
./target
# Output: Target Process PID: 12345
```
### 3. Inject the Shellcode
Open a second terminal and run the injector, passing the victim's PID. Requires root privileges.
```bash
sudo ./needle 12345
```
### 4. Observe the Takeover
Check the first terminal (where ./target was running). The simple counting program should stop and be replaced by a shell prompt (sh-5.2$), indicating successful code injection and remote execution.

---

## ⚠️ Critical Safety Disclaimer
Needle is for educational and research purposes only. This tool demonstrates a severe attack vector (Remote Code Execution and Defense Evasion).
Do not use this tool on any system you do not explicitly own or have permission to audit.
The injection process is destructive and will likely crash the target process after the shellcode executes.
Use only on isolated virtual machines for testing.

---

## License
Distributed under the MIT License. See LICENSE for more information.
