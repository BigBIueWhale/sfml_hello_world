#pragma once

#include <cstdint>

class pixel {
    std::uint32_t m_value{};
public:
    constexpr pixel() = default;
    constexpr pixel(const std::uint8_t red,
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
