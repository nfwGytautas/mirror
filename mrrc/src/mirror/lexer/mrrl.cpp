#include "mirror/lexer/mrrl.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "mirror/utility/log.hpp"

namespace mirror {

	int parse_identifier_str(const std::string& identifier) {

		if (identifier == "fn") {
			return mrrt_fn;
		}

		if (identifier == "var") {
			return mrrt_var;
		}

		if (identifier == "return") {
			return mrrt_ret;
		}

		if (identifier == "loop") {
			return mrrt_loop;
		}

		if (identifier == "match") {
			return mrrt_match;
		}

		if (identifier == "type") {
			return mrrt_typedef;
		}

		return mrrt_identifier;
	}

	namespace lexer {

		mrrl* g_ci = nullptr;

		int get_c(mrrl* ls) {
		    if (ls->In == nullptr) {
		        ls->In = new mrr_lexer_stream();

		        int c = getchar();
		        while(c != EOF) {
		            ls->In->Input += c;
                    c = getchar();
		        }
		    }

            if (ls->In->StreamPosition + 1 > ls->In->Input.size()) {
                return EOF;
            }

            return ls->In->Input[ls->In->StreamPosition++];
		}

		int peek_c(mrrl* ls) {
		    return ls->In->Input[ls->In->StreamPosition];
		}

		int parse_identifier(mrrl* ls) {
			return parse_identifier_str(ls->IdentifierStr);
		}

		int parse_token(mrrl* ls) {
			static int currentChar = ' ';

			// Ignore whitespace
			while (isspace(currentChar)) {
				currentChar = get_c(ls);
			}

			// Comment
			if (currentChar == '/') {
				// Ignore until end of file
				do {
					currentChar = get_c(ls);
				} while (currentChar != EOF && currentChar != '\n' && currentChar != '\r');

				if (currentChar != EOF) {
					return next_token(ls);
				}
			}

			if (currentChar == '-' && peek_c(ls) == '>') {
			    // Consume both
                currentChar = get_c(ls);
                currentChar = get_c(ls);
			    return mrrt_rarrow;
			}

			if (currentChar == ':') {
                currentChar = get_c(ls);
			    return mrrt_type_annot;
			}

			// String
			if (currentChar == '"') {
				ls->IdentifierStr = "";

				// Parse string
				currentChar = get_c(ls);
				while (currentChar != '"') {
					ls->IdentifierStr += currentChar;
					currentChar = get_c(ls);
				}
				currentChar = get_c(ls);

				return mrrt_str;
			}

			// Identifier (alphabetic char)
			if (currentChar == '_' || isalpha(currentChar)) {
				ls->IdentifierStr = currentChar;

				// Parse identifier
                currentChar = get_c(ls);
				while (isalnum(currentChar) || currentChar == '_') {
					ls->IdentifierStr += currentChar;
                    currentChar = get_c(ls);
				}

				return parse_identifier(ls);
			}

			// Number
			if (isdigit(currentChar)) {
				std::string numStr;

				// TODO: Fix for input where '.' entered multiple times
				do {
					numStr += currentChar;
					currentChar = get_c(ls);
				} while (isdigit(currentChar) || currentChar == '.');

				ls->Num = strtod(numStr.c_str(), 0);
				return mrrt_number;
			}

			// Equal
			if (currentChar == '=' && peek_c(ls) == '=') {
                currentChar = get_c(ls);
                currentChar = get_c(ls);
			    return mrrt_equal;
			}

			// Other
			if (currentChar == EOF) {
				return mrrt_eof;
			}

			int prevChar = currentChar;
			currentChar = get_c(ls);
			return prevChar;
		}

		int next_token(mrrl* ls) {
			int token = parse_token(ls);
			ls->Curtok = token;
			return ls->Curtok;
		}

		mrrl* from_file(const std::string& file) {
			mrrl* l = new mrrl();

			// Read file
			std::ifstream t(file);
			t.seekg(0, std::ios::end);
			size_t size = t.tellg();
			std::string buffer(size, ' ');
			t.seekg(0);
			t.read(&buffer[0], size);

			// Create input stream
			l->In = new mrr_lexer_stream();
			l->In->Input = buffer;

			return l;
		}

		void free_mrrl(mrrl* l) {
			if (l->In) {
				delete l->In;
			}

			delete l;
		}

		mrrl* get_current() {
			return g_ci;
		}
		
