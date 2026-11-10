# Example Questions

A collection of computer-network client-server examples, organized by protocol.
Each folder contains a question statement (`question.txt`) and the complete C
implementation.

## FTP

1. **[Vowel Count Date File](FTP/01-Vowel-Count-Date-File)** — The server counts
   the vowels in a file; if the count is prime it writes the current date and
   time to `date.txt`, otherwise it replies with "sorry".
2. **[Message Reverse Length](FTP/02-Message-Reverse-Length)** — Odd-length file
   content is reversed into `new_file.txt`; even-length content is replaced by
   its length. Repeats until the client sends `stop`.
3. **[Fibonacci Factorial](FTP/03-Fibonacci-Factorial)** — The client writes the
   Fibonacci series or the factorial to the file based on the length of the
   server's reply. Repeats until the client sends `exit`.
4. **[Matrix Addition](FTP/04-Matrix-Addition)** — The server adds two matrices
   read from files, writes the result to `matrix.txt`, and sends it back.
5. **[Sort Second Largest](FTP/05-Sort-Second-Largest)** — The server writes the
   sorted numbers to `sort.txt` and the second-largest number to `second.txt`;
   the client displays both files.
6. **[Armstrong File](FTP/06-Armstrong-File)** — The server reads a number from
   a file and writes an Armstrong/Not Armstrong result for the client to open.

## TCP

1. **[Digit Sum Prime Multi-Client](TCP/01-Digit-Sum-Prime-Multi-Client)** —
   Client 1 sends a number, the server sends its digit sum to Client 2, and
   Client 2 replies with "prime" or "not prime".
2. **[Number Reversal Multi-Client](TCP/02-Number-Reversal-Multi-Client)** —
   Each client sends a number; the server reverses both and swaps the replies.
3. **[Message Length Parity](TCP/03-Message-Length-Parity)** — The server finds
   the message length without a built-in function and replies "odd" or "even";
   the client replaces all a's with b's for an odd message.
4. **[Message Reverse Length](TCP/04-Message-Reverse-Length)** — Odd-length
   messages are reversed, while even-length messages are answered with their
   length. Repeats until the client sends `stop`.
5. **[Matrix Addition Largest](TCP/05-Matrix-Addition-Largest)** — The server
   adds two matrices and returns the resultant matrix and its largest element.
6. **[Armstrong Multi-Client](TCP/06-Armstrong-Multi-Client)** — The server
   sends "Armstrong" to Client 2 or "Not Armstrong" to Client 1 based on the
   number received from Client 1.

## UDP

1. **[Substring Occurrence](UDP/01-Substring-Occurrence)** — The server counts
   occurrences of a substring and sends that many replies back to the client.
2. **[Number Pattern](UDP/02-Number-Pattern)** — The server prints a number
   pattern up to the limit supplied by the client.
3. **[Matrix Multiplication Sort](UDP/03-Matrix-Multiplication-Sort)** — The
   server multiplies two matrices, sorts the product elements, and returns them.
4. **[Consonant Count Date Time](UDP/04-Consonant-Count-Date-Time)** — A prime
   consonant count receives the current date and time; otherwise the client
   receives "sorry".
5. **[Fibonacci Factorial](UDP/05-Fibonacci-Factorial)** — The client prints the
   Fibonacci series for an odd-length reply or the factorial for an even-length
   reply. Repeats until the client sends `exit`.
6. **[Sort Second Largest Palindrome](UDP/06-Sort-Second-Largest-Palindrome)** —
   The server sorts the numbers and returns the second-largest number, which the
   client checks for a palindrome.

## Running an example

Open two terminals in the example folder. Start the server first:

```sh
gcc *_server.c -o server
./server
```

Then run the client from the second terminal:

```sh
gcc *_client.c -o client
./client
```

Multi-client examples provide `client1` and `client2` programs; compile and run
both after starting the server. The examples use port `8080` and connect to
`127.0.0.1`.
