#include <string>
#include <sstream>

class SyntaxError : public std::exception {
	private:
		int line;
		std::string message;
		std::string res;

	public:
		SyntaxError(int line, const char* message) : line(line) {
			std::stringstream stream;
			stream << "Syntax error at line " << line << ": " << message << "\n";
			this->message = stream.str();
		}

		const char* what() const throw() {
			return message.c_str();
		}

};

class RuntimeError : public std::exception {
private:
	std::string message;

public:
	const char* what() const throw() {
		std::stringstream stream;
		stream << "Runtime Error: " << message << std::endl;
		return stream.str().c_str();
	}

};

