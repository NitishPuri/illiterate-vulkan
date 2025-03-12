#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_set>

// extern constexpr bool LOG_TO_README;
extern const char* logFileName;  // = LOG_TO_README ? "README.md" : "log.hpp";

class Logger {
 public:
  static void logFunctionEntry(const char* functionName) { instance().logFunctionEntryImpl(functionName); }
  static void logFunctionExit() { instance().logFunctionExitImpl(); }

  template <typename... Args>
  static void log(Args... args) {
    instance().logImpl(args...);
  }

 private:
  Logger() : logFile(logFileName) {
    if (!logFile.is_open()) {
      std::cerr << "Failed to open log file" << std::endl;
    }

    if (LOG_TO_README) {
      logFile << "# Illiterate Vulkan" << std::endl;
      logFile << std::endl;
      logFile << "Vulkan tutorial unrolled." << std::endl;
      logFile << std::endl;
      logFile << "```cpp" << std::endl;
    }
  }

  ~Logger() {
    if (LOG_TO_README) {
      logFile << "```" << std::endl;
    }
    logFile.close();
  }

  static Logger& instance() {
    static Logger logger;
    return logger;
  }

  void logFunctionEntryImpl(const char* functionName) {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
      logFile << "  ";
    }
    std::cout << functionName << " {" << std::endl;
    logFile << functionName << " {" << std::endl;
    callStack.push(functionName);
  }

  void logFunctionExitImpl() {
    if (!callStack.empty()) {
      auto functioName = callStack.top();
      callStack.pop();
      for (int i = 0; i < callStack.size(); ++i) {
        std::cout << "  ";
        logFile << "  ";
      }
      std::cout << "}" << std::endl;
      logFile << "}" << std::endl;
    }
  }

  template <typename T>
  void logImpl(T arg) {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
      logFile << "  ";
    }
    std::cout << arg << std::endl;
    logFile << arg << std::endl;
  }

  template <typename T, typename... Args>
  void logImpl(T arg, Args... args) {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
      logFile << "  ";
    }
    std::cout << arg << " ";
    logFile << arg << " ";
    logImplRec(args...);
  }

  template <typename T, typename... Args>
  void logImplRec(T arg) {
    std::cout << arg << std::endl;
    logFile << arg << std::endl;
  }

  template <typename T, typename... Args>
  void logImplRec(T arg, Args... args) {
    std::cout << arg << " ";
    logFile << arg << " ";
    logImplRec(args...);
  }

  std::stack<const char*> callStack;
  std::ofstream logFile;
};

class FunctionLogger {
 public:
  FunctionLogger(const char* functionName) : functionName(functionName) { Logger::logFunctionEntry(functionName); }

  ~FunctionLogger() { Logger::logFunctionExit(); }

 private:
  const char* functionName;
};

class OneTimeLogger {
 public:
  OneTimeLogger(const std::string& functionName) : functionName(functionName) {
    if (loggedFunctions.find(functionName) == loggedFunctions.end()) {
      Logger::logFunctionEntry(functionName.c_str());
      loggedFunctions.insert(functionName);
      firstTime = true;
    }
  }

  ~OneTimeLogger() {
    if (firstTime) {
      Logger::logFunctionExit();
    }
  }

  template <typename... Args>
  void logOnce(Args... args) {
    if (firstTime) {
      Logger::log(args...);
    }
  }

 private:
  std::string functionName;
  bool firstTime = false;
  static std::unordered_set<std::string> loggedFunctions;
};

#ifdef NDEBUG
#define LOGFN
#define LOGCALL(x) x
#define LOG(...)
#define LOGFN_ONCE
#define LOG_ONCE(...)
#define LOGCALL_ONCE(x) x
#else
#define LOGFN FunctionLogger functionLogger(__FUNCTION__)
#define LOGCALL(x) \
  Logger::log(#x); \
  x
#define LOG(...) Logger::log("//", __VA_ARGS__)
#define LOGFN_ONCE OneTimeLogger otl(__FUNCTION__)
#define LOG_ONCE(...) otl.logOnce("//", __VA_ARGS__)
#define LOGCALL_ONCE(x) \
  otl.logOnce(#x);      \
  x
#endif
