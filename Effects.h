#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "PCMAudioDevice.h"
#include "AudioUtilityes.h"
#include <QtWidgets>
#include <QtMultimedia>

//typedef int16_t type;
typedef double real;

class Effects
{
public:
    static void Normalize(AudioBuffersSimple &buffer, float ampl, real in, real to);
    static void ReverseY (AudioBuffersSimple &buffer, real in, real to);
    static void ReverseX (AudioBuffersSimple &buffer, real in, real to);
    static void FadeIn   (AudioBuffersSimple &buffer, float force, float ampl, real in, real to, real sz);
    static void FadeOut  (AudioBuffersSimple &buffer, float force, float ampl, real in, real to, real sz);

    static void NoiseLogarithmic(AudioBuffersSimple &buffer, float force, real in, real to);

    static void Sinosyde(AudioBuffersSimple &buffer, float force, float ampl, float hz, real in, real to);

    static void Cut(AudioBuffersSimple &buffer, real in, real to);
    static void Crop(AudioBuffersSimple &buffer, real in, real to);

};

#endif // FUNCTIONS_H
