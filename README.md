# CPU Scheduler Simulator

A C++ console application that simulates classical CPU scheduling algorithms.

### Overview
The simulator reads process data from an input file, organizes processes into queues, and executes scheduling algorithms while calculating waiting times.

### Algorithms Implemented
- **FCFS** – First Come First Serve
- **SJF** – Shortest Job First (Non-Preemptive)

### Features
- Supports multiple queues
- Computes waiting time per process
- Computes average waiting time per queue
- Handles CPU idle time correctly
- Dynamic memory management using linked lists

### Process Structure
Each process contains:
- `procId` – Unique process ID (auto-generated)
- `execTime` – Execution (burst) time
- `priority` – Priority value (read but not used in scheduling)
- `arrival` – Arrival time

### Data Structures
- Singly linked list for process storage
- Linked list of queues
- Dynamic memory allocation
- Custom node structure tracking waiting time and completion flag
