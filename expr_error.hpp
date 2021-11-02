#pragma once
#include<exception>
#include<stdexcept>
namespace expression {
    class divided_by_zero : public std::runtime_error { // base of all overflow-error exceptions
    public:
        using _Mybase = std::runtime_error;

        explicit divided_by_zero(const std::string& _Message) : _Mybase(_Message.c_str()) {}

        explicit divided_by_zero(const char* _Message) : _Mybase(_Message) {}

#if !_HAS_EXCEPTIONS
    protected:
        virtual void _Doraise() const override { // perform class-specific exception handling
            _RAISE(*this);
        }
#endif // !_HAS_EXCEPTIONS
    };
}