#include <string>

namespace engine {
	bool runFile(char const* progname);
	void init();
	void killPy();
	bool isRunning();
}
inline uint64_t pyThreadID;