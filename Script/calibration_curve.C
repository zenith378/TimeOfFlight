#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include <iostream>
#include "TGraphErrors.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"

void calibration_curve(){
   TString authors = "G. Cordova, A. Giani";
   Double_t length=280;
   Double_t points[14] ={10,30,50,70,90,110,130,150,170,190,210,230,260,268};
   for(int i=0;i<14;i++) points[i]=length/2-points[i];
   Double_t err_x[14] ={3,3,3,3,3,3,3,3,3,3,3,3,3,3};
   Double_t time[14] = {19.32,16.57,13.71,10.73,7.817,5.28,2.43,-0.03,-2.54,-5.258,-8.016,-10.75,-14.95,-16.08};
   Double_t time_error[14]={1.3,1.9,1.3,1.9,1.89,1.6,1.3,1.3,1.7,1.18,1.423,1.56,1.716,1.46};

   auto c1 = new TCanvas();
   auto g1 = new TGraphErrors(14,points,time,err_x,time_error);
   g1->SetTitle("");
   g1->Draw("AP");
   auto tp2 = new TPaveText(0.15, 0.7, 0.35, 0.85, "NDC");
   tp2->AddText("ToF");
   tp2->AddText(authors);
   tp2->AddText("Calibration curve 30/03/23"); 
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   //TF1* f = new TF1("f","[2]/[1]-2*x/[1]+[0]");
   //f->FixParameter(2,length);
   TF1* f = new TF1("f","[0]+[1]*x");
   f->SetParNames("offset","ang coef");

   g1->Fit(f);
   f->Draw("SAME");
   tp2->Draw();

}