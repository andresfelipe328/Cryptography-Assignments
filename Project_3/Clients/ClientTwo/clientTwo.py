# General Imports
import socket
import pickle
import threading
import sys
import os

# RSA Related Imports
import rsa
import hashlib

FORMAT = "utf-8"
BUFFER = 2048
DBUFFER = 256
UBUFFER = 245
CLIENT_PORT = 50452
BASE_PORT = 50450


def Dowload(NEW_SOCKET, portNumber, scheme, fileSize, upFile):
    data = ""
    plaintext = ""
    delete = False

    newFile = open("Downloads/download_" + upFile, 'wb')
    if scheme == "rsa":
        data = NEW_SOCKET.recv(DBUFFER)
        with open('privateTwo.pem', 'rb') as filePrivateKey:
            keyData = filePrivateKey.read()
            privateKey = rsa.PrivateKey._load_pkcs1_pem(keyData)

        try:
            plaintext = rsa.decrypt(data, privateKey)
        except:
            delete = True
        newFile.write(plaintext)

        while len(data) > 0:
            data = NEW_SOCKET.recv(DBUFFER)
            if data == "stop".encode(FORMAT):
                break
            try:
                plaintext = rsa.decrypt(data, privateKey)
            except:
                delete = True
                break
            newFile.write(plaintext)

    elif scheme == "dgs":
        signature = NEW_SOCKET.recv(BUFFER)
        
        if portNumber == (BASE_PORT + 1):
            with open('PublicKeys/publicOne.pem', 'rb') as filePublicKey:
                keyData = filePublicKey.read()
                publicKey = rsa.PublicKey.load_pkcs1_openssl_pem(keyData)

            fileBuffer = NEW_SOCKET.recv(BUFFER)
            
            if fileBuffer[len(fileBuffer) - 4:] == "stop".encode(FORMAT):
                final = fileBuffer[0:len(fileBuffer) - 4]
                verify = rsa.verify(final, signature, publicKey)
                if verify == 'SHA-256':
                    print("\t[CLIENT] - Valid")
                    print("\t[CLIENT] - Successful Download")
                    return
                else:
                    print("\t[CLIENT] - Invalid")
                    print("\t[CLIENT] - Unsuccessful Download")
                    return
            message = fileBuffer
            newFile.write(fileBuffer)
            
            while fileBuffer:
                fileBuffer = NEW_SOCKET.recv(BUFFER)
                newFile.write(fileBuffer)
                message += fileBuffer
                if fileBuffer[len(fileBuffer) - 4:] == "stop".encode(FORMAT):
                    final = message[0:len(message) - 4]
                    break
            
            verify = rsa.verify(final, signature, publicKey)
            if verify == 'SHA-256':
                print("\t[CLIENT] - Valid")
            else:
                delete = True
                print("\t[CLIENT] - Invalid")

        if portNumber == (BASE_PORT + 5):
            with open('PublicKeys/publicThree.pem', 'rb') as filePublicKey:
                keyData = filePublicKey.read()
                publicKey = rsa.PublicKey.load_pkcs1_openssl_pem(keyData)

            fileBuffer = NEW_SOCKET.recv(BUFFER)
            
            if fileBuffer[len(fileBuffer) - 4:] == "stop".encode(FORMAT):
                final = fileBuffer[0:len(fileBuffer) - 4]
                verify = rsa.verify(final, signature, publicKey)
                if verify == 'SHA-256':
                    print("\t[CLIENT] - Valid")
                    print("\t[CLIENT] - Successful Download")
                    return
                else:
                    print("\t[CLIENT] - Valid")
                    print("\t[CLIENT] - Unsuccessful Download")
                    return
            message = fileBuffer
            newFile.write(fileBuffer)
            
            while fileBuffer:
                fileBuffer = NEW_SOCKET.recv(BUFFER)
                newFile.write(fileBuffer)
                message += fileBuffer
                if fileBuffer[len(fileBuffer) - 4:] == "stop".encode(FORMAT):
                    final = message[0:len(message) - 4]
                    break
            
            verify = rsa.verify(final, signature, publicKey)
            if verify == 'SHA-256':
                print("\t[CLIENT] - Valid")
            else:
                delete = True
                print("\t[CLIENT] - Invalid")

    newFile.close()

    if delete:
        name = "Downloads/download_" + upFile
        print("\t[CLIENT] - Unsucccessful")
        os.remove(name)
    else:
        print("\t[CLIENT] - Successful Download")


