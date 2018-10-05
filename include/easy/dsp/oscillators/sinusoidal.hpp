/*
 * EasyDSP, A cross-platform Digital Signal Processing library written in modern C++.
 * Copyright (C) 2018 Mohammed Boujemaoui Boulaghmoudi, All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along withº
 * this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * Filename: sine.hpp
 * Author: Mohammed Boujemaoui
 * Date: 27/7/2018
 */
#ifndef EASYDSP_OSCILLATOR_SIN_HPP
#define EASYDSP_OSCILLATOR_SIN_HPP

#include <easy/dsp/oscillator.hpp>
#include <easy/dsp/math/constant.hpp>
#include <easy/dsp/math/numeric.hpp>
#include <cmath>

namespace easy { namespace dsp { namespace oscillators {

    /**
     * @brief the OscillatorType enum represents the different waveforms generated by tha available oscillators.
     */
    enum class OscillatorType { Sinusoidal, Square, Sawtooth, Triangular };

    /**
     * @class oscillator
     * @brief The oscillator class generates a periodic signal.
     *
     * An oscillator is a repeating waveform with a fundamental frequency and peak amplitude. Aside from the frequency
     * or pitch of the oscillator and its amplitude, one of the most important features is the shape of its waveform.
     *
     * @tparam T
     */

    template <typename T>
    class oscillator {
    public:
        using value_type = T;

        /**
         * @brief Creates an oscillator that generates a waveform with the configuration.
         *
         * @param amplitude Amplitude of the waveform.
         * @param samplerate The sampling frequency in Hz.
         * @param frequency The fundamental frequency of the signal (also known as pitch).
         * @param phase Phase shift in radians.
         */
        constexpr oscillator(value_type amplitude, value_type samplerate, value_type frequency,
                             value_type phase) noexcept;
        /**
         * @brief Returns the fundamental frequency in Hz.
         * @return Fundamental frequency in Hz.
         */
        constexpr value_type frequency() const noexcept;

        /**
         * @brief Sets the fundamental frequency of the periodic signal.
         * @param frequency Fundamental frequency in Hz.
         */
        constexpr void set_frequency(value_type frequency) noexcept;

        /**
         * @brief Returns the phase shift in radians.
         * @return Phase shift in radians,
         */
        constexpr value_type phase() const noexcept;

        /**
         * @brief Sets the phase shift of the periodic signal.
         * @param phase Phase shift in radians.
         */
        constexpr void set_phase(value_type phase) noexcept;

        /**
         * @brief Returns the current timestamp of the signal in seconds.
         * @return Timestamp of the periodic signal in secs.
         */
        constexpr value_type timestamp() const noexcept;

        /**
         * @brief Sets the current timestamp of the periodic signal.
         * @param timestamp Timestamp of the signal in secs.
         */
        constexpr void set_timestamp(value_type timestamp) noexcept;

        /**
         * @brief Returns the sampling frequency in Hz.
         * @return Sampling frequency in Hz.
         */
        constexpr value_type samplerate() const noexcept;

        /**
         * @brief Sets the sampling frequency
         * @param samplerate Sampling frequency in Hz.
         */
        constexpr void set_samplerate(value_type samplerate) noexcept;

        /**
         * @brief Returns the sampling period in secs.
         * @return Sampling period in secs.
         * @see frequency
         */
        constexpr value_type sampling_period() const noexcept;

        /**
         * @brief Returns the amplitude of the periodic signal.
         * @return Amplitude of the periodic signal.
         */
        constexpr value_type amplitude() const noexcept;

        /**
         * @brief Set the amplitude of the periodic signal.
         * @param amplitude Amplitude of the signal
         */
        constexpr void set_amplitude(value_type amplitude) noexcept;

        /**
         * @brief Reset the oscillator to the original state
         */
        constexpr void reset() noexcept;

    private:
        value_type amplitude_{0.};
        value_type timestamp_{0.};
        value_type samplerate_{0.};
        value_type sampling_period_{0.};
        value_type frequency_{1.};
        value_type phase_{0.};
    };

