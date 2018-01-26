#include <cmath>

class Wavetable {
 public:
    Wavetable(int size = 1024);
    ~Wavetable();
    int getLength() {return m_length;}

    double operator()(int index);

 private:
    int m_length;
    double* m_wavetable;
};
