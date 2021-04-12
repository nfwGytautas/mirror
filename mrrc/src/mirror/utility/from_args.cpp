#include "from_args.hpp"

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "mirror/lexer/mrrl.hpp"
#include "mirror/compiler/mrrc.hpp"

#include "mirror/utility/log.hpp"

namespace mirror {

	struct args {
		std::vector<std::string> In;
		std::string Out;
	};

	args parse_args(int argc, char** argv) {
		args result = {};
		
		for (int i = 1; i < argc; i++) {
			log_info("Found option: %s", argv[i]);

			std::string astr = std::string(argv[i]);

			if (astr == "-in") {
				result.In.push_back(std::string(argv[i + 1]));
				i++;
			}
			else if (astr == "-out") {
				result.Out = std::string(argv[i + 1]);
				i++;
			}
		}

		return result;
	}

	bool validate_args(args& a) {
		if (a.In.size() == 0) {
			log_error("No .mr source given");
			return false;
		}

		if (a.Out == "") {
			a.Out = "a.exe";
		}

		return true;
	}

	void from_args(int argc, char** argv) {
		// Parse arguments
		args a = parse_args(argc, argv);
		if (!validate_args(a)) {
			log_error("Incorrect arguments, check console for more information");
			return;
		}

		// Create mirror compiler and compile file to llvm IR
		compiler::mrrc_init();
		compiler::mrrc* c = compiler::mrrc_new();
		log_info("Mirror compiler initiated:");
		log_info("Beginning mirror compilation:");

		std::string llvmOut = std::filesystem::path(a.Out).stem().string() + ".o";

		for (const auto& inFile : a.In) {
			log_info("Compiling: '%s'", inFile.c_str());
			lexer::mrrl* l = lexer::from_file(inFile);

			if (!compiler::compile(c, l)) {
				log_error("Failed to compile '%s' check console for more information", inFile.c_str());
				compiler::free_mrrc(c);
				lexer::free_mrrl(l);
				return;
			}

			lexer::free_mrrl(l);
		}

		log_info("Emitting to: '%s'", llvmOut.c_str());
		compiler::emit_to_file(c, llvmOut);

		log_info("CLANG compilation:");

		// Invoke clang
		system(std::string("clang " + llvmOut + " -o " + a.Out).c_str());

		// Cleanup
		std::filesystem::remove(llvmOut);

		log_info("CLANG compilation complete result stored in: '%s'", a.Out.c_str());
	}

}
