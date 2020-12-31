#pragma once

//
#include <vector>

#include <algorithm>
namespace Algo {

template <typename Dst_Type, typename Src_Type>
void Append(Dst_Type &dst, Src_Type const &src)
{
    dst.Reserve(dst.Count() + src.Count());
    std::copy (src.begin(), src.end(), std::back_inserter(dst));
}

}


template <typename Item_Type>
class Array
    : public std::vector<Item_Type>
{
private:
    typedef std::vector<Item_Type> __Container;

public:
    Array() = default;

    Array(std::initializer_list<Item_Type> const &init_list)
        : __Container(init_list)
    {
        // Empty...
    }


public:
    void PushBack(Array<Item_Type>                 const &t) { Algo::Append(*this, t); }
    void PushBack(std::initializer_list<Item_Type> const &t) { Algo::Append(*this, t); }

    void PushBack(Item_Type const &t) { __Container::push_back(t); }
    void PopBack ()                   { __Container::pop_back ();  }

    template<class... _Valty> void
    EmplaceBack(_Valty&&... _Val)
    {
        __Container::emplace_back(_Val...);
    }

    void Clear() { __Container::clear(); }
    void Reserve(size_t const size) { __Container::reserve(size); }

    bool   IsEmpty() const { return __Container::empty(); }
    size_t Count  () const { return __Container::size (); }

    Item_Type       & Back()       { return __Container::back(); }
    Item_Type const & Back() const { return __Container::back(); }

    __Container::iterator       begin() noexcept       { return __Container::begin(); }
	__Container::const_iterator begin() const noexcept { return __Container::begin(); }
	__Container::iterator       end  () noexcept       { return __Container::end  (); }
	__Container::const_iterator end  () const noexcept { return __Container::end  (); }

}; // class Array
