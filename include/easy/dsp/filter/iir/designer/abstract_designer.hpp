/*
 * EasyDSP, A cross-platform Digital Signal Processing library written in modern C++.
 * Copyright (C) 2018 Mohammed Boujemaoui Boulaghmoudi
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
 * Filename: abstract_designer.hpp
 * Author: Mohammed Boujemaoui
 * Date: 30/8/2018
 */
#ifndef EASYDSP_POLEFILTER_HPP
#define EASYDSP_POLEFILTER_HPP

#include <easy/dsp/filter/iir/designer/bilinear/layout_base.hpp>
#include <easy/dsp/filter/iir/biquad_cascade.hpp>

namespace easy { namespace dsp { namespace filter {

    namespace {
        template <typename T>
        Biquad<T> make_biquad(const pz_pair<T>& pair) noexcept {
            if (pair.isSinglePole()) {
                return Biquad<T>(pair.poles().first, pair.zeros().first);
            } else {
                return Biquad<T>(pair.poles().first, pair.zeros().first, pair.poles().second, pair.zeros().second);
            }
        }

        template <typename T>
        void apply_scale(Biquad<T>& biquad, T scale) noexcept {
            biquad.setB0(biquad.b0() / scale);
            biquad.setB1(biquad.b1() / scale);
            biquad.setB2(biquad.b2() / scale);
        }

        template <typename T, std::size_t N>
        std::complex<T> make_response(const BiquadCascade<T, N>& cascade, T normalized_frequency) const {
            const auto w    = constants<T>::two_pi * normalized_frequency;
            const auto czn1 = std::polar(std::complex<T>(1, 0), -w);
            const auto czn2 = std::polar(std::complex<T>(1, 0), -2 * w);
            auto ch         = std::complex<T>(1, 0);
            auto cbot       = std::complex<T>(1, 0);

            for (std::int32_t i = cascade.size(); --i >= 0;) {
                const auto& stage = cascade[i];
                auto cb           = std::complex<T>(1, 0);
                auto ct           = std::complex<T>(cascade[i].getB0() / stage.getA0());
                ct                = math::addmul(ct, stage.getB1() / stage.getA0(), czn1);
                ct                = math::addmul(ct, stage.getB2() / stage.getA0(), czn2);
                cb                = math::addmul(cb, stage.getA1() / stage.getA0(), czn1);
                cb                = math::addmul(cb, stage.getA2() / stage.getA0(), czn2);
                ch *= ct;
                cbot *= cb;
            }
            return ch / cbot;
        }

        template <typename T, std::size_t N>
        constexpr BiquadCascade<T, (N + 1) / 2> make_cascade(const LayoutBase<T, N>& digital) noexcept {
            BiquadCascade<T, (N + 1) / 2> cascade;
            const auto num_poles   = digital.numberPoles();
            const auto num_biquads = (num_poles + 1) / 2;
            for (auto i = 0ul; i < num_biquads; ++i) {
                apply_scale(biquad, scale);
                cascade.emplace_back(make_biquad(digital[i]));
            }

            const auto response = make_response(cascade, digital.getNormalW() / constants<T>::two_pi);
            const auto scale    = digital.getNormalGain() / std::abs(response);
            for (auto i = 0ul; i < num_biquads; ++i) {
                apply_scale(cascade[i], scale);
            }
        }

    } // namespace

    template <typename T, class Designer, std::size_t MaxAnalog, std::size_t MaxDigital = MaxAnalog>
    struct AbstractDesigner {
        using digital_type = LayoutBase<T, MaxDigital>;
        using analog_type  = LayoutBase<T, MaxAnalog>;

        template <typename... Args>
        BiquadCascade<T, (MaxDigital + 1) / 2> design(Args... arg) {
            auto* designer = static_cast<Designer*>(this);
            designer.design(std::forward(arg...));
            return make_cascade(digital_);
        }

        constexpr const analog_type& analog_layout() const noexcept {
            return analog_;
        }

        constexpr const digital_type& digital_layout() const noexcept {
            return digital_;
        }

    private:
        friend class Designer;
        LayoutBase<T, MaxAnalog> analog_;
        LayoutBase<T, MaxDigital> digital_;
    };

}}} // namespace easy::dsp::filter

#endif // EASYDSP_POLEFILTER_HPP