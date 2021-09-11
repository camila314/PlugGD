#include <string>
#include <thread>
namespace engine {
	bool runFile(char const* progname, std::string keybind);
	void init();
	void killPy();
	bool isRunning();
}
inline std::thread::id pyThreadID;