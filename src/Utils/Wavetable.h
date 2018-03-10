#ifndef MILION_MILIONWAVETABKE_H_
#define MILION_MILIONWAVETABKE_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

class Wavetable {
 public:
    Wavetable(int size = 4096);
    ~Wavetable();
    int getLength() {return m_length;}
    void setWavetable(int index);
    static std::string getName(int index);
    static int getNumWavetable() {return 7;}


    double operator()(double position);

 private:
    int m_length;
    int m_waveformIndex;
    double* m_wavetable[7];
};

#endif  // MILION_MILIONWAVETABKE_H_
