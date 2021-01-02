#pragma once

template <typename To_Type, typename From_Type>
To_Type Cast(From_Type value) { return (To_Type)(value); }


//----------------------------------------------------------------------------//
// Disallow                                                                   //
//----------------------------------------------------------------------------//
//
// CTOR / DTOR
//
///-----------------------------------------------------------------------------
/// @brief Makes the constructor deleted.
#define ARK_DISALLOW_CTOR(_type_)  _type_() = delete;

///-----------------------------------------------------------------------------
/// @brief Makes the destructor deleted.
#define ARK_DISALLOW_DTOR(_type_) ~_type_() = delete;

///-----------------------------------------------------------------------------
/// @brief Makes both the constructor and destructor deleted.
#define ARK_DISALLOW_CTOR_DTOR(_type_) \
    ARK_DISALLOW_CTOR(_type_)          \
    ARK_DISALLOW_DTOR(_type_)

//
// Copy
//
///-----------------------------------------------------------------------------
/// @brief Makes the copy-constructor deleted.
#define ARK_DISALLOW_COPY_CTOR(_type_) \
    _type_(const _type_ &) = delete;

///-----------------------------------------------------------------------------
/// @brief Makes the copy-assign operator deleted.
#define ARK_DISALLOW_COPY_ASSIGN(_type_) \
    _type_& operator=(const _type_&) = delete;

///-----------------------------------------------------------------------------
/// @brief Makes both the copy-constructor and copy-assign operator deleted.
#define ARK_DISALLOW_COPY_CTOR_AND_COPY_ASSIGN(_type_) \
    ARK_DISALLOW_COPY_CTOR(_type_)                     \
    ARK_DISALLOW_COPY_ASSIGN(_type_)

//
// Move
//
///-----------------------------------------------------------------------------
/// @brief Makes the move-constructor deleted.
#define ARK_DISALLOW_MOVE_CTOR(_type_) \
    _type_(const _type_ &&) = delete;

///-----------------------------------------------------------------------------
/// @brief Makes the move-assign operator deleted.
#define ARK_DISALLOW_MOVE_ASSIGN(_type_) \
    _type_& operator=(const _type_&&) = delete;

///-----------------------------------------------------------------------------
/// @brief Makes both the move-constructor and move-assign operator deleted.
#define ARK_DISALLOW_MOVE_CTOR_AND_MOVE_ASSIGN(_type_) \
    ARK_DISALLOW_MOVE_CTOR(_type_)                     \
    ARK_DISALLOW_MOVE_ASSIGN(_type_)

//
// Everything
//
///-----------------------------------------------------------------------------
/// @brief
///    Makes the constructor, desctructor, copy-ctor, copy-assign,
///    move-ctor and move-assign deleted.
#define ARK_DISALLOW_EVERYTHING(_type_)            \
    ARK_DISALLOW_CTOR_DTOR(_type_)                 \
    ARK_DISALLOW_COPY_CTOR_AND_COPY_ASSIGN(_type_) \
    ARK_DISALLOW_MOVE_CTOR_AND_MOVE_ASSIGN(_type_)

#define ARK_STATIC_CLASS(_type_) \
        ARK_DISALLOW_EVERYTHING(_type_)

//----------------------------------------------------------------------------//
// Singleton                                                                  //
//----------------------------------------------------------------------------//
///-----------------------------------------------------------------------------
/// @brief
///   Makes the type a Singleton. So instead of type all of the code bellow for
///   each type that we want to make a singleton we can just type, lets say:
///     pw_SINGLETON_OF(Enemy);
///
///   Please notice that it expects that the user is reasonable with the type
///   passed in, it only work for the classes that you own.
///
///   Reference:
///     https://en.wikipedia.org/wiki/Singleton_pattern
#define ARK_SINGLETON_OF(_type_)                       \
    public:                                            \
        static _type_* Instance() {                    \
            static _type_ s_instance;                  \
            return &s_instance;                        \
        };                                             \
                                                       \
    /* CTOR / DTOR */                                  \
    private:                                           \
         _type_() = default;                           \
        ~_type_() = default;                           \
                                                       \
    public:                                            \
        /* Copy CTOR / Copy Assign */                  \
        _type_(const _type_&) = delete;                \
        _type_& operator=(const _type_&) = delete;     \
                                                       \
        /* Move CTOR / Move Assign */                  \
        _type_(_type_&&) = delete;                     \
        _type_& operator=(_type_&&) = delete;          \
                                                       \
    /* Let everything bellow be private by default */  \
    private:
