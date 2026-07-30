# Basic TCP Client-Server Connection

A simple TCP socket program demonstrating a basic client-server connection using C.

## Objective

- Create a TCP socket.
- Establish a connection between client and server.
- Send a message from the client.
- Receive the message on the server.
- Send a reply back to the client.

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
close()

Client

socket()
    ↓
connect()
    ↓
send()
    ↓
recv()
    ↓
close()
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

Start the server first.

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
Server waiting for connection...
Client Connected!
Message from Client: Hello Server!
```

### Client

```
Server Reply: Connection Successful!
```

---

## Concepts Used

- TCP Socket Programming
- socket()
- bind()
- listen()
- accept()
- connect()
- send()
- recv()
- close()
