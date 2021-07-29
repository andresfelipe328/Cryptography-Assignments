Author:
	Andres F. Romero Nogales
	andresfelipe328@csu.fullerton.edu

Execution:
	1. Open four terminals, one for Server Directory and the other three for each client's directory.
	2. Execute the server first by inputing:
		python3 iServer.py <IP address of your system> <portnumber of server>
	3. Execute one client at minimum by inputting:
		python3 clientOne.py <IP addess of server, your system> <portnumber of server>
	4. Input the correct username and password
	5. add a "put" to store files, "key" to set the keyword for those files. You must input the keyword after
	   begin done with the files. So the order is files -> keyword. You may repeat this for as times as you
	   want.
	6. When exiting the server, you must input a domain and port number. An error here will crash the program.
	7. Select an existing file from the requested client and provide the scheme. You must input an existing file.
	   Do this as many times as you want.
	8. Close the client with quit. 
	9. Close the server with ctrl + c

Important Mention:	
	1. When a client connects to you, a message will be displayed, do not press enter. Just continue inputting data.
	   If you hit enter after the message is shown, the program will crash. 