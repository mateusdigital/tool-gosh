#pragma once

// std
#include <vector>
// Arkadia
#include "BasicTypes.hpp"
#include "Algo.hpp"

namespace ark {

template <typename Item_Type>
class Array
    : public std::vector<Item_Type>
{
private:
    typedef std::vector<Item_Type> __Container;
public:
    typedef __Container::iterator       Iterator;
    typedef __Container::const_iterator Const_Iterator;

    static const size_t InvalidIndex = NumericLimits<size_t>::Max;

public:
    static Array<Item_Type>
    CreateWithCapacity(size_t const capacity)
    {
        Array<Item_Type> s;
        s.Reserve(capacity);
        return s;
    }

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

    void   Reserve(size_t const size)       { __Container::reserve(size); }
    void   Clear  ()                        { __Container::clear();        }
    bool   IsEmpty()                  const { return __Container::empty(); }
    size_t Count  ()                  const { return __Container::size (); }

    size_t
    IndexOf(Item_Type const &item) const
    {
        for(size_t i = 0, count = Count(); i < count; ++i) {
            if(__Container::operator[](i) == item) {
                return i;
            }
        }

        return Array<Item_Type>::InvalidIndex;
    }
    template <typename Func_Type>
    size_t
    IndexOf(Func_Type const &func) const
    {
        for(size_t i = 0, count = Count(); i < count; ++i) {
            if(func(__Container::operator[](i))) {
                return i;
            }
        }

        return Array<Item_Type>::InvalidIndex;
    }

    enum class RemoveOptions {
        PreserveOrder,
        IgnoreOrder
    };

    void
    RemoveAt(
        size_t        const index,
        RemoveOptions const &remove_options = RemoveOptions::PreserveOrder)
    {
        if(remove_options == RemoveOptions::IgnoreOrder) {
            std::swap(__Container::operator[](index), __Container::back());
            __Container::pop_back();
        }
        else {
            __Container::erase(Begin() + index);
        }
    }


    Item_Type       & Back()       { return __Container::back(); }
    Item_Type const & Back() const { return __Container::back(); }

    Item_Type       & Front()       { return __Container::front(); }
    Item_Type const & Front() const { return __Container::front(); }



    Iterator       begin() noexcept       { return __Container::begin(); }
    Const_Iterator begin() const noexcept { return __Container::begin(); }
    Iterator       end  () noexcept       { return __Container::end  (); }
    Const_Iterator end  () const noexcept { return __Container::end  (); }


    Iterator       Begin() noexcept       { return __Container::begin(); }
    Const_Iterator Begin() const noexcept { return __Container::begin(); }
    Iterator       End  () noexcept       { return __Container::end  (); }
    Const_Iterator End  () const noexcept { return __Container::end  (); }

}; // class Array
}  // namespace ark
