#pragma once

#include <cstdint>
#include <type_traits>
#include <new>
#include <span>

class Pixel {
    std::uint32_t m_value{};
public:
    constexpr Pixel() = default;
    constexpr Pixel(const std::uint8_t red,
        const std::uint8_t green,
        const std::uint8_t blue,
        const std::uint8_t alpha = 0xff) noexcept {
        this->m_value = 0;
        this->set_red(red);
        this->set_green(green);
        this->set_blue(blue);
        this->set_alpha(alpha);
    }
    constexpr std::uint8_t get_red() const noexcept {
        return static_cast<std::uint8_t>((this->m_value >> 0) & 0xffU);
    }
    constexpr std::uint8_t get_green() const noexcept {
        return static_cast<std::uint8_t>((this->m_value >> 8) & 0xffU);
    }
    constexpr std::uint8_t get_blue() const noexcept {
        return static_cast<std::uint8_t>((this->m_value >> 16) & 0xffU);
    }
    constexpr std::uint8_t get_alpha() const noexcept {
        return static_cast<std::uint8_t>((this->m_value >> 24) & 0xffU);
    }
    constexpr void set_red(const std::uint8_t val) & noexcept {
        this->m_value |= static_cast<std::uint32_t>(val) << 0;
    }
    constexpr void set_green(const std::uint8_t val) & noexcept {
        this->m_value |= static_cast<std::uint32_t>(val) << 8;
    }
    constexpr void set_blue(const std::uint8_t val) & noexcept {
        this->m_value |= static_cast<std::uint32_t>(val) << 16;
    }
    constexpr void set_alpha(const std::uint8_t val) & noexcept {
        this->m_value |= static_cast<std::uint32_t>(val) << 24;
    }
};

inline std::span<const std::uint8_t> view_as_bytes(const std::span<const Pixel> pixels) {
    static_assert(std::is_trivially_copyable<Pixel>::value, "Not safe to reinterpret");
    static_assert(sizeof(Pixel) == 4, "Can\'t be reinterpreted as RGBA");
    const auto as_bytes = std::launder(reinterpret_cast<const std::uint8_t*>(pixels.data()));
    return std::span<const std::uint8_t>{ as_bytes, pixels.size_bytes() };
}
