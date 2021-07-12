#ifdef _DEBUG
#include "mirror/utility/from_debug.hpp"

int main(int argc, char** argv) {
	mirror::from_debug(argc, argv);
	//mirror::from_args(argc, argv);
	return 0;
}

#else
#include "mirror/utility/from_args.hpp"

int main(int argc, char** argv) {
	mirror::from_args(argc, argv);
	return 0;
}
#endif
