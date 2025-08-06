# Security-Defenses: DEP and ASLR Exploration

## Overview

This project demonstrates two key modern memory protection technologies:

* **DEP (Data Execution Prevention)** — prevents execution of code from memory areas not explicitly marked as executable.
* **ASLR (Address Space Layout Randomization)** — randomizes the memory layout of a process on each run.

The repository includes code examples and CI workflow automation (via GitHub Actions) that illustrate how DEP and ASLR work on different operating systems: Linux, Windows, and macOS.

## Problem Description

Memory safety is critical in modern software. DEP and ASLR are widely adopted defenses that make exploitation of classic vulnerabilities like buffer overflows much harder.

* **DEP** stops code execution from heap or stack memory unless the memory was explicitly marked as executable. Attempts to execute code from a non-executable region will cause a runtime error (segmentation fault, access violation, etc).
* **ASLR** makes it difficult to predict the location of code, stack, heap, and global data by randomizing their addresses on every program launch, hindering reliable memory corruption exploits.

## Explanation of Key Concepts

### DEP (Data Execution Prevention)

DEP prevents execution of code from memory regions that were not marked executable.
**In practice:**

* If you allocate memory without EXECUTE permissions and try to execute code from there, the OS will block it.
* If you explicitly allocate memory with EXECUTE permissions (e.g., using `mmap` with `PROT_EXEC` on Linux, or `VirtualAlloc` with `PAGE_EXECUTE_READWRITE` on Windows), code execution will be allowed.
* On **macOS**, a stricter policy called W^X (write xor execute) applies: you cannot have a memory page that is both writable and executable. Trying to allocate such memory results in an error.

### ASLR (Address Space Layout Randomization)

ASLR randomizes the placement of key memory regions (code, stack, heap, globals) on each program run. This means addresses of functions and data change every launch, making exploitation far less predictable and reliable.

## Example Output

### DEP Example

**Ubuntu:**

```
Function pointer address: 0x7f81a9aea000
Executed result: 42
```

(Memory was allocated with EXECUTE; DEP allows execution.)

**macOS:**

```
mmap: Permission denied
```

(W^X/DEP prohibits simultaneously WRITE+EXECUTE pages, so allocation fails.)

### ASLR Example

**Multiple runs:**

```
--- Run 1 ---
Address of main(): 0x10011efec
Address of foo(): 0x10011efe8
Address of global_var: 0x100124000
Address of stack_var: 0x16fce2478
Address of heap_var: 0x6000015e0040

--- Run 2 ---
Address of main(): 0x1046d2fec
Address of foo(): 0x1046d2fe8
Address of global_var: 0x1046d8000
Address of stack_var: 0x16b72e478
Address of heap_var: 0x6000015c0040

--- Run 3 ---
Address of main(): 0x100d46fec
Address of foo(): 0x100d46fe8
Address of global_var: 0x100d4c000
Address of stack_var: 0x16f0ba478
Address of heap_var: 0x6000039a0040
```

## Explanation of Output

* **DEP:**

  * If memory is allocated with EXECUTE permissions (`PROT_EXEC` / `PAGE_EXECUTE_READWRITE`), code execution from it succeeds (Ubuntu, Windows).
  * If W^X or stricter DEP is enforced (macOS), allocation of WRITE+EXECUTE memory fails.
* **ASLR:**

  * On each program run, the addresses of code, global data, stack, and heap change, showing that ASLR is working.

## How to Compile and Run

### 1. Clone the Repository

```sh
git clone https://github.com/LyudmilaKostanyan/Security-Defenses.git
cd Security-Defenses
```

### 2. Build

Use CMake:

```sh
cmake -S . -B build
cmake --build build
```

Make sure you have CMake and a suitable compiler installed (gcc/g++ for Linux, clang/clang++ for macOS, MSVC or MinGW for Windows).

### 3. Run

#### For Linux/macOS:

```sh
cd build
./dep       # DEP demonstration
./aslr      # ASLR demonstration
```

#### For Windows:

```sh
cd build/Release
dep.exe
aslr.exe
```
