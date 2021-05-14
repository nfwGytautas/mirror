#pragma once

// Should only be included in visual studio or other IDE, this allows to debug code in editor

#include "mirror/utility/from_args.hpp"

namespace mirror {
	/**
	 * @brief Run mirror compiler on IDE
	*/
	void from_debug(int argc, char** argv) {
		char* arguments[] = {
			"", // Doesn't matter
			"-in",
			"../examples/type.mr",
			"-out",
			"bin/a.exe"
		};

		from_args(4, arguments);
	}
}
