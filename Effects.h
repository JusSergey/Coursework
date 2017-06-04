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
    static void Normalize(AudioBuffersSimple &buffer, float ampl, real in = -1, real to = -1);
    static void ReverseY (AudioBuffersSimple &buffer, real in = -1, real to = -1);
    static void ReverseX (AudioBuffersSimple &buffer, real in = -1, real to = -1);
    static void FadeIn   (AudioBuffersSimple &buffer, float force, float ampl, real in = -1, real to = -1, real sz = -1);
    static void FadeOut  (AudioBuffersSimple &buffer, float force, float ampl, real in = -1, real to = -1, real sz = -1);

    static void NoiseLogarithmic(AudioBuffersSimple &buffer, float force, real in = -1, real to = -1);

    static void Sinosyde(AudioBuffersSimple &buffer, float force, float ampl, float hz, real in = -1, real to = -1);

};

#endif // FUNCTIONS_H
