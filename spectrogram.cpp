#include <iostream>
#include "spectrogram.h"
#include <cmath>


using namespace std;

Spectrogram::Spectrogram(TImageBuffer *in, WavFile *wav) : TImageProcessor(in){
    wavFile = wav;
}

void Spectrogram::setWaveFile(WavFile *file){
    wavFile = file;
}

void Spectrogram::process(){
    int i;
    int transform_size = 1024;
    int half = transform_size/2;
    int step_size = transform_size/2;
    double db;
    double in[transform_size];
    double processed[half];
    fftw_complex *out;
    fftw_plan p;

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * transform_size);


    for(int x=0; x < wavFile->getSamples()/step_size; x++){

        int j = 0;
        for(i = step_size*x; i < (x * step_size) + transform_size - 1; i++){
            in[j] = wavFile->getSample(i)/32767.9;
            j++;
        }

        //apply window function
        for(i = 0; i < transform_size; i++){
//            cout << in[i] << endl;
//            in[i] *= windowHanning(i, transform_size);
            in[i] *= windowBlackmanHarris(i, transform_size);
        }

        p = fftw_plan_dft_r2c_1d(transform_size, in, out, FFTW_ESTIMATE);

        fftw_execute(p); /* repeat as needed */

        for(i = 0; i < half; i++){
            out[i][0] *= (2./transform_size);
            out[i][1] *= (2./transform_size);
//            processed[i] = 10*log10(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
//            cout << processed[i] << endl;
            processed[i] = out[i][0]*out[i][0] + out[i][1]*out[i][1];
            processed[i] =10. * (log (processed[i] + 1e-6)/log(10)) /-60.;
//            cout << processed[i] << endl;
        }

        for (i = 0; i < half; i++){
            if(processed[i] > 0.99)
                processed[i] = 1;
//            cout << processed[i] << endl;
            In->setPixel(x,(half-1)-i,processed[i]*255);
        }


    }


    fftw_destroy_plan(p);
    fftw_free(out);
}

double Spectrogram::windowHanning(double n, double N){
    return (double)(0.5*(1-cos(2*M_PI*n/(N-1))));
}

double Spectrogram::windowBlackmanHarris(double n, double N){
    double a0, a1, a2, a3, seg1, seg2, seg3, w_n;

    a0 = 0.35875;
    a1 = 0.48829;
    a2 = 0.14128;
    a3 = 0.01168;

    seg1 = a1 * (double) cos((double)(2*M_PI*n)/(double)(N-1));
    seg2 = a2 * (double) cos((double)(4*M_PI*n)/(double)(N-1));
    seg3 = a3 * (double) cos((double)(6*M_PI*n)/(double)(N-1));

    w_n = a0 - seg1 + seg2 - seg3;

    //    cout << "before: " << (double) n << " => after: " << w_n << endl;
    return w_n;
}
