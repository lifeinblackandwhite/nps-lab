# README - Running on Ubuntu

This document describes how to compile and run the programs in this repository on Ubuntu (tested on Ubuntu 20.04+). Each program folder (Prog1..Prog8) contains C source files; the instructions below show the minimal commands to build and run them locally.

## Prerequisites

Install the build tools and common networking utilities:

```bash
sudo apt update
sudo apt install -y build-essential gcc make netcat
```

## General notes

- For TCP programs: start the server first in one terminal, then run the client in another terminal. Use `localhost` if both run on the same machine or an IP address if running across machines.
- Default ports used by code: mostly `15001` for TCP examples and `16001` for UDP examples; multicast example uses port `12345` and group `225.0.0.37`.
- If a client expects an IP argument, pass `127.0.0.1` (localhost) when testing locally.
- Be careful with `Prog7` (remote command server) — it executes received commands via `system()` (security risk). Use only in a safe environment.

## Folder-by-folder commands

Prog1 - simple TCP file transfer

Compile:

```bash
gcc Prog1/server.c -o Prog1/server
gcc Prog1/client.c -o Prog1/client
```

Run server (terminal A):

```bash
./Prog1/server
```

Run client (terminal B):

```bash
./Prog1/client 127.0.0.1
# then type the filename when prompted (file must exist in server cwd)
```

Prog2 - Distance Vector (matrix input)

Compile:

```bash
gcc Prog2/dv.c -o Prog2/dv
```

Run and follow prompts (enter node count then the matrix):

```bash
./Prog2/dv
```

Prog3 - Checksum and Hamming

Compile both:

```bash
gcc Prog3/checksum.c -o Prog3/checksum
gcc Prog3/hamming.c -o Prog3/hamming
```

Run checksum (enter 10 hex 16-bit words when prompted):

```bash
./Prog3/checksum
```

Run hamming (enter four data bits when prompted):

```bash
./Prog3/hamming
```

Prog4 - Multicast sender/receiver

Compile:

```bash
gcc Prog4/server.c -o Prog4/mcast_recv
gcc Prog4/client.c -o Prog4/mcast_send
```

Run receiver (terminal A):

```bash
./Prog4/mcast_recv
```

Run sender (terminal B):

```bash
./Prog4/mcast_send
```

Prog5 - TCP echo (server/client)

Compile:

```bash
gcc Prog5/server.c -o Prog5/server
gcc Prog5/client.c -o Prog5/client
```

Run server (terminal A):

```bash
./Prog5/server
```

Run client (terminal B):

```bash
./Prog5/client 127.0.0.1
# then type lines to send; server will echo
```

Prog6A - TCP file/echo (similar to Prog1)

Compile:

```bash
gcc Prog6A/server.c -o Prog6A/server
gcc Prog6A/client.c -o Prog6A/client
```

Run server (terminal A):

```bash
./Prog6A/server
```

Run client (terminal B):

```bash
./Prog6A/client 127.0.0.1
# follow prompts (enter filename or input as requested)
```

Prog6B - UDP echo (client/server)

Compile:

```bash
gcc Prog6B/server.c -o Prog6B/udp_server
gcc Prog6B/client.c -o Prog6B/udp_client
```

Run server (terminal A):

```bash
./Prog6B/udp_server
```

Run client (terminal B):

```bash
./Prog6B/udp_client 127.0.0.1
# then type lines to send; server will echo them back
```

Prog6C - TCP echo (single-client echo)

Compile:

```bash
gcc Prog6C/server.c -o Prog6C/server
gcc Prog6C/client.c -o Prog6C/client
```

Run server (terminal A):

```bash
./Prog6C/server
```

Run client (terminal B):

```bash
./Prog6C/client 127.0.0.1
```

Prog7 - Remote command server (dangerous)

Compile:

```bash
gcc Prog7/server.c -o Prog7/server
gcc Prog7/client.c -o Prog7/client
```

Run server (terminal A):

```bash
./Prog7/server
```

Run client (terminal B):

```bash
./Prog7/client 127.0.0.1
# type shell commands; server executes them using system()
```

Prog8 - Crypto demos (Diffie-Hellman and RSA)

Compile:

```bash
gcc Prog8/dh.c -o Prog8/dh
gcc Prog8/rsa.c -o Prog8/rsa
```

Run DH demo:

```bash
./Prog8/dh
# follow interactive prompts for p, g, a, b
```

Run RSA demo (single word, no spaces):

```bash
./Prog8/rsa
# enter the text when prompted
```

## Troubleshooting

- If compilation fails, install `build-essential` and `gcc` (see prerequisites).
- If a TCP client fails to connect, ensure the server is running and listening on the correct port and that no firewall blocks the port.
- For network tests across machines, replace `127.0.0.1` with the server machine IP and ensure both machines can reach each other.

If you want, I can also: compile all programs now and report any compile errors, or create a single helper script that builds every program automatically. Reply which you'd prefer.

Fixes applied
-------------
I made a set of safety and correctness fixes to the C sources to make them safer to build and run on Ubuntu. Key changes:

- Added `argc` / address validation to clients that assumed `argv[1]` existed.
- Fixed incorrect `send`/`sendto` lengths where `sizeof(pointer)` or full buffer size was used; now use `strlen()` or the actual returned byte counts.
- Avoided unsafe `printf("%s", buffer)` on non-NUL-terminated or binary data; use `write()` or NUL-terminate after `recv()`/`read()`.
- Added basic error checking for `socket()`, `connect()`, `bind()`, `listen()`, and `accept()` in several places.
- Replaced an incorrect RSA encrypt/decrypt loop with a correct modular-exponentiation implementation.
- Disabled automatic execution of commands received by the remote-command server (`Prog7`) unless compiled with `-DENABLE_REMOTE_COMMAND_EXEC` (see `Prog7/server.c`). This prevents accidental remote code execution by default.

Files changed (high level): `Prog1/*`, `Prog4/*`, `Prog5/*`, `Prog6C/*`, `Prog7/*`, `Prog8/rsa.c`.

If you'd like, I can now attempt to compile all programs on this machine and report warnings/errors, or produce a `build_all.sh` script that compiles everything on Ubuntu. Which would you like me to do next?
