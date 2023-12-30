#ifndef _WAVEREADER_HH_
#define _WAVEREADER_HH_

class WaveReader {
protected:
  void       Init();
  void       Clear();

  short      NumOfChannels;
  int        SampleRate;
  short      BitsPerSample;
  int        NumOfSamples;
  double     Peak2Peak;
  bool       DoNormalize;
  double**   Channel;
  double*    Time;

public:
  WaveReader();
  WaveReader(const char* file, int maxsmp=-1);
  virtual ~WaveReader();

  int        Open(const char* file, int maxsmp=-1);
  double*    GetData(int) const;
  double*    GetTimeAxis() const { return Time; }
  short      GetNchannels() const { return NumOfChannels; }
  int        GetNsamples() const { return NumOfSamples; }
  int        GetNbits() const { return BitsPerSample; }
  int        GetSampleRate() const { return SampleRate; }
  int        GetMaxPositive();
  long       GetMaxRange();
  void       Print() const;
  void       SetVpp(double Vpp = 2.);
  void       Scale(double coe);
};

#endif
