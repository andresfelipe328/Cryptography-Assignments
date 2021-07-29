import threading
import socket
import pickle
import hashlib
import sys
import os

FORMAT = "utf-8"
BUFFER = 1024
clientList = []


def ShowFileList(client, qKeyword):

    inFile = open('FileRecord.txt', 'rb')
    record = pickle.load(inFile)

    return record[client][qKeyword]


def SearchClient(qkeyword):
    # Variables
    ownerList = []
    found = False

    # If file doesn't exist return none
    if not os.path.isfile('FileRecord.txt'):
        print("\t[Server] - query not  possible, file does not exist")
    # else, open file, save data and compare to given keyword
    else:
        inFile = open('FileRecord.txt', 'rb')
        record = pickle.load(inFile)
        
        # Iterate through the nested dictionary, for each found append to list
        for client, keywords in record.items():
            for key in keywords:
                if (key == qkeyword):
                    ownerList.append(client)
                    found = True
        
        # If owners were found, send list
        if found:
            return ownerList

    return "none"


def StoreFileNames(fileList, keyword, address): 
    # Variables
    record = {}
    clientAdd = str(address[0]) + ":" + str(address[1] + 1)

    # If file does not exist, add files under the given address into the file
    if not os.path.isfile('FileRecord.txt'):
        record = {clientAdd:{keyword:fileList}}
        outFile = open('FileRecord.txt', 'wb')
        pickle.dump(record, outFile)
        print("[Server] - all files' names were successfully stored in your account " + str(clientAdd))

    # else, load data, and append new data according to client's address
    else:
        inFile = open('FileRecord.txt', 'rb')
        record = pickle.load(inFile)

        # if client exist, check if keyword exists
        if clientAdd in record.keys():
            # if keyword exist, append new files to keyword given
            if keyword in record[clientAdd].keys():
                for files in fileList:
                    record[clientAdd][keyword].append(files)
            # else, create a new keyword and add the files
            else:
                record[clientAdd].update({keyword:fileList})
        # if the client doesn't exist, create new user and add the files
        else:
                record.update({clientAdd:{keyword:fileList}})
        
        # Update the record in file
        outFile = open('FileRecord.txt', 'wb')
        pickle.dump(record, outFile)
        print("[Server] - all files' names were successfully stored in your account " + str(clientAdd))
        return True


def SearchSetUsers(userName, password):
    fileName = "AccountRecord.txt"
    verifier = ""
    
    if not os.path.isfile(fileName):
        password = hashlib.sha256(password.encode('utf-8')).hexdigest()
        
        record = {userName:password}
        newFile = open(fileName, "wb")
        pickle.dump(record, newFile)
        verifier = "added"
    else: 
        inFile = open(fileName, "rb")
        record = pickle.load(inFile)
        if userName in record:
            verifier = "exists"
            password = hashlib.sha256(password.encode('utf-8')).hexdigest()
        
            if not record[userName] == password:
                verifier = "wrong"
        else:
            password = hashlib.sha256(password.encode('utf-8')).hexdigest()
            record[userName] = password
            outFile = open(fileName, "wb")
            pickle.dump(record, outFile)
            verifier = "added"
            if len(record.keys()) > 3:
                verifier = "full"    
        
    return verifier


def VerifyAccount(username, password):
    # Opens files with account dic
    verifier = SearchSetUsers(username, password)

    # If it exist, send confirmed, else send unconfirmed
    if verifier == "exits":
        print("\t[Server] - Account exists")
        verifier = "exists"
    elif verifier == "full":
        print("\t[Server] - Account cannot be added, full capacity reached")
        verifier = "full"
    elif verifier == "added":
        print("\t[Server] - Account added")
        verifier = "added"
    elif verifier == "wrong":
        print("\t[Server] - Wrong Account Info")
    return verifier


def CommunicationWithClients(conn, address):
    # Variable
    fileList = []
    qKeyword = ""

    # Communication loop
    while True:
        data = conn.recv(BUFFER).decode(FORMAT)

        if not data:
            print("\t[Server] - Client [" + str(address[0]) + ":" + str(address[1]) + "] disconnected")
            break

        else:
            # Variables
            code = data[0]
            data = data[1:]
            
            # For username
            if code == 'u':
                username = data
            # For password
            if code == 'p':
                password = data
                verifier = VerifyAccount(username, password)
                if verifier == "exists":
                    conn.send("exists".encode(FORMAT))
                elif verifier == "full":
                    conn.send("full".encode(FORMAT))
                elif verifier == "added":
                    conn.send("added".encode(FORMAT))
                else:
                    conn.send("nexist".encode(FORMAT))
            # For uploading files' names
            if code == 'f':
                fileList.append(data)
            # For files' keyword 
            if code == 'k':
                keyword = data
                if StoreFileNames(fileList, keyword, address):
                    conn.send("success".encode(FORMAT))
            # For query search based on keyword
            if code == 'q':
                qKeyword = data
                client = SearchClient(qKeyword)
                sendData = pickle.dumps(client)
                conn.send(sendData)
            if code == 'l':
                list = ShowFileList(data, qKeyword)
                sendData = pickle.dumps(list)
                conn.send(sendData)
    conn.close()

def main():
    SERVER_PORT = int(sys.argv[1])
    SERVER = socket.gethostbyname(socket.gethostname())
    SERVER_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    SERVER_SOCKET.bind((SERVER, SERVER_PORT))

    SERVER_SOCKET.listen(3)
    print("[Server: " + str(SERVER) + "] is searching for connections")

    while True:
        conn, address = SERVER_SOCKET.accept()
        print("\t[Server] - Client [" + str(address[0]) +":" + str(address[1]) + "] is connected")
        cThread = threading.Thread(target=CommunicationWithClients, args=(conn, address))
        cThread.daemon = True
        cThread.start()
        clientList.append(conn)


if __name__ == '__main__':
    main()
