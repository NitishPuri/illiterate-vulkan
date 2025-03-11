#include <iostream>
#include <stack>

class Logger {
 public:
  static void logFunctionEntry(const char* functionName) { instance().logFunctionEntryImpl(functionName); }
  static void logFunctionExit() { instance().logFunctionExitImpl(); }
  static void log(const char* call) { return instance().logCallImpl(call); }
  static std::ostream& logStream() { return instance().logStreamImpl(); }

 private:
  Logger() = default;

  static Logger& instance() {
    static Logger logger;
    return logger;
  }

  void logFunctionEntryImpl(const char* functionName) {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
    }
    std::cout << functionName << " {" << std::endl;
    callStack.push(functionName);
  }

  void logFunctionExitImpl() {
    if (!callStack.empty()) {
      auto functioName = callStack.top();
      callStack.pop();
      for (int i = 0; i < callStack.size(); ++i) {
        std::cout << "  ";
      }
      std::cout << "}" << std::endl;
      // std::cout << "} // " << functioName << std::endl;
    }
  }

  void logCallImpl(const char* call) {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
    }
    std::cout << call << std::endl;
  }

  std::ostream& logStreamImpl() {
    for (int i = 0; i < callStack.size(); ++i) {
      std::cout << "  ";
    }
    return std::cout;
  }

  std::stack<const char*> callStack;
};

class FunctionLogger {
 public:
  FunctionLogger(const char* functionName) : functionName(functionName) { Logger::logFunctionEntry(functionName); }

  ~FunctionLogger() { Logger::logFunctionExit(); }

 private:
  const char* functionName;
};
#ifdef NDEBUG
#define LOGFN
#define LOGCALL(x) x
#define LOG(x)
#else
#define LOGFN FunctionLogger functionLogger(__FUNCTION__)
#define LOGCALL(x) \
  Logger::log(#x); \
  x
#define LOG Logger::logStream()
#endif
