#include "mirror/lexer/mrrl.hpp"

#include <fstream>

namespace mirror {

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

			if (ls->IdentifierStr == "fn") {
				return mrrt_fn;
			}

			if (ls->IdentifierStr == "var") {
				return mrrt_var;
			}

			if (ls->IdentifierStr == "return") {
				return mrrt_ret;
			}

			if (ls->IdentifierStr == "loop") {
				return mrrt_loop;
			}

			if (ls->IdentifierStr == "match") {
				return mrrt_match;
			}

			if (ls->IdentifierStr == "type") {
			    return mrrt_typedef;
			}

			return mrrt_identifier;
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
}
