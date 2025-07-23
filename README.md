# Concepts in Programming Languages – Assignments 🧠

This repository contains two C programming assignments implemented during the **Concepts in Programming Languages** course in the second year of B.Tech in Computer Science and Engineering.

## 📁 Contents

### 1. `HeapManagement.c` – Simulated Dynamic Memory Allocator

A custom implementation of a heap memory allocator using manual memory management concepts. This simulation includes:

- Manual memory allocation and deallocation
- Free block merging
- Metadata-based block tracking
- Simulated 100-byte heap using static memory
- Functions: `heap_init()`, `alloc_memory()`, `dealloc_memory()`, `merge_free_blocks()`, `heap_dump()`

📌 **Key Concepts**:
- Memory simulation
- Pointer arithmetic
- Custom memory block headers
- Fragmentation and coalescing

---

### 2. `SymbolTable.c` – Multi-Scope Symbol Table

An implementation of a symbol table with support for nested scopes. It processes input from a file (`input.txt`) with commands to:

- Enter/exit scopes (`begin`, `end`)
- Assign variables (`assign <name> <value>`)
- Print variable values (`print <name>`)

📌 **Key Concepts**:
- Hash-based symbol table
- Scope chaining
- Variable shadowing
- Command parsing

---

## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/your-repo-name.git
   cd your-repo-name
   ```
2. Compile and run any file:
   ```bash
   gcc HeapManagement.c -o heap
   ./heap
   gcc SymbolTable.c -o symbol
   ./symbol
   ```
Make sure to include a valid input.txt file in the root for SymbolTable.c.

## 🛠️ Requirements
GCC compiler
