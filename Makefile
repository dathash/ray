rtw: main.cpp *.h
	clang++ -std=c++11 -o $@ $<

clean:
	rm -f *.o rtw
