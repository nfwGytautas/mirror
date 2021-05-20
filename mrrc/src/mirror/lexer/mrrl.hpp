#pragma once

#include <string>
#include <vector>
#include "mirror/utility/macros.hpp"

namespace mirror {

	/**
	 * @brief Mirror lexer tokens
	*/
	enum mrr_token {
		mrrt_eof = -1,

		mrrt_fn = -2,

		mrrt_identifier = -3,
		mrrt_number = -4,
		mrrt_str = -5,
		mrrt_var = -6,
		mrrt_ret = -7,
		mrrt_loop = -8,
		mrrt_match = -9,
		mrrt_rarrow = -10, // -> statement
		mrrt_type_annot = -11, // :
		mrrt_equal = -12,
		mrrt_typedef = -13,
	};

	namespace lexer {
		/**
		 * @brief When parsing a file
		*/
		struct mrr_lexer_stream {
			std::string Input = "";
			size_t StreamPosition = 0;
		};

		/**
		 * @brief Mirror lexer
		*/
		struct mrrl {
			std::string IdentifierStr = "";
			double Num = 0;
			int Curtok = 0;

			// Once specified this is owned by mrrl instance
			mrr_lexer_stream* In = nullptr;
		};

		/**
		 * @brief Returns next token from the lexer input stream
		 * @param ls Lexer state
		 * @return mrrt token instance
		*/
		int next_token(mrrl* ls);

		/**
		 * @brief Creates a lexer instance from the specified input file
		 * @param file File to create for
		 * @return mrrl instance, call free_mrrl once done
		*/
		mrrl* from_file(const std::string& file);

		/**
		 * @brief Free mirror lexer instance
		 * @param l Instance to free
		*/
		void free_mrrl(mrrl* l);

		/**
		 * @brief Returns the current lexer instance
		 * @return mrrl instance
		*/
		mrrl* get_current();

		/**
		 * @brief Sets the current lexer instance
		 * @param l mrrl instance to set
		*/
		void set_current(mrrl* l);
	}

	namespace lexer_v2 {

		/**
		 * @brief Single token stream entry
		*/
		struct mrr_tse {
			/**
			 * @brief Token of the entry
			*/
			mrr_token token;

			/**
			 * @brief Parameter as string
			*/
			std::string param;
		};

		/**
		 * @brief Stream of mrr_tse
		*/
		class mrr_token_stream {
		public:
			mrr_token_stream();

			void add(mrr_token token, const std::string& param = "") {
				m_tses.push_back({token, param});
			}

			/**
			 * @brief Prints the token stream into the standard output
			*/
			void print();

			mrr_tse& current() {
				return m_tses[m_tseCounter];
			}

			mrr_tse& next() {
				return m_tses[m_tseCounter + 1];
			}

			mrr_tse& prev() {
				return m_tses[m_tseCounter - 1];
			}

			void advance() {
				m_tseCounter++;
			}

			/**
			 * @brief Returns all token stream entries
			*/
			std::vector<mrr_tse> get_all() {
				return m_tses;
			}
		private:
			size_t m_tseCounter = 0;
			std::vector<mrr_tse> m_tses;
		};

		/**
		 * @brief Mirror lexer
		*/
		class mrrl {
		public:
			static mrrl* get();

			/**
			 * @brief Parses the specified file into a token stream
			 * @param file File to parse
			 * @return Token stream object instance
			*/
			mrr_token_stream parse_file(const std::string& file);

			/**
			 * @brief Returns the string representation of the specified token
			 * @param t Token to stringize
			*/
			static std::string token_to_str(mrr_token t);

			/**
			 * @brief Parses input string into token stream
			 * @param str String to parse
			 * @return Token stream object instance
			*/
			mrr_token_stream parse_string(const std::string& str);
		private:
			mrrl() {};
			mrrl(mrrl&) = delete;
		};

	}
}
