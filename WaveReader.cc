#include "WaveReader.hh"
#include <fstream>
#include <cstring>
#include <iostream>
#include <cmath>
using namespace std;

void WaveReader::Init(){
    NumOfChannels = 0;
    SampleRate = 0;
    BitsPerSample = 0;
    NumOfSamples = 0;
    Peak2Peak = 2.0;
    DoNormalize = false;
    Channel = 0x0;
    Time = 0x0;
}

void WaveReader::Clear(){
    if(Channel != 0x0){
        for(int i=0; i<NumOfChannels; ++i){
                delete [] Channel[i];
        }
        delete [] Channel;
        Channel = 0x0;
    }
}

WaveReader::WaveReader(){
    Init();
}

WaveReader::WaveReader(const char* file, int maxsmp){
    Init();
    Open(file, maxsmp);
}

int WaveReader::Open(const char* fileName, int maxsmp){
    ifstream file(fileName, ios::binary);

    char name[4];
    file.read(name, 4);
    if(!strstr(name, "RIFF")) return -1;

    int ChunkSize;
    file.read((char*)&ChunkSize, 4);

    char Format[4];
    file.read(Format, 4);
    if(!strstr(Format, "WAVE")) return -2;

    char Subchunk1ID[4];
    file.read((char*)&Subchunk1ID, 4);
    if (!strstr(Subchunk1ID,"fmt ")) return -3;

    int Subchunk1Size;
    file.read((char*)&Subchunk1Size, 4);

    short AudioFormat;
    file.read((char*)&AudioFormat, 2);

    file.read((char*)&NumOfChannels, 2);

    file.read((char*)&SampleRate, 4);

    int ByteRate;
    file.read((char*)&ByteRate, 4);

    short BlockAlign;
    file.read((char*)&BlockAlign, 2);

    file.read((char*)&BitsPerSample, 2);

    char Subchunk2ID[4];
    file.read(Subchunk2ID, 4);
    if (!strstr(Subchunk2ID,"data")) return -4;

    int Subchunk2Size;
    file.read((char*)&Subchunk2Size, 4);
    NumOfSamples = Subchunk2Size*8/BitsPerSample/NumOfChannels;

    cout << "NumOfChannels = " << NumOfChannels << endl;
    cout << "SampleRate = " << SampleRate << " Hz" << endl;
    cout << "BitsPerSample = " << BitsPerSample << endl;
    cout << "Number of Samples = " << NumOfSamples << endl<<endl;

    try {
        Time  = new double [NumOfSamples];
        Channel = new double* [NumOfChannels];
        for (int k=0; k<NumOfChannels; k++)
            Channel[k] = new double[NumOfSamples];
    }
    catch (bad_alloc&) {
        cout << "Error allocating memory: No enough free memory!" << endl;
        return -5;
    }

    int data;
    int MaxRange = pow(2,BitsPerSample);
    int MaxPositive = MaxRange/2 - 1;
    int SSize = BitsPerSample/8;
    bool Is2sComplement = BitsPerSample>=16;

    file.seekg(44, ios::beg);

    for (int k=0; k<NumOfSamples; k++) {
        for (int i=0; i<NumOfChannels; i++) {
        data = 0;
        file.read((char*)&data, SSize);
        if (Is2sComplement) {
            if (data>MaxPositive)
                Channel[i][k] = data - MaxRange;
            else
                Channel[i][k] = data;
        }
            else Channel[i][k] = data - MaxRange/2;
        }

        Time[k] = 1./SampleRate*k;
    }
    file.close();

    //In order if we create with default constructor and before reading set peak2peak (with SetVpp)
    if(DoNormalize == true){
        double coe = Peak2Peak/GetMaxRange();
        Scale(coe);
    }

    return 0;
}

double* WaveReader::GetData(int a) const{
    if(a<NumOfChannels)
        return Channel[a];
}

long WaveReader::GetMaxRange(){
    int maxRange = (long)1<<BitsPerSample;
    return maxRange;
}

int WaveReader::GetMaxPositive(){
    int maxRange = ((long)1<<(BitsPerSample-1))-1;
    return maxRange;
}

void WaveReader::Print() const{
    cout<<"Number of Channels: "<<GetNchannels()<<endl;
    cout<<"Number of Samples: "<<GetNsamples()<<endl;
    cout<<"Number of bits: "<<GetNbits()<<endl;
    cout<<"Sample rates: "<<GetSampleRate()<<endl;
}

void WaveReader::SetVpp(double Vpp){
    if(DoNormalize == false){
        Peak2Peak = Vpp;
        double coe = Peak2Peak/GetMaxRange(); //esaa scales coe-ficienti
        Scale(coe);
        DoNormalize = true;
    }
}

void WaveReader::Scale(double coe){
    for(int i=0; i<NumOfChannels; ++i){
        for(int j=0; j<NumOfSamples; ++j)
            Channel[i][j]*=coe;
    }
}

WaveReader::~WaveReader(){Clear();}


