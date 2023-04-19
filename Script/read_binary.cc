#include "ReadWD.cc"
#include "ReadWD.hh"
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include <iostream>

using namespace ROOT;
void read_binary()
{
    const char *path_name = "../Dati/Binary/";
    const char *path_root = "../Dati/Root/";
    const char *filename = "40.dat";
    char run_string[256];
    strcpy(run_string, path_name);
    strcat(run_string, filename);

    char rootfile[256];
    char rootfile_tmp[256];

    strcpy(rootfile_tmp, filename);
    for (int i = 0; i < strlen(rootfile_tmp); i++)
        if (rootfile_tmp[i] == '.')
        {
            rootfile_tmp[i] = 0;
            break;
        }
    strcat(rootfile, path_root);
    strcat(rootfile, rootfile_tmp);
    strcat(rootfile, ".root");
    TFile *outfile = new TFile(rootfile, "RECREATE");

    // float time1, time2, time3, tof;
    float CF = 0.15;
    float offset1 = 6.538;
    float offset2 = 7.356;
    float inv_velocity = 0.06414; //
    float half_lenght = 140;      // cm

    DAQFile file(run_string);
    DRSEvent event;

    file.Initialise();

    //define vector of length of events and appen
    Float_t time1_v[1024];
    Float_t time2_v[1024];
    Float_t time3_v[1024];
    Float_t v1_v[1024];
    Float_t v2_v[1024];
    Float_t v3_v[1024];
 
    Double_t rise1;
    Double_t rise2;
    Double_t rise3;

    TTree *rec = new TTree("rec", "rec");
    rec->Branch("t1", &time1_v, "t1[1024]/F");
    rec->Branch("t2", &time2_v, "t2[1024]/F");
    rec->Branch("t3", &time3_v, "t3[1024]/F");
    rec->Branch("v1", &v1_v, "v1[1024]/F");
    rec->Branch("v2", &v2_v, "v2[1024]/F");
    rec->Branch("v3", &v3_v, "v3[1024]/F");

    rec->Branch("rise1", &rise1, "rise1/D");
    rec->Branch("rise2", &rise2, "rise2/D");
    rec->Branch("rise3", &rise3, "rise3/D");


    while (file >> event)
    {   /*store in time the times from DRS*/
        auto time1 = event.GetChannel(0, 0).GetTimes();
        auto time2 = event.GetChannel(0, 1).GetTimes();
        auto time3 = event.GetChannel(0, 2).GetTimes();
        auto v1 = event.GetChannel(0, 0).GetVolts();
        auto v2 = event.GetChannel(0, 1).GetVolts();
        auto v3 = event.GetChannel(0, 2).GetVolts();
        for(int i=0;i<1024;i++){
            time1_v[i]=time1[i];
            time2_v[i]=time2[i];
            time3_v[i]=time3[i];
            v1_v[i]=v1[i];
            v2_v[i]=v2[i];
            v3_v[i]=v3[i];
        }

        rise1 = event.GetChannel(0, 0).GetRiseTime();
        rise2 = event.GetChannel(0, 1).GetRiseTime();
        rise3 = event.GetChannel(0, 2).GetRiseTime();

        rec->Fill();


    }
    //ROOT::EnableImplicitMT();
    RDataFrame d(*rec);
   // auto df=d.Filter("timeCF1_15>0")
    //         .Filter("timeCF2_15>0")
    //        .Filter("timeCF3_15>0")
    //         .Filter("timeCF1_50>0")
    //         .Filter("timeCF2_50>0");
             //.Filter("timeCF3_50>0")
             //.Define("tof15_cor", "timeCF3_15-(timeCF1_15 + timeCF2_15)/2 + 140*0.06414+(6.538 + 7.356)/2")
             //.Define("tof50_cor", "timeCF3_50-(timeCF1_50 + timeCF2_50)/2 + 140*0.06414+(6.538 + 7.356)/2")
             //.Define("tof15", "timeCF3_15-(timeCF1_15 + timeCF2_15)/2 + 140*0.06414")
             //.Define("tof50", "timeCF3_50-(timeCF1_50 + timeCF2_50)/2")
             //.Define("x_50","(7.356-6.538-timeCF2_50+timeCF1_50)/(2*0.06414)")
             //.Define("x_cor","(7.356-6.538-timeCF2_15+timeCF1_15)/(2*0.06414)");
    d.Snapshot("Events", rootfile);
    outfile->Close();
    //auto d1 = df.Display({"x_15","x_50"},7000);
    //d1->Print();
}