#ifndef C0_EXCEPTION
#define C0_EXCEPTION

#include <exception>

#ifdef _MSC_VER
#define noexcept throw()
#endif

namespace C0{

	class ProcException : public ::std::exception{
	public:
		ProcException(const ProcException& other) noexcept : message(other.message){}
		explicit ProcException(const char* _message) noexcept : message(_message){
		}
		virtual ~ProcException() noexcept{}
		virtual const char* what() const noexcept{ return message; }
	private:
		const char* message;
	};
}

#endif
