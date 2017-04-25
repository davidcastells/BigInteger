TARGET=BigInteger


TARGET:
	g++ -g -O3 main.cpp BigInteger.cpp CorrectnessTest.cpp PerformanceLap.cpp PerformanceTest.cpp -o $(TARGET)
