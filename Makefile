TARGET=BigInteger


TARGET:
	g++ -g main.cpp BigInteger.cpp CorrectnessTest.cpp -o $(TARGET)
