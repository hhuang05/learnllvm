#ifndef	LEXER_H	
#define	LEXER_H	
#include <string>	
#include <cstdio>

class Lexer {
public:
	std::string getToken();
	Lexer() :
			lastChar(' ') {
	} //member initialization

private:
	char lastChar;
	inline char getNextChar() {
		char c = lastChar;
		lastChar = getchar();
		return c;
	}
};
#endif
