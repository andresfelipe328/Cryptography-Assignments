
cipher: driver.o Playfair.o Railfence.o Row.o Caesar.o Vigenre.o Hill.o
	g++ driver.o Playfair.o Railfence.o Row.o Caesar.o Vigenre.o Hill.o -o cipher

driver.o: driver.cpp
	g++ -c driver.cpp

Playfair.o: Playfair.cpp Playfair.h
	g++ -c Playfair.cpp

Railfence.o: Railfence.cpp Railfence.h
	g++ -c Railfence.cpp

Row.o: Row.cpp Row.h
	g++ -c Row.cpp

Caesar.o: Caesar.cpp Caesar.h
	g++ -c Caesar.cpp

Vigenre.o: Vigenre.cpp Vigenre.h
	g++ -c Vigenre.cpp

Hill.o: Hill.cpp Hill.h
	g++ -c Hill.cpp

clean:
	rm *.o cipher