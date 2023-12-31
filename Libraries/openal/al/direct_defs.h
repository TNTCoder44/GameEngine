#ifndef AL_DIRECT_DEFS_H
#define AL_DIRECT_DEFS_H

namespace detail_ {

template<typename T>
constexpr T DefaultVal() noexcept { return T{}; }

template<>
constexpr void DefaultVal() noexcept { }

} // namespace detail_

#define DECL_FUNC(R, Name)                                                    \
auto AL_APIENTRY Name() noexcept -> R                                         \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get());                                       \
}

#define DECL_FUNC1(R, Name, T1)                                               \
auto AL_APIENTRY Name(T1 a) noexcept -> R                                     \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get(), a);                                    \
}

#define DECL_FUNC2(R, Name, T1, T2)                                           \
auto AL_APIENTRY Name(T1 a, T2 b) noexcept -> R                               \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get(), a, b);                                 \
}

#define DECL_FUNC3(R, Name, T1, T2, T3)                                       \
auto AL_APIENTRY Name(T1 a, T2 b, T3 c) noexcept -> R                         \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get(), a, b, c);                              \
}

#define DECL_FUNC4(R, Name, T1, T2, T3, T4)                                   \
auto AL_APIENTRY Name(T1 a, T2 b, T3 c, T4 d) noexcept -> R                   \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get(), a, b, c, d);                           \
}

#define DECL_FUNC5(R, Name, T1, T2, T3, T4, T5)                               \
auto AL_APIENTRY Name(T1 a, T2 b, T3 c, T4 d, T5 e) noexcept -> R             \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct(context.get(), a, b, c, d, e);                        \
}


#define DECL_FUNCEXT(R, Name,Ext)                                             \
auto AL_APIENTRY Name##Ext() noexcept -> R                                    \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get());                                  \
}

#define DECL_FUNCEXT1(R, Name,Ext, T1)                                        \
auto AL_APIENTRY Name##Ext(T1 a) noexcept -> R                                \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a);                               \
}

#define DECL_FUNCEXT2(R, Name,Ext, T1, T2)                                    \
auto AL_APIENTRY Name##Ext(T1 a, T2 b) noexcept -> R                          \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b);                            \
}

#define DECL_FUNCEXT3(R, Name,Ext, T1, T2, T3)                                \
auto AL_APIENTRY Name##Ext(T1 a, T2 b, T3 c) noexcept -> R                    \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b, c);                         \
}

#define DECL_FUNCEXT4(R, Name,Ext, T1, T2, T3, T4)                            \
auto AL_APIENTRY Name##Ext(T1 a, T2 b, T3 c, T4 d) noexcept -> R              \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b, c, d);                      \
}

#define DECL_FUNCEXT5(R, Name,Ext, T1, T2, T3, T4, T5)                        \
auto AL_APIENTRY Name##Ext(T1 a, T2 b, T3 c, T4 d, T5 e) noexcept -> R        \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b, c, d, e);                   \
}

#define DECL_FUNCEXT6(R, Name,Ext, T1, T2, T3, T4, T5, T6)                    \
auto AL_APIENTRY Name##Ext(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f) noexcept -> R  \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b, c, d, e, f);                \
}

#define DECL_FUNCEXT8(R, Name,Ext, T1, T2, T3, T4, T5, T6, T7, T8)            \
auto AL_APIENTRY Name##Ext(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, T7 g, T8 h) noexcept -> R \
{                                                                             \
    auto context = GetContextRef();                                           \
    if(!context) UNLIKELY return detail_::DefaultVal<R>();                    \
    return Name##Direct##Ext(context.get(), a, b, c, d, e, f, g, h);          \
}

#endif /* AL_DIRECT_DEFS_H */
