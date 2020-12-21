#pragma once

//
#include "String.hpp"


struct Error
{
    static int const UNKNOWN_ERROR = -1;
    static int const NO_ERROR      = 0;

    int    error_code;
    String error_msg;

    Error(int error_code = NO_ERROR, String const &error_msg = "")
        : error_code(error_code)
        , error_msg (error_msg)
    {
        // Empty...
    }
};

template <typename Value_Type, typename Error_Type = Error>
class Result
{
public:
    Value_Type value;

public:
    bool               HasSucceeded() const { return  _success; }
    bool               HasFailed   () const { return !_success; }
    Error_Type const & GetError    () const { return  _error;   }
    
    static Result<Value_Type, Error_Type> 
    Success(Value_Type const &value) 
    {
        return Result<Value_Type, Error_Type>(value);
    }

    static Result<Value_Type, Error_Type> 
    Fail(Error_Type const &error) 
    {
        return Result<Value_Type, Error_Type>(error);
    }

private:
    Result(Value_Type const &value) :  value(value), _success(true ) {}
    Result(Error_Type const &error) : _error(error), _success(false) {}

private:
    bool       const _success;
    Error_Type const _error;
}; // class Result
