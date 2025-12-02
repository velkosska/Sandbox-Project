#Lightweight Linux Container Sandbox

This project was really inspired by our topics learned in Computer Architecture,OS and Networking and Cloud Computing. We saw how C is the backbone of all crucial technologies, so we decided to try and recreate one of them. We had so much fun building and debugging:’’)! A video that we stumbled upon during the development which we are linking here: https://www.youtube.com/watch?v=8fi7uSYlOdc was an enormous help. At the moment we thought we knew everything, this taught us 10x more. Now let’s move on to our amazing creation!

We implemented a Mini Bash Container which is a small but functional sandbox that isolates processes using:
clone() to create a new process in isolated namespaces
chroot() and a minimal root filesystem to restrict file access
/proc mounting to provide process introspection
setrlimit() for memory constraints
Threaded execution (pthread) to launch multiple sandboxes concurrently
A simple TCP server for remote control (RUN /bin/bash from a network client).
You know what this means? You can:
./sandbox run /bin/bash
./sandbox help
./sandbox run-parallel /bin/echo /bin/date /bin/hostname
./sandbox server
./sandbox run-parallel /bin/sleep 3 /bin/sleep 1

Our goal was not to make a Docker clone, but to really understand its structure to make kernel isolation visible, tangible, and hackable.
With syscalls such as clone() and unshare(), and thanks to your feedback we realized how making a secure sandbox is super challenging.
Ours is minimal and not so secure, and it’s because it runs as root and relies on chroot() instead of full user namespaces, a malicious process could potentially escape the sandbox.
Real containers like Docker use some preventative measures such as:
Seccomp filters to restrict allowed syscalls, capabilities in order to drop dangerous privileges, cgroups to enforce resource quotas ,they also enforce kernel-level access control policies. No jailbreak on their watch!!




Design Choices Justified:
We built it on top of a Docker container, why?To learn in an authentic Linux environment while ensuring our experiments never get even close to the host system.
We used the clone() syscall instead of fork() to manually create new namespaces. The flags CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS allow hostname, process tree, and mount isolation also each sandbox runs as PID 1 inside its namespace, similar to how Docker containers spawn init processes.
We simulated the rootfs(empty folders for most things) containing /bin/bash, libraries, and optionally BusyBox utilities for additional features. The sandbox changes its root with chroot() and chdir() to the new filesystem which basically simulates a separate OS root.
Memory limits are set with setrlimit to restrict the virtual memory available to sandboxed processes.
/proc is mounted inside the chroot to enable tools like ps or top and before we exit the sandbox , /proc cleanly unmounts using umount(). With this we are preventing mount leaks just like Docker does with their safe exiting mechanisms.
We also designed a small logging module utils.c with timestamped [INFO] and [ERROR] messages. 
We structured the project cleanly, headers in the include folder, functions in src, tests… everything orchestrated by CMake.

Contribution:
Marija: core logic: clone,chroot(), /proc mount; set up the Docker environment, handled build/debug cycle, included parallelism and networking.
Ali: Designed CMake structure, logging utilities,memory limits, and report and documentation.
AI Statement:
We sometimes used it for debugging and brainstorming, as well as documentation. Most of the built product is minimal, and well understood. We enjoyed every bit of the challenge!





