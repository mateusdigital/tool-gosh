#pragma once

//
#include <vector>

template <typename Item_Type>
class Array
    : public std::vector<Item_Type>
{
private:
    typedef std::vector<Item_Type> __Container;

public:
    void PushBack(Item_Type const &t) { __Container::push_back(t); }

    bool   IsEmpty() const { return __Container::empty();  }
    size_t Size   () const { return __Container::size(); }


    __Container::iterator       begin() noexcept       { return __Container::begin(); }
	__Container::const_iterator begin() const noexcept { return __Container::begin(); }
	__Container::iterator       end  () noexcept       { return __Container::end  (); }
	__Container::const_iterator end  () const noexcept { return __Container::end  (); }

}; // class Array
