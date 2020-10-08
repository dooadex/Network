
# UDP communication test program

This is for UDP communication test.

It's vety easy to use.

Check your IP Address of computer, and edit just one line of code in 'client.cpp'.

```c++
serveraddr.sin_addr.s_addr=inet_addr("your IP Address");
```

If you want to test with only one computer, set the IP Address part as "127.0.0.1".

This sends message to HOST PC.

It means sending and receiving are operated in one same computer.

