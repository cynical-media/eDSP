//
// Created by mohabouje on 18/02/18.
//
#include "frequency/dct.h"
using namespace edsp::frequency;

fftw_r2r_kind DCT::format() const noexcept {
    switch(DCT::t) {
        case Type_I: return FFTW_REDFT00;
        case Type_II: return FFTW_REDFT10;
        case Type_III: return FFTW_REDFT01;
        case Type_IV: return FFTW_REDFT11;
        default: return FFTW_REDFT00;
    }
}

DCT::DCT(edsp::frequency::DCT_Type t) :
        BaseTransform(),
        t(t)
{
    setType(t);
}

