## C Chat Client for Linux

This project contains a server and client written in C that allow users to communicate in a chat-room style. It does not have a standalone GUI. Instead, it is used with a UNIX terminal.

## Motivation

I created this project for two reasons:

1. To gain a deeper understanding of low-level network communications after being exposed to the high level in [my Python-based chat client project](https://github.com/cplant1776/simple_kivy_chat "Kivy-based Python Chat Client and Server").

2. To refresh myself on the basics of C, as I have not used it in over a year.

## Current Features

* Server-side stored user credentials in a database, requiring users to log in when connecting
* Public messages broadcast to the entire "room" of connected users
* Private messaging between users

## Usage

After compiling,

Server:

```c
./server
```

Client:

```c
./client SERVER_IP_ADDRESS
```

To change the server to a specific address/port, edit ADDRESS/PORT macro in config.h.

To change the client connection port, edit PORT macro at top of client.c. (client config.h is coming in a future update).

By default, the server runs locally (127.0.0.1) on port 1776. Client defaults to port 1776.

When connecting, the server will prompt the user for a username and password. If it's a new username, the user will be asked to set a password. If it is an existing user, they will be prompted to enter their password. If it is correct, they will be connected.

Once connected, simply type the message you want to broadcast to the room and press enter. To send a private message, type it in the format `@[message recipient], [message]`.

## Upcoming Changes

* Add config.h file for client
* Add encryption to communications
* Add protections against user misuse
* Fix occasional infinite loop on user disconnect