    /**
     * @class sin_oscillator
     * @brief The class %sin_oscillator generates a sinusoidal signal.
     *
     * The sine wave can be considered the most fundamental building block of sound. A cycle of a sine wave is \f$ \displaystyle 2 \pi \f$
     *  radians long and has a peak amplitude of \f$ \displaystyle +/-1  \f$.
     *
     * With a sample rate of 44100 cycles per second, and a required cycle length of 1 second, it will take 44100 samples to get from
     * 0 to \f$ \displaystyle 2\pi \f$ . In other words, we can determine the steps per cycle \f$ \displaystyle S \f$ from cycle length
     * \f$ \displaystyle T \f$:
     *
     * \f[
     *     {\displaystyle S=T\cdot F_{s}\,}
     * \f]
     *
     * Where \f$ {\displaystyle F_{s}} \f$ is the sample rate. Each step will therefore take the following amount in radians:
     *
     * \f[
     *     {\displaystyle \delta \phi ={\frac {2\pi }{T\cdot F_{s}}}}  or {\displaystyle {\frac {2\pi f}{F_{s}}}}
     * \f]
     *
     * Where \f$ {\displaystyle f} \f$, in the second result, is the same result in terms of frequency.
     */
    template <typename T>
    class sin_oscillator : public oscillator<T> {
    public:
        using value_type = T;

        /**
         * @brief Creates a sinusoidal oscillator that generates a waveform with the configuration.
         *
         * @param amplitude Amplitude of the waveform.
         * @param samplerate The sampling frequency in Hz.
         * @param frequency The fundamental frequency of the signal (also known as pitch).
         * @param phase Phase shift in radians.
         */
        constexpr sin_oscillator(value_type amplitude, value_type samplerate, value_type frequency,
                                 value_type phase) noexcept;

        /**
         * @brief Generates one step.
         * @return Returns the value of the current step.
         */
        constexpr value_type operator()();
    };

    template <typename T>
    constexpr sin_oscillator<T>::sin_oscillator(value_type amplitude, value_type samplerate, value_type frequency,
                                                value_type phase) noexcept :
        oscillator<T>(amplitude, samplerate, frequency, phase) {}

    template <typename T>
    constexpr typename sin_oscillator<T>::value_type sin_oscillator<T>::operator()() {
        const value_type result =
            std::sin(constants<value_type>::two_pi * oscillator<T>::frequency() * oscillator<T>::timestamp() +
                     oscillator<T>::phase());
        this->set_timestamp(oscillator<T>::timestamp() + oscillator<T>::sampling_period());
        return result * oscillator<T>::amplitude();
    }

    template <typename T>
    constexpr oscillator<T>::oscillator(value_type amplitude, value_type samplerate, value_type frequency,
                                        value_type phase) noexcept :
        amplitude_(amplitude),
        samplerate_(samplerate),
        sampling_period_(math::inv(samplerate)),
        frequency_(frequency),
        phase_(phase) {}

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::timestamp() const noexcept {
        return timestamp_;
    }

    template <typename T>
    constexpr void oscillator<T>::set_timestamp(value_type timestamp) noexcept {
        timestamp_ = timestamp;
    }

    template <typename T>
    constexpr void oscillator<T>::reset() noexcept {
        timestamp_ = 0;
    }

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::samplerate() const noexcept {
        return samplerate_;
    }

    template <typename T>
    constexpr void oscillator<T>::set_samplerate(value_type samplerate) noexcept {
        samplerate_      = samplerate;
        sampling_period_ = math::inv(samplerate_);
    }

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::frequency() const noexcept {
        return frequency_;
    }

    template <typename T>
    constexpr void oscillator<T>::set_frequency(value_type frequency) noexcept {
        frequency_ = frequency;
    }

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::phase() const noexcept {
        return phase_;
    }

    template <typename T>
    constexpr void oscillator<T>::set_phase(value_type phase) noexcept {
        phase_ = phase;
    }

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::amplitude() const noexcept {
        return amplitude_;
    }

    template <typename T>
    constexpr void oscillator<T>::set_amplitude(value_type amplitude) noexcept {
        amplitude_ = amplitude;
    }

    template <typename T>
    constexpr typename oscillator<T>::value_type oscillator<T>::sampling_period() const noexcept {
        return sampling_period_;
    }

}}} // namespace easy::dsp::oscillators

#endif // EASYDSP_OSCILLATOR_SIN_HPP
