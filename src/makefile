# location of the Python header files
 
PYTHON_VERSION = 2.7
PYTHON_VERSION_BOOST = 27
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
 
# compile mesh classes
TARGET = launcher
 
$(TARGET).so: $(TARGET).o
	g++ -shared -Wl,--export-dynamic ./*.o -L$(BOOST_LIB) -lboost_python-py$(PYTHON_VERSION_BOOST)  -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so

$(TARGET).o:
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC)  -fPIC -c ./*.cpp ./*.h -std=c++0x
	
clean:
	$(RM) *.h.gch *.o *.so*