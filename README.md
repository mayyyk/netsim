# NetSim
> A discrete-event logistics network simulator with graph-based modeling and integrity validation.

---

## About

**NetSim** is a C++17-based network simulation engine that models discrete-event logistics systems through configurable node graphs.  The system simulates package flow through production networks composed of loading ramps, processing workers, and storehouses, enabling validation of network topology and throughput analysis.

The simulator addresses the need for **deterministic behavior modeling** in supply chain networks, providing round-based execution, queue management (FIFO/LIFO), and comprehensive network integrity checks to detect dangling references or incomplete paths before runtime.

**Core Design Principles:**
- **Precise** — Round-based deterministic logic
- **Solid** — Pre-simulation integrity verification
- **Scalable** — Polymorphism and design patterns throughout

---

## Tech Stack

- **C++17** — Core language with modern STL features
- **CMake** — Build system configuration
- **Google Test (GTest)** — Unit testing framework
- **Clang-Format** — Code style enforcement
- **Polymorphic design patterns** — Interface-driven architecture (`IPackageReceiver`, `PackageSender`)

---

## Key Features

- **File-Based Network Definition** — Load network topology from structured text files with built-in parsing and error handling
- **Graph-Based Simulation** — Model logistics networks as directed graphs with ramps, workers, and storehouses as nodes
- **Integrity Validation** — Pre-simulation consistency checks ensure all ramps have valid storehouse paths (no orphaned nodes)
- **Flexible Queue Strategies** — Support for FIFO/LIFO processing policies per worker node
- **Comprehensive Reporting** — Generate simulation state dumps and network structure reports to `.txt` files

---

## Application Functionality

### 1. Load Network Structure from File

NetSim parses structured text files with automatic error handling.  

**File Format Rules:**
- Lines starting with `;` are comments (ignored)
- Empty lines are ignored
- Valid lines follow the format: `TAG key=value [key=value ...]`
- Elements must appear in order: **ramps → workers → storehouses → links**

**Supported Tags:**

```
LOADING_RAMP id=<ramp-id> delivery-interval=<interval>
```
- `delivery-interval`: Number of rounds between package deliveries

```
WORKER id=<worker-id> processing-time=<time> queue-type=<FIFO|LIFO>
```
- `processing-time`: Rounds required to process one package
- `queue-type`: Queue strategy (FIFO or LIFO)

```
STOREHOUSE id=<storehouse-id>
```
- Terminal nodes for package storage

```
LINK src=<node-type>-<node-id> dest=<node-type>-<node-id>
```
- `node-type`: `ramp`, `worker`, or `storehouse`
- `node-id`: Unique identifier within that node type

**Example Network File:**
```
; Production line example
LOADING_RAMP id=1 delivery-interval=3
WORKER id=1 processing-time=2 queue-type=FIFO
WORKER id=2 processing-time=1 queue-type=LIFO
STOREHOUSE id=1

LINK src=ramp-1 dest=worker-1
LINK src=worker-1 dest=worker-2
LINK src=worker-2 dest=storehouse-1
```

---

### 2. Programmatic Network Modification

Dynamically modify network structure through the Factory API: 
- Add/remove nodes (ramps, workers, storehouses)
- Create/destroy connections between nodes
- Export modified networks to new structure files

See [`FACTORY_MANUAL.md`](FACTORY_MANUAL.md) for complete API documentation.

---

### 3. Simulation Execution & Reporting

**Pre-Simulation Phase:**
1. **Integrity Check** — Validates all ramps have paths to at least one storehouse
2. Network becomes **immutable** once simulation starts

**Per-Round Execution:**
1. **Delivery** — Ramps generate packages based on delivery intervals
2. **Hand-off** — Packages instantly transfer between nodes
3. **Processing** — Workers advance package processing by one round
4. **Reporting** — Output network state or structure to `.txt` files

**Simulation Termination:**  
Completes after specified number of rounds. 

**Usage Example:**
This code in [`main.cpp`](main.cpp) loads factory structure from file and generates structure report to the console.
```cpp
    std::ifstream structure_file("network_structure.txt");

    if (!structure_file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Factory factory;

    try {
        factory = load_factory_structure(structure_file);
    } catch (const std::exception &e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        return 1;
    }

    generate_structure_report(factory, std::cout);
```

---

## Installation

### Prerequisites
```bash
# CMake 3.10+, C++17 compiler, GTest
sudo apt install cmake g++ libgtest-dev  # Debian/Ubuntu
```

### Build
```bash
git clone https://github.com/mayyyk/netsim.git
cd netsim
mkdir build && cd build
cmake ..
make
```

### Run
```bash
# Execute simulation
./netsim_app

# Run tests
./netsim_tests
```

---

## Node Behavior Specifications

### Products (Packages)
- Unique auto-incremented ID assignment
- Immutable once created

### Loading Ramps
- Generate packages at fixed intervals (`delivery-interval`)
- Single output connection to workers or storehouses

### Workers
- **Queue Management:** Accept incoming packages into FIFO/LIFO queues
- **Processing:** Each package requires `processing-time` rounds to complete
- **Future Enhancement:** Per-ramp weighted routing

### Storehouses
- Terminal nodes for package accumulation
- Support for custom storage policies (extensible)

---

## Project Structure
```
netsim/
├── include/              # Header files
│   ├── factory.hpp       # Network container and manipulation
│   ├── nodes. hpp         # Ramp, Worker, Storehouse definitions
│   ├── package.hpp       # Product/package logic
│   ├── simulation.hpp    # Round-based execution engine
│   ├── reports.hpp       # Output formatting
│   └── storage_types.hpp # Queue implementations (FIFO/LIFO)
├── src/                  # Implementation files
├── test/                 # Google Test suite
├── CMakeLists.txt        # Build configuration
└── network_structure.txt # Sample network definition
```

---

## Target Users

1. **Programmers (API Users)**  
   Embed NetSim in larger systems via clean public interfaces for network construction, simulation execution, and report generation.

2. **Analysts**  
   Define networks via text files, run simulations, and analyze output reports without writing code.

---

## Contributing

This project follows strict coding standards:
- **Const correctness** enforced throughout
- **Doxygen-style** code documentation
- **Compilation flags:** `-Wall -Wextra -pedantic -Werror`
- Unit tests for all non-trivial logic

**Commit Message Conventions:**  
`feat:`, `fix:`, `refactor:`, `docs:`, `test:`

**Workflow:**
```bash
git checkout main
git pull origin main
git checkout -b feature/your-feature-name
```

---

## Development Roadmap

- [x] Products and storage types
- [x] Node implementations (Ramp, Worker, Storehouse)
- [x] Factory + I/O parsing
- [x] Simulation engine
- [ ] Enhanced reporting (JSON/CSV export)
- [ ] Weighted routing for workers
- [ ] Interactive visualization

---

**License:** MIT
**Author:** [@mayyyk](https://github.com/mayyyk)  
