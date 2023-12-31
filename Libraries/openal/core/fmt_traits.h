#ifndef CORE_FMT_TRAITS_H
#define CORE_FMT_TRAITS_H

#include <array>
#include <cstddef>
#include <stdint.h>

#include "buffer_storage.h"


namespace al {

extern const std::array<int16_t,256> muLawDecompressionTable;
extern const std::array<int16_t,256> aLawDecompressionTable;


template<FmtType T>
struct FmtTypeTraits { };

template<>
struct FmtTypeTraits<FmtUByte> {
    using Type = uint8_t;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept { return val*OutT{1.0/128.0} - OutT{1.0}; }
};
template<>
struct FmtTypeTraits<FmtShort> {
    using Type = int16_t;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept { return val*OutT{1.0/32768.0}; }
};
template<>
struct FmtTypeTraits<FmtInt> {
    using Type = int32_t;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept
    { return static_cast<OutT>(val)*OutT{1.0/2147483648.0}; }
};
template<>
struct FmtTypeTraits<FmtFloat> {
    using Type = float;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept { return val; }
};
template<>
struct FmtTypeTraits<FmtDouble> {
    using Type = double;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept { return static_cast<OutT>(val); }
};
template<>
struct FmtTypeTraits<FmtMulaw> {
    using Type = uint8_t;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept
    { return muLawDecompressionTable[val] * OutT{1.0/32768.0}; }
};
template<>
struct FmtTypeTraits<FmtAlaw> {
    using Type = uint8_t;

    template<typename OutT>
    static constexpr OutT to(const Type val) noexcept
    { return aLawDecompressionTable[val] * OutT{1.0/32768.0}; }
};


template<FmtType SrcType, typename DstT>
inline void LoadSampleArray(DstT *RESTRICT dst, const std::byte *src, const std::size_t srcstep,
    const std::size_t samples) noexcept
{
    using TypeTraits = FmtTypeTraits<SrcType>;
    using SampleType = typename TypeTraits::Type;

    const SampleType *RESTRICT ssrc{reinterpret_cast<const SampleType*>(src)};
    for(size_t i{0u};i < samples;i++)
        dst[i] = TypeTraits::template to<DstT>(ssrc[i*srcstep]);
}

} // namespace al

#endif /* CORE_FMT_TRAITS_H */
