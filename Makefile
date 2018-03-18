TARGET_CYG=BigInteger.cyg.exe
TARGET_MINGW=BigInteger.mingw.exe
TARGET_VC=BigInteger.vc.exe
TARGET_ZZ_CYG=ZZTest.cyg.exe
TARGET_ZZ_MINGW=ZZTest.mingw.exe

MINGW=/usr/x86_64-w64-mingw/bin
VC="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC"

CC_MINGW=/usr/bin/x86_64-w64-mingw32-gcc.exe
CPP_MINGW=/usr/bin/x86_64-w64-mingw32-g++.exe
CC_VC=$(VC)/bin/cl.exe
CPP_VC=$(VC)/bin/cl.exe
MSVC_DIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC"
WINDOWS_SDK="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0"
WINDOWS_INCLUDE_DIR=/I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\ucrt" /I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\um" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\shared"
WINDOWS_LIBS=$(WINDOWS_SDK)/um/x86/uuid.lib $(WINDOWS_SDK)/um/x86/kernel32.lib $(WINDOWS_SDK)/ucrt/x86/libucrt.lib $(MSVC_DIR)/lib/libcpmt.lib $(MSVC_DIR)/lib/libcmt.lib $(MSVC_DIR)/lib/oldnames.lib $(MSVC_DIR)/lib/libvcruntime.lib
INCLUDE_DIRS_MSVC=/I /usr/local/include $(WINDOWS_INCLUDE_DIR) /I$(MSVC_DIR)/include
CFLAGS_MSVC=/D WIN32 $(INCLUDE_DIRS_MSVC) $(LIB_DIRS_MSVC) $(WINDOWS_LIBS)  /EHa /GR /Zl /DEBUG /MACHINE:X86

all: $(TARGET_CYG) 
	
perf: $(TARGET_CYG) $(TARGET_MINGW) $(TARGET_ZZ_CYG) # $(TARGET_VC) #$(TARGET_ZZ_MINGW)
	
SOURCES_CPP= main.cpp BigInteger.cpp CorrectnessTest.cpp PerformanceLap.cpp PerformanceTest.cpp cpp/*.cpp c/*.cpp c_array/*.cpp c_base/*.cpp	
SOURCES_C= big_integer.cpp big_integer_base.cpp big_integer_array.cpp

clean:
	rm -fr *.exe

$(TARGET_CYG): $(SOURCES_CPP) $(SOURCES_C)
	g++ -static -g -O0 $(SOURCES_CPP) $(SOURCES_C) -o $(TARGET_CYG)
	
$(TARGET_MINGW): $(SOURCES_CPP) 
	$(CPP_MINGW) -static -g -O3 -m64 $(SOURCES_CPP) $(SOURCES_C) -o $(TARGET_MINGW)

$(TARGET_VC): main.cpp BigInteger.cpp CorrectnessTest.cpp PerformanceLap.cpp PerformanceTest.cpp big_integer.cpp $(SOURCES_CPP) $(SOURCES_C)
	$(CPP_VC) $(CFLAGS_MSVC) -g -O3 $(SOURCES_CPP) $(SOURCES_C)  main.cpp BigInteger.cpp CorrectnessTest.cpp PerformanceLap.cpp PerformanceTest.cpp big_integer.cpp -o $(TARGET_VC)

$(TARGET_ZZ_CYG): mainZZ.cpp 
	g++ -g -O3 mainZZ.cpp PerformanceLap.cpp PerformanceTestZZ.cpp -L/usr/local/lib -lntl -lm -lgmp -o $(TARGET_ZZ_CYG)

$(TARGET_ZZ_MINGW): 
	$(CPP_MINGW) -static -g -O3 -m64 mainZZ.cpp PerformanceLap.cpp PerformanceTestZZ.cpp big_integer.cpp -I/usr/local/include -L/usr/local/lib -lntl -lm -lgmp -o $(TARGET_ZZ_CYG)
