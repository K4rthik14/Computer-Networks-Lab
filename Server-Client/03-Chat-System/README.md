# Chat System using TCP Socket

A simple client-server chat application using TCP sockets in C.

The client and server exchange messages continuously until either side sends the keyword **stop**.

---

## Objective

- Create a TCP connection between client and server.
- Exchange messages between both ends.
- Continue chatting until either side sends `stop`.

---

## Socket Flow

```
Server

socket()
    ↓
bind()
    ↓
listen()
    ↓
accept()
    ↓
recv()
    ↓
send()
    ↓
(repeat)
```

```
Client

socket()
    ↓
connect()
    ↓
send()
    ↓
recv()
    ↓
(repeat)
```

---

## Compilation

### Server

```bash
gcc server.c -o server
```

### Client

```bash
gcc client.c -o client
```

---

## Execution

Run the server first.

```bash
./server
```

Open another terminal.

```bash
./client
```

---

## Sample Output

### Server

```
Waiting for client...
Client Connected!

Client : Hello
Server : Hi

Client : How
Server : Fine

Client : stop
```

### Client

```
Client : Hello
Server : Hi

Client : How
Server : Fine

Client : stop
```

---

## Functions Used

- socket()
- bind()
- listen()
- accept()
- connect()
- send()
- recv()
- strcmp()
- close()

---

## Note

This implementation uses `scanf("%s", message)`, so it reads only one word at a time. Messages containing spaces (e.g., "Hello World") are not supported in this basic version.