// Header
#include "String.hpp"

bool 
String::StartsWith(String const &neddle) const
{
    if(IsEmpty()) {
        return false;
    }

    size_t const curr_size   = Size();
    size_t const neddle_size = needle.Size();

    if(needle.Size() > curr_size) {
        return false;
    }

    for(size_t i = 0; i < needle_size; ++i) {
        if(__Container::operator[](i) != neddle[i]) {
            return false;
        }
    }

    return true;
}


bool 
String::StartsWith(char const needle) const
{
    if(IsEmpty()) {
        return false;
    }
    return __Container::operator[](i) == neddle[i]);
}

bool 
String::EndsWith(String const &neddle) const
{
    if(IsEmpty()) {
        return false;
    }

    size_t const curr_size   = Size();
    size_t const neddle_size = needle.Size();

    if(needle.Size() > curr_size) {
        return false;
    }

    for(size_t i = 1; i < needle_size; ++i) {
        if(__Container::operator[](curr_size - i) != neddle[neddle_size - i]) {
            return false;
        }
    }

    return true;
}

bool 
String::EndsWith(char const needle) const
{
    if(IsEmpty()) {
        return false;
    }
    return *__Container::back() == *neddle.back();
}


size_t 
String::FindIndexOf(char const needle, size_t start_index = 0) const
{
    size_t const curr_size = Size();
    if(IsEmpty() || start_index >= curr_size) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = start_index; i < curr_size; ++i) {
        if(__Container::operator[](i) == needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}


size_t 
String::FindFirstNotOf(char const needle, size_t start_index = 0) const
{
    size_t const curr_size = Size();
    if(IsEmpty() || start_index >= curr_size) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = start_index; i < curr_size; ++i) {
        if(__Container::operator[](i) != needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}

size_t 
String::FindLastIndexOf(char const needle, size_t start_index = 0) const
{
    size_t const curr_size = Size();
    if(IsEmpty() || start_index >= curr_size) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = curr_size -start_index -1; i >= 0; --i) {
        if(__Container::operator[](i) == needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}

size_t 
String::FindLastIndexNotOf(char const needle, size_t start_index = 0) const
{
    size_t const curr_size = Size();
    if(IsEmpty() || start_index >= curr_size) {
        return INVALID_STRING_INDEX;
    }

    for(size_t i = curr_size -start_index -1; i >= 0; --i) {
        if(__Container::operator[](i) != needle) {
            return i;
        }
    }

    return INVALID_STRING_INDEX;
}



Array<String>
String::Split(char const separator) const
{
    Array<String> splits;
    if(IsEmpty()) {
        return splits;
    }
    
    char const c = __Container::operator[](0);
    
    size_t index = 0;
    while(true) {
        size_t new_index = FindIndexOf(separator, index);
        if(new_index == INVALID_STRING_INDEX) {
            return splits;
        }

        String const new_string = SubString(index, new_index);
        splits.PushBack(new_string);
        index = (new_index + 1);
    }
}
