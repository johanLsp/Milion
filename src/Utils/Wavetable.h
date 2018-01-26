#include <cmath>

class Wavetable {
 public:
    Wavetable(int size = 4096);
    ~Wavetable();
    int getLength() {return m_length;}
    void setWavetable(int index);


    double operator()(double position);

 private:
    int m_length;
    int m_waveformIndex;
    double* m_wavetable[8];
};
