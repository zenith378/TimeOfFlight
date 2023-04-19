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
   Double_t points_translated[15];
   Double_t points_translated_bis[15];
   Double_t points[15] ={5,20,40,60,80,100,120,140,160,180,200,220,240,260,278};
   for(int i=0;i<15;i++) points_translated[i]=length/2-points[i];
   for(int i=0;i<15;i++) points_translated_bis[i]=length-points[i];
   Double_t err_x[15] ={3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
   Double_t t2_t1[15] = {18.68,16.76,13.71,11.01,8.304,5.869,3.465,0.8962,-1.826,-4.165,-6.579,-9.165,-11.89,-14.57,-16.54};
   //Double_t t2_t1[15] = {18.38};
   //for(int i=0;i<15;i++) t2_t1[i]=-t2_t1[i];
   Double_t t2_t1_error[15]={0.883/sqrt(234),0.824/sqrt(157),0.8645/sqrt(200),0.9996/sqrt(201),0.9941/sqrt(223),0.8626/sqrt(228),
                              0.7882/sqrt(216),0.9451/sqrt(220),0.9252/sqrt(226),0.7987/sqrt(240),0.8698/sqrt(242),0.8541/sqrt(240),0.9412/sqrt(245),0.8196/sqrt(245),1.039/sqrt(239)};
   //Double_t t2_t1_error[15]={1.948/sqrt(215)};
   Double_t t3_t1[15] = {-6.713,-7.826,-9.096,-10.37,-11.71,-12.98,-14.23,-15.57,-16.8,-17.93,-19.13,-20.49,-21.89,-23.21,-24.19};
   //Double_t t3_t1[15] = {-6.743};
   for(int i=0;i<15;i++) t3_t1[i]=-t3_t1[i];
   Double_t t3_t1_error[15]={0.653/sqrt(234),0.532/sqrt(157),0.5449/sqrt(200),0.512/sqrt(201),0.635/sqrt(223),0.588/sqrt(228),
                              0.6267/sqrt(216),0.6663/sqrt(220),0.7822/sqrt(226),0.634/sqrt(240),0.8147/sqrt(242),0.7766/sqrt(240),0.7383/sqrt(245),0.7529/sqrt(245),0.7661/sqrt(239)};
   //Double_t t3_t1_error[15]={1.153/sqrt(215)};
   Double_t t3_t2[15] = {-25.39,-24.5,-22.85,-21.31,-20.05,-18.85,-17.66,-16.48,-15.01,-13.85,-12.55,-11.25,-9.98,-8.652,-7.65};
   //Double_t t3_t2[15] = {-25.13};
   for(int i=0;i<15;i++) t3_t2[i]=-t3_t2[i];   
   Double_t t3_t2_error[15]={0.712/sqrt(234),0.767/sqrt(157),0.8169/sqrt(200),0.7373/sqrt(201),0.8503/sqrt(223),0.6779/sqrt(228),
                              0.6413/sqrt(216),0.6755/sqrt(220),0.6681/sqrt(226),0.6028/sqrt(240),0.638/sqrt(242),0.6209/sqrt(240),0.5313/sqrt(245),0.4705/sqrt(245),0.7964/sqrt(239)};
   //Double_t t3_t2_error[15]={1.261/sqrt(215)};

   Double_t tof_un[15]={-15.95,-16.03,-15.91,-15.8,-15.72,-15.89,-15.84,-15.9,-15.89,-15.83,-15.8,-15.78,-15.86,-15.8,-15.77};
   
   Double_t tof_un_error[15]={.7168/sqrt(209),0.7964/sqrt(130),.7977/sqrt(172),0.8258/sqrt(178),0.6184/sqrt(203),1.322/sqrt(212),
   0.654/sqrt(200),0.5816/sqrt(206),0.7522/sqrt(194),0.699/sqrt(211),0.7072/sqrt(219),0.8376/sqrt(214),0.6232/sqrt(221),
   0.7009/sqrt(219),0.8298/sqrt(220)};

   Double_t x[15]={-140.1,-124.9,-100.8,-80.26,-59.32,-39.65,-20.71,-1.322,19.05,38.37,57.21,77.34,98.74,119.5,136};
   Double_t x_error[15]={6.396/sqrt(195),6.892/sqrt(122),8.142/sqrt(166),7.32/sqrt(166),6.555/sqrt(189),6.705/sqrt(206),6.59/sqrt(192),
   6.856/sqrt(198),7.1/sqrt(179),6.48/sqrt(196),6.7/sqrt(201),6.59/sqrt(194),7.4/sqrt(212),6.8/sqrt(213),7.12/sqrt(208)};
   Double_t tof_un_cut[15]={-15.97,-16.11,-15.92,-15.86,-15.74,-15.93,-15.85,-15.89,-15.86,-15.86,-15.82,-15.81,-15.87,-15.84,-15.83};
   Double_t tof_un_cut_error[15]={0.5582/sqrt(195),0.6771/sqrt(122),0.7563/sqrt(166),0.545/sqrt(189),1.306/sqrt(206),0.5513/sqrt(192),
   0.54/sqrt(198),0.5525/sqrt(179),0.5959/sqrt(196),0.6151/sqrt(201),0.7246/sqrt(194),0.615/sqrt(212),0.575/sqrt(213),0.6019/sqrt(208),0.6019/sqrt(208)};


   //t2 t1
   auto c21 = new TCanvas();
   auto g21 = new TGraphErrors(15,points_translated,t2_t1,err_x,t2_t1_error);
   g21->SetTitle("Calibration curve;x_{centered};t_{2}-t_{1}");
   g21->Draw("AP");
   auto tp21 = new TPaveText(0.15, 0.7, 0.35, 0.85, "NDC");
   tp21->AddText("ToF");
   tp21->AddText(authors);
   tp21->AddText("Calibration curve 4/04/23"); 
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   //TF1* f = new TF1("f","[2]/[1]-2*x/[1]+[0]");
   //f->FixParameter(2,length);
   TF1* f21 = new TF1("f21","[0]+[1]*x");
   f21->SetParNames("offset1-offset2","2/v");

   g21->Fit(f21);
   f21->Draw("SAME");
   tp21->Draw();


      //t3 t1
   auto c31 = new TCanvas();
   auto g31 = new TGraphErrors(15,points,t3_t1,err_x,t3_t1_error);
   g31->SetTitle("Arrival time in t1;x;t_{1}-t_{3}");
   g31->Draw("AP");
   auto tp31 = new TPaveText(0.15, 0.7, 0.35, 0.85, "NDC");
   tp31->AddText("ToF");
   tp31->AddText(authors);
   tp31->AddText("Calibration curve 04/04/23"); 
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   //TF1* f = new TF1("f","[2]/[1]-2*x/[1]+[0]");
   //f->FixParameter(2,length);
   TF1* f31 = new TF1("f31","[0]+[1]*x");
   f31->SetParNames("offset1","1/v");

   g31->Fit(f31);
   f31->Draw("SAME");
   tp31->Draw();


      //t3 t2
   auto c32 = new TCanvas();
   auto g32 = new TGraphErrors(15,points_translated_bis,t3_t2,err_x,t3_t2_error);
   g32->SetTitle("Arrival time in t2;x_{inv};t_{2}-t_{3}");
   g32->Draw("AP");
   auto tp32 = new TPaveText(0.15, 0.7, 0.35, 0.85, "NDC");
   tp32->AddText("ToF");
   tp32->AddText(authors);
   tp32->AddText("Calibration curve 04/04/23"); 
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   //TF1* f = new TF1("f","[2]/[1]-2*x/[1]+[0]");
   //f->FixParameter(2,length);
   TF1* f32 = new TF1("f32","[0]+[1]*x");
   f32->SetParNames("offset2","1/v");

   g32->Fit(f32);
   f32->Draw("SAME");
   tp32->Draw();

   auto c_tof = new TCanvas();
   auto g_tof = new TGraphErrors(15,points,tof_un,err_x,tof_un_error);
   g_tof->SetTitle("ToF;x;ToF");
   g_tof->Draw("AP");
   tp32->Draw();
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   TF1* f_tof = new TF1("f_tof","[0]+[1]*x");
   f_tof->SetParNames("offset1","slope1");

   g_tof->Fit(f_tof);
   f_tof->Draw("SAME");

   auto c_tof_cut = new TCanvas();
   auto g_tof_cut = new TGraphErrors(15,x,tof_un_cut,err_x,tof_un_cut_error);
   g_tof_cut->SetTitle("ToF;x;ToF");
   g_tof_cut->Draw("AP");
   tp32->Draw();
   gStyle -> SetOptStat(0);
   gStyle->SetOptFit(111);
   TF1* f_tof_cut = new TF1("f_tof_cut","[0]+[1]*x");
   f_tof_cut->SetParNames("offset1","slope1");

   g_tof_cut->Fit(f_tof_cut);
   f_tof_cut->Draw("SAME");

}