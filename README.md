# Lightweight Container Sandbox in C

A minimal Linux container runtime written in C.  
This project demonstrates the use of namespaces, chroot, and resource limits  
to isolate processes — similar to a very simplified version of Docker.

## Build

```bash
mkdir build && cd build
cmake ..
make
# Sandbox-Project
