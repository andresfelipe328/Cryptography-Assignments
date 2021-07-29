import pickle

file = input("File to display -> ")
inFile = open(file, 'rb')
record = pickle.load(inFile)
print(record)