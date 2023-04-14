#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include <iostream>
#include "TString.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TStyle.h"

using namespace std;
using namespace ROOT::VecOps;




void TOF(){
   TString authors="G. Cordova, A. Giani";

   ROOT::EnableImplicitMT(); // Tell ROOT you want to go parallel
   ROOT::RDataFrame d("Events", "../Dati/Root/Run0long_df.root"); // Interface to TTree and TChain
   auto myHisto = d.Histo1D("TOF"); // This books the (lazy) filling of a histogram
   myHisto->Draw(); // Event loop is run here, upon first access to a result


   auto c_TOF = new TCanvas("c_TOF", "TOF", 950, 800);


   auto h_TOF = d.Histo1D({"h_TOF", "TOF",55,1,12}, "TOF");
   h_TOF->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_TOF->SetTitle("Time of Flight");
   h_TOF->GetYaxis()->SetTitle("Counts");
   h_TOF->GetXaxis()->SetTitle("Time difference [ns]");

   auto tp1 = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp1->AddText("ToF");
   tp1->AddText(authors);
   tp1->AddText("Run0 long 05/04/23 18:30-09:40");
   //tp1->AddText("Exact Method");
   h_TOF->DrawClone();
   tp1->Draw();

   auto df = d.Define("x","(val50_t1-val50_t2)/(2*0.06414)+140-(6.538-7.356)/(2*0.06414)")
              .Define("theta","atan((x-140)/106)")
              .Filter("146<x<148");
   
   auto c_x = new TCanvas("c_x", "x", 950, 800);


   auto h_x = df.Histo1D({"h_x", "x",100,-50,350}, "x");
   h_x->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_x->SetTitle("Distribution of hits along the bar");
   h_x->GetYaxis()->SetTitle("Counts");
   h_x->GetXaxis()->SetTitle("Position x on the bar");
   h_x->DrawClone();


   auto c_theta = new TCanvas("c_theta", "theta", 950, 800);


   auto h_theta = df.Histo1D({"h_theta", "theta",50,-1,1}, "theta");
   h_theta->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_theta->SetTitle("Distribution of theta");
   h_theta->GetYaxis()->SetTitle("Counts");
   h_theta->GetXaxis()->SetTitle("Angle #theta");
   h_theta->DrawClone("hist");
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(111);
   //TF1* f = new TF1("f","[0]*cos([1]*x+[2])+[3]");
   //h_theta->Fit(f);
   //f->Draw("SAME");


}