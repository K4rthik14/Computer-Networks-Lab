# Sum of Two Numbers using TCP Socket

This program demonstrates communication between a TCP client and server.

The client sends two integers to the server. The server calculates their sum and sends the result back to the client.

---

## Objective

- Establish a TCP connection.
- Send two numbers from the client.
- Compute the sum on the server.
- Return the result to the client.

---

## Socket Flow

```
Client
------
Enter Numbers
      │
      ▼
send()
      │
      ▼
Server
------
recv()
      │
Calculate Sum
      │
send()
      │
      ▼
Client
------
recv()
Display Result
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
Received: 15 and 25
Sum = 40
```

### Client

```
Enter first number : 15
Enter second number: 25
Sum received from server = 40
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
- sscanf()
- sprintf()
- close()