		void set_current(mrrl* l) {
			g_ci = l;
		}
	}

	namespace lexer_v2 {

		mrrl* mrrl::get() {
			static mrrl instance;
			return &instance;
		}

		mrr_token_stream mrrl::parse_file(const std::string& file) {
			// Read file
			std::ifstream t(file);
			t.seekg(0, std::ios::end);
			size_t size = t.tellg();
			std::string buffer(size, ' ');
			t.seekg(0);
			t.read(&buffer[0], size);
			return parse_string(buffer);
		}

		std::string mrrl::token_to_str(mrr_token t) {
			switch (t) {
			case mrrt_eof:
				return "EOF";
			case mrrt_fn:
				return "fn";
			case mrrt_identifier:
				return "identifier";
			case mrrt_str:
				return "string";
			case mrrt_var:
				return "var";
			case mrrt_ret:
				return "return";
			case mrrt_loop:
				return "loop";
			case mrrt_match:
				return "match";
			case mrrt_rarrow:
				return "->";
			case mrrt_type_annot:
				return ":";
			case mrrt_equal:
				return "=";
			case mrrt_typedef:
				return "type";
			default:
				if (t == 0) {
					return "EOF";
				}

				return std::string(1, (char)t);
			}
		}

		mrr_token_stream mrrl::parse_string(const std::string& str) {
			mrr_token_stream ts;
			size_t currentPos = 0;
			size_t line = 1;

			while (currentPos <= str.length()) {
				// Ignore whitespace
				while(isspace(str[currentPos])) {
					if (str[currentPos] == '\n') {
						line++;
					}

					currentPos++;
				}

				// Comment
				if (str[currentPos] == '/' && str[currentPos + 1] == '/') {
					currentPos += 2;

					// Ignore until end of file
					do {
						currentPos++;
					} while (str[currentPos] != EOF && str[currentPos] != '\n' && str[currentPos] != '\r');

					if (str[currentPos] == '\n') {
						line++;
					}

					continue;
				}

				if (str[currentPos] == '-' && str[currentPos + 1] == '>') {
					currentPos += 2;
					ts.add(mrrt_rarrow);
					continue;
				}

				if (str[currentPos] == ':') {
					currentPos++;
					ts.add(mrrt_type_annot);
					continue;
				}

				// String
				if (str[currentPos] == '"') {
					std::string param = "";

					// Parse string
					currentPos++;
					while (str[currentPos] != '"') {
						param += str[currentPos++];
					}
					currentPos++;
					ts.add(mrrt_str, param);
					continue;
				}

				// Identifier (alphabetic char)
				if (str[currentPos] == '_' || isalpha(str[currentPos])) {
					std::string param = "";
					param += str[currentPos++];

					// Parse identifier
					while (isalnum(str[currentPos]) || str[currentPos] == '_') {
						param += str[currentPos++];
					}

					int token = parse_identifier_str(param);
					ts.add((mrr_token)token, token == mrrt_identifier ? param : "");
					continue;
				}

				// Number
				if (isdigit(str[currentPos])) {
					std::string numStr;
					size_t dotCnt = 0;

					// TODO: Fix for input where '.' entered multiple times
					do {
						if (str[currentPos] == '.') {
							dotCnt++;

							if (dotCnt > 1) {
								log_error("Lexer: multiple . separate number found at line %ld near %s", line, numStr);
							}
						}

						numStr += str[currentPos++];
					} while (isdigit(str[currentPos]) || str[currentPos] == '.');

					ts.add(mrrt_number, numStr);
					continue;
				}

				// Equal
				if (str[currentPos] == '=' && str[currentPos + 1] == '=') {
					currentPos += 2;
					ts.add(mrrt_equal);
					continue;
				}

				// Other
				if (str[currentPos] == EOF) {
					currentPos++;
					ts.add(mrrt_eof);
					continue;
				}

				ts.add((mrr_token)str[currentPos++]);
			}

			return ts;
		}

		mrr_token_stream::mrr_token_stream()
		{
		}

		void mrr_token_stream::print() {
			size_t idx = 0;

			std::cout << "=========================\n";

			for (const mrr_tse& tse : m_tses) {
				std::cout << std::setw(5) << idx << "| " << mrrl::token_to_str(tse.token) << " " << tse.param << "\n";
				idx += 1;
			}

			std::cout << "=========================\n";
		}
	}
}
