#pragma once

#include <string>

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
}