def TransferFile(conn, address, data, scheme):
    if not os.path.exists("OwnedFiles/" + data):
        return False
    print("TransReach")
    #scheme = conn.recv(BUFFER).decode(FORMAT)
    path = "OwnedFiles/" + data
    
    with open(path, 'rb') as outFile:
        bytesSend = outFile.read(UBUFFER)
        
        # Decide which public key to use
        if address[1] == (BASE_PORT + 6) and scheme == "rsa":
            with open('PublicKeys/publicOne.pem', 'rb') as filePublicKey:
                keyData = filePublicKey.read()
                publicKey = rsa.PublicKey.load_pkcs1_openssl_pem(keyData)

            ciphertext = rsa.encrypt(bytesSend, publicKey)
            conn.send(ciphertext)
            while len(bytesSend) > 0:
                bytesSend = outFile.read(UBUFFER)
                if len(bytesSend) == 0:
                    conn.send("stop".encode(FORMAT))
                    break 
                ciphertext = rsa.encrypt(bytesSend, publicKey)
                conn.send(ciphertext)
            
        elif address[1] == (BASE_PORT + 10) and scheme == "rsa":
            with open('PublicKeys/publicThree.pem', 'rb') as filePublicKey:
                keyData = filePublicKey.read()
                publicKey = rsa.PublicKey.load_pkcs1_openssl_pem(keyData)
            filePublicKey.close()

            ciphertext = rsa.encrypt(bytesSend, publicKey)
            conn.send(ciphertext)
            while len(bytesSend) > 0:
                bytesSend = outFile.read(UBUFFER)
                if len(bytesSend) == 0:
                    conn.send("stop".encode(FORMAT))
                    break 
                ciphertext = rsa.encrypt(bytesSend, publicKey)
                conn.send(ciphertext)

        elif scheme == "dgs":
            with open('privateTwo.pem', 'rb') as filePrivateKey:
                keyData = filePrivateKey.read()
                privateKey = rsa.PrivateKey._load_pkcs1_pem(keyData)

            sha = hashlib.sha256()
            while len(bytesSend) > 0:
                sha.update(bytesSend)
                bytesSend = outFile.read(BUFFER)
                
            signature = rsa.sign_hash(sha.digest(), privateKey, 'SHA-256')
            conn.send(signature)
            
            outFile.seek(0,0)
            data = outFile.read(BUFFER)
            mess = data
            total = len(data)
            conn.send(data)

            while len(mess) > 0:
                data = outFile.read(BUFFER)
                mess = data
                total += len(mess)
                conn.send(data)
            conn.send("stop".encode(FORMAT))
            
    outFile.close()            


def CommunicationWithClients(conn, address):                        # Changed
    while True:
        data = conn.recv(BUFFER).decode(FORMAT)
        scheme = conn.recv(BUFFER).decode(FORMAT)
        if data == "quit":
            break
        if TransferFile(conn, address, data, scheme) == False or not data or not scheme:
            #data = conn.recv(BUFFER).decode(FORMAT)
            #scheme = conn.recv(BUFFER).decode(FORMAT)
            continue
    conn.close()

def ConnectWithClients(CLIENT, CLIENT_PORT):
    CLIENT_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    CLIENT_SOCKET.bind((CLIENT, CLIENT_PORT + 1))

    CLIENT_SOCKET.listen(2)
    while True:
        conn, address = CLIENT_SOCKET.accept()
        cThread = threading.Thread(target=CommunicationWithClients, args=(conn, address))
        cThread.daemon = True
        cThread.start()
        print("\t[Server] - Client [" + str(address[0]) +":" + str(address[1]) + "] is connected")


