#include <iostream>
#include <fstream>
#include <Graph.hh>
#include "WaveReader.hh"
#include <cmath>

using namespace std;

double cosFi(WaveReader wr){
    double Preal = 0;
    double Vrms = 0;
    double Irms = 0;
    for(int i=0; i<wr.GetNsamples(); ++i){
        Preal += -1 * (wr.GetData(1)[i] * wr.GetData(0)[i]);
        Vrms += (wr.GetData(1)[i] * wr.GetData(1)[i]);
        Irms += (wr.GetData(0)[i] * wr.GetData(0)[i]);
    }
    return Preal/sqrt(Vrms*Irms);
}

int main(){
//     WaveReader myFile("load.wav");
//     myFile.Print();
//     myFile.SetVpp(3.);
//     myFile.SetVpp(3.);

//     Graph g = ("MyGraph");
//     for(int i=0; i<myFile.GetNsamples(); ++i){
//         g.AddPoint(myFile.GetTimeAxis()[i], myFile.GetData(0)[i]);
//     }
//     g.Draw(0);

    WaveReader myF;
    myF.SetVpp(10.);
    myF.Open("load.wav");

    Graph gVol = ("Voltage_Graph");
    for(int i=0; i<myF.GetNsamples(); ++i){
        gVol.AddPoint(myF.GetTimeAxis()[i], myF.GetData(1)[i]);
    }

    Graph gCurr = ("Current_Graph");
    for(int i=0; i<myF.GetNsamples(); ++i){
        gCurr.AddPoint(myF.GetTimeAxis()[i], myF.GetData(0)[i]);
    }
    gVol.Draw(0);
    gCurr.Draw(0);
}