def ConnectToClient(CLIENT, CLIENT_PORT):
    
    domain = input("Domain -> ")
    if not domain:
      domain = input("Domain -> ")  
    portNum = int(input("Port Number -> "))
    if not portNum:
        portNum = int(input("Port Number -> "))
    NEW_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    NEW_SOCKET.bind((CLIENT, CLIENT_PORT + 6))

    NEW_SOCKET.connect((domain, portNum))

    while True:
        file = input("Download File -> ")
        NEW_SOCKET.send(file.encode(FORMAT))
        if file == "quit":
            break
        elif not file:
            continue
        scheme = input("Scheme Selection -> ")
        NEW_SOCKET.send(scheme.encode(FORMAT))
        if scheme == "quit":
            break
        elif not scheme:
            continue
        Dowload(NEW_SOCKET, portNum, scheme, file)
    NEW_SOCKET.close()


def main():
    SERVER = socket.gethostbyname(sys.argv[1])
    CLIENT = socket.gethostbyname(socket.gethostname())              
    SERVER_PORT = int(sys.argv[2])  
    CLIENT_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    CLIENT_SOCKET.bind((CLIENT, CLIENT_PORT))
    counter = 0

    CLIENT_SOCKET.connect((SERVER, SERVER_PORT))
    
    cThread = threading.Thread(target=ConnectWithClients, args=(CLIENT, CLIENT_PORT))
    cThread.daemon = True
    cThread.start()

    while True:
        # Inputs Username
        if counter == 0:
            comm = input("Username -> ")
            if comm == "quit":
                break
            elif not comm:
                counter = 0
                continue 
            username = 'u' + comm
            CLIENT_SOCKET.send(username.encode(FORMAT))

        # Inputs Password
        elif counter == 1:
            comm = input("Password -> ")
            if comm == "quit":
                break
            password = 'p' + comm
            CLIENT_SOCKET.send(password.encode(FORMAT))
            conf = CLIENT_SOCKET.recv(BUFFER).decode(FORMAT)
            
            if conf == "exists":
                print("\t[SERVER] - Account exists, continue")
            elif conf == "added":
                print("\t[SERVER] - Account added, continue")
            elif conf == "full":
                print("\t[SERVER] - Account cannot be added, server capacity is full")
                counter = 0
                continue
            else:
                print("\t[SERVER] - Wrong account, please try again (username and password)")
                counter = 0
                continue
        
        # Start Communcating with Server   
        else:
            comm = input("INPUT -> ")
            # Inputs file's name to store
            if comm[:3] == "put":
                fileName = 'f' + comm[4:]
                CLIENT_SOCKET.send(fileName.encode(FORMAT))

            # Inputs keyword
            elif comm[:3] == "key":
                keyword = 'k' + comm[4:]
                CLIENT_SOCKET.send(keyword.encode(FORMAT))
                conf = CLIENT_SOCKET.recv(BUFFER).decode(FORMAT)
                if conf == "success":
                    print("\t[SERVER] - successful file name(s) transfer")

            # Inputs keyword to search
            elif comm[:4] == "srch":
                query = 'q' + comm[5:]
                CLIENT_SOCKET.send(query.encode(FORMAT))
                rClient = CLIENT_SOCKET.recv(BUFFER)
                rClient = pickle.loads(rClient)
                if not rClient == "none":
                    print("\t[SERVER] - These are the owners (domain, port #): " + str(rClient))
                else:
                    print("\t[SERVER] - Keyword does not exist")
            
            # Show list of files
            elif comm[:4] == "list":
                rClientList = 'l' + comm[5:]
                CLIENT_SOCKET.send(rClientList.encode(FORMAT))
                rClient = CLIENT_SOCKET.recv(BUFFER)
                rClient = pickle.loads(rClient)
                print("\t[Server] - Files under keyword: " + str(rClient))

            # Quits connection
            elif comm == "quit":
                break
        counter += 1
    
    CLIENT_SOCKET.close()
    ConnectToClient(CLIENT, CLIENT_PORT)

if __name__ == '__main__':
    main()                
