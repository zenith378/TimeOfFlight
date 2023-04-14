
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TKey.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TStyle.h"

#include "TPaveText.h"

using namespace std;
using namespace ROOT::VecOps;




void tof_anal()
{
   //ROOT::EnableImplicitMT();
   // Open root file
   const char *path_root = "../Dati/Root/";
   const char *filename = "run0";
   const char *extension = ".root";
   char rootfile[256];
   strcpy(rootfile,path_root);
   strcat(rootfile,filename);
   strcat(rootfile,extension);


   ROOT::RDataFrame d("Events",rootfile);
   auto df =d.Filter("x_cor<160","x<400")
   .Filter("x_cor>-160","x>-400")
   //.Filter("x_50<400")
   //.Filter("x_50>-400")
   .Filter("tof15>-1000","tof>-1000")
   .Filter("tof15<1000","tof<1000")
   //.Filter("tof50>-1000")
   //.Filter("tof50<1000")
   .Filter("timeCF1_15 > 5","timeCF1_15 > 5")
   .Filter("timeCF2_15 > 5","timeCF2_15 > 5")
   .Filter("timeCF3_15 > 5","timeCF3_15 > 5")
   .Filter("Min(v1)>-0.43","not saturated 1")
   .Filter("Min(v2)>-0.43","not saturated 2")
   .Filter("Min(v3)>-0.43","not saturated 3")
   .Filter("Min(v1)<0","minimum under 0 for 1")
   .Filter("Min(v2)<0","minimum under 0 for 2")
   .Filter("Min(v3)<0","minimum under 0 for 3")
   .Filter("Max(v1)<0.2","maximum under 0.2 for 1")
   .Filter("Max(v2)<0.2","maximum under 0.2 for 2")
   .Filter("Max(v3)<0.2","maximum under 0.2 for 3")
   .Define("t21","timeCF2_15-timeCF1_15")
   .Define("t31","timeCF3_15-timeCF1_15")
   .Define("t32","timeCF3_15-timeCF2_15")
   .Filter("t31<50","t31<50")
   .Filter("t32<50","t32<50")
   //.Filter("tof15_cor<1.5","tof15<1.5")
   .Define("Min_v1","Min(v1)")
   .Define("Min_v2","Min(v2)")
   .Define("Min_v3","Min(v3)")
   .Filter("abs(rise1)<1000")
   .Filter("abs(rise2)<1000")
   .Filter("abs(rise3)<1000")
   .Filter("Min_v3<-0.15","Min_v3<-0.15")
   .Define("tof_un","timeCF3_15-(timeCF1_15+timeCF2_15)/2");
   auto cutrep=d.Report();
   cutrep->Print();

//   df_set.Snapshot("Events","../Dati/Root/Run0long_df.root");
   TString authors="G. Cordova, A. Giani";
   char position[1024] = "PMT 3 at ";
   strcat(position,filename);
   strcat(position," cm");
   char name_32[256]="../Plots/Calibration/";
   strcat(name_32,filename); 

   auto tp1 = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp1->AddText("ToF");
   tp1->AddText(authors);
   tp1->AddText(position);
   tp1->AddText("Calibration 4/4/23");

/*
   auto c_TOF_50 = new TCanvas("c_TOF_50", "TOF_50", 950, 800);

   auto h_TOF_50 = df.Histo1D({"h_TOF_50", "TOF_50",125,0,0}, "tof50");

   h_TOF_50->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_TOF_50->SetTitle("Time of Flight");
   h_TOF_50->GetYaxis()->SetTitle("Counts");
   h_TOF_50->GetXaxis()->SetTitle("Time difference [ns]");
   auto tp2 = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp2->AddText("ToF");
   tp2->AddText(authors);
   tp2->AddText("Run0 long 05/04/23 18:30-09:40");
   //tp2->AddText("Exact Method");
   h_TOF_50->DrawClone();
   tp2->Draw();
*/
  auto c_TOF_1_50 = new TCanvas("c_TOF_15_50", "TOF_15_50", 950, 800);

  auto g_TOF_1_50 = df.Graph("timeCF1_15","tof15_cor");
   g_TOF_1_50->SetTitle("ToF vs time1_15");
   g_TOF_1_50->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_1_50->GetXaxis()->SetTitle("time1_15 [ns]");
   g_TOF_1_50->DrawClone("AP");
   tp1->Draw();
   char n2[1024];
   strcpy(n2,name_32);
   strcat(n2,"_tof15_time1");
   char n2r[1024];
   strcpy(n2r,n2);
   strcat(n2,".png");
   strcat(n2r,".root");
   c_TOF_1_50->SaveAs(n2);
   c_TOF_1_50->SaveAs(n2r);

   auto c_TOF_2_50 = new TCanvas("c_TOF_2_50", "TOF_2_50", 950, 800);

   auto g_TOF_2_50 = df.Graph("timeCF2_15","tof15_cor");
   g_TOF_2_50->SetTitle("ToF vs time2_15");
   g_TOF_2_50->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_2_50->GetXaxis()->SetTitle("time2_15 [ns]");
   g_TOF_2_50->DrawClone("AP");
   tp1->Draw();
   char n3[1024];
   strcpy(n3,name_32);
   strcat(n3,"_tof15_time2");
   char n3r[1024];
   strcpy(n3r,n3);
   strcat(n3,".png");
   strcat(n3r,".root");
   c_TOF_2_50->SaveAs(n3r);
   c_TOF_2_50->SaveAs(n3);


   auto c_TOF_3_50 = new TCanvas("c_TOF_3_50", "TOF_3_50", 950, 800);

   auto g_TOF_3_50 = df.Graph("timeCF3_15","tof15_cor");
   g_TOF_3_50->SetTitle("ToF vs time3_15");
   g_TOF_3_50->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_3_50->GetXaxis()->SetTitle("time3_15 [ns]");
   g_TOF_3_50->DrawClone("AP");
   tp1->Draw();
   char n4[1024];
   strcpy(n4,name_32);
   strcat(n4,"_tof15_time3");
   char n4r[1024];
   strcpy(n4r,n4);
   strcat(n4,".png");
   strcat(n4r,".root");
   c_TOF_3_50->SaveAs(n4r);
   c_TOF_3_50->SaveAs(n4);



   auto c_TOF_x = new TCanvas("c_TOF_x", "TOF_x", 950, 800);


   auto g_TOF_x = df.Graph("x_cor","tof15_cor");
   g_TOF_x->SetTitle("ToF vs x");
   g_TOF_x->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_x->GetXaxis()->SetTitle("x_{15} [cm]");
   g_TOF_x->DrawClone("AP");
   tp1->Draw();
   char n5[1024];
   strcpy(n5,name_32);
   strcat(n5,"_tof15_x");
   char n5r[1024];
   strcpy(n5r,n5);
   strcat(n5,".png");
   strcat(n5r,".root");
   c_TOF_x->SaveAs(n5r);
   c_TOF_x->SaveAs(n5);
/*
   auto c_TOF_y = new TCanvas("c_TOF_y", "TOF_y", 950, 800);

   auto g_TOF_50_x = df.Graph("x_50","tof50");
   g_TOF_50_x->SetTitle("ToF vs x");
   g_TOF_50_x->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_50_x->GetXaxis()->SetTitle("x_{50} [cm]");
   g_TOF_50_x->DrawClone("AP");
*/
   auto c_x_y = new TCanvas("c_x_y", "x_y", 950, 800);

   auto h_x = df.Histo1D({"h_x", "x",100,0,0}, "x_cor");

   h_x->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_x->SetTitle("x");
   h_x->GetYaxis()->SetTitle("Counts");
   h_x->GetXaxis()->SetTitle("x [cm]");
   h_x->DrawClone();
   tp1->Draw();
   char n6[1024];
   strcpy(n6,name_32);
   strcat(n6,"_x");
   char n6r[1024];
   strcpy(n6r,n6);
   strcat(n6,".png");
   strcat(n6r,".root");
   c_x_y->SaveAs(n6r);
   c_x_y->SaveAs(n6);

   auto c_TOF_corr = new TCanvas("c_TOF_corr", "TOF_corr", 950, 800);

   auto h_TOF_corr = df.Histo1D({"h_TOF_corr", "TOF_corr",125,0,0}, "tof15_cor");

   h_TOF_corr->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_TOF_corr->SetTitle("Time of Flight");
   h_TOF_corr->GetYaxis()->SetTitle("Counts");
   h_TOF_corr->GetXaxis()->SetTitle("Time difference [ns]");
   h_TOF_corr->DrawClone();
   tp1->Draw();
   char n7[1024];
   strcpy(n7,name_32);
   strcat(n7,"_tof15_cor");
   char n7r[1024];
   strcpy(n7r,n7);
   strcat(n7,".png");
   strcat(n7r,".root");
   c_TOF_corr->SaveAs(n7r);
   c_TOF_corr->SaveAs(n7);

   auto c_21 = new TCanvas("c_21", "21", 950, 800);

   auto h_t21 = df.Histo1D({"h_t21", "t21",100,0,0}, "t21");

   h_t21->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t21->SetTitle("time difference 2-1");
   h_t21->GetYaxis()->SetTitle("Counts");
   h_t21->GetXaxis()->SetTitle("Time difference 2-1[ns]");
   h_t21->DrawClone();
   tp1->Draw();
   char n8[1024];
   strcpy(n8,name_32);
   strcat(n8,"_t21");
   char n8r[1024];
   strcpy(n8r,n8);
   strcat(n8,".png");
   strcat(n8r,".root");
   c_21->SaveAs(n8r);
   c_21->SaveAs(n8);

   auto c_31 = new TCanvas("c_31", "31", 950, 800);  

   auto h_t31 = df.Histo1D({"h_t31", "t31",100,0,0}, "t31");

   h_t31->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t31->SetTitle("time difference 3-1");
   h_t31->GetYaxis()->SetTitle("Counts");
   h_t31->GetXaxis()->SetTitle("Time difference 3-1[ns]");
   h_t31->DrawClone();
   tp1->Draw();
   char n9[1024];
   strcpy(n9,name_32);
   strcat(n9,"_t31");
   char n9r[1024];
   strcpy(n9r,n9);
   strcat(n9,".png");
   strcat(n9r,".root");
   c_31->SaveAs(n9r);
   c_31->SaveAs(n9);

   auto c_32 = new TCanvas("c_32", "32", 950, 800);

   auto h_t32 = df.Histo1D({"h_t32", "t32",100,0,0}, "t32");

   h_t32->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t32->SetTitle("time difference 3-2");
   h_t32->GetYaxis()->SetTitle("Counts");
   h_t32->GetXaxis()->SetTitle("Time difference 3-2[ns]");
   h_t32->DrawClone();
   tp1->Draw();
   char n10[1024];
   strcpy(n10,name_32);
   strcat(n10,"_t32");
   char n10r[1024];
   strcpy(n10r,n10);
   strcat(n10,".png");
   strcat(n10r,".root");
   c_32->SaveAs(n10r);
   c_32->SaveAs(n10);

   auto c_tof_un = new TCanvas("c_tof_un", "tof_un", 950, 800);

   auto h_tof_un = df.Histo1D({"h_tof_un", "tof_un",100,0,0}, "tof_un");

   h_tof_un->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_tof_un->SetTitle("Time of Flight uncorrected");
   h_tof_un->GetYaxis()->SetTitle("Counts");
   h_tof_un->GetXaxis()->SetTitle("ToF [ns]");
   h_tof_un->DrawClone();
   tp1->Draw();
   char n11[1024];
   strcpy(n11,name_32);
   strcat(n11,"_tof_un");
   char n11r[1024];
   strcpy(n11r,n11);
   strcat(n11,".png");
   strcat(n11r,".root");
   c_tof_un->SaveAs(n11r);
   c_tof_un->SaveAs(n11);

   auto c_TOF_t21 = new TCanvas("c_TOF_t21", "TOF_t21", 950, 800);

   auto g_TOF_t21 = df.Graph("t21","tof15_cor");

   g_TOF_t21->SetTitle("Time of Flight vs time difference 2-1");
   g_TOF_t21->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_t21->GetXaxis()->SetTitle("Time difference 2-1[ns]");
   g_TOF_t21->DrawClone("AP");
   tp1->Draw();
   char n12[1024];
   strcpy(n12,name_32);
   strcat(n12,"_TOF_t21");
   char n12r[1024];
   strcpy(n12r,n12);
   strcat(n12,".png");
   strcat(n12r,".root");
   c_TOF_t21->SaveAs(n12r);
   c_TOF_t21->SaveAs(n12);

   auto c_TOF_t31 = new TCanvas("c_TOF_t31", "TOF_t31", 950, 800);

   auto g_TOF_t31 = df.Graph("t31","tof15_cor");

   g_TOF_t31->SetTitle("Time of Flight vs time difference 3-1");
   g_TOF_t31->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_t31->GetXaxis()->SetTitle("Time difference 3-1[ns]");
   g_TOF_t31->DrawClone("AP");
   tp1->Draw();
   char n13[1024];
   strcpy(n13,name_32);
   strcat(n13,"_TOF_t31");
   char n13r[1024];
   strcpy(n13r,n13);
   strcat(n13,".png");
   strcat(n13r,".root");
   c_TOF_t31->SaveAs(n13r);
   c_TOF_t31->SaveAs(n13);

   auto c_TOF_t32 = new TCanvas("c_TOF_t32", "TOF_t32", 950, 800);

   auto g_TOF_t32 = df.Graph("t32","tof15_cor");

   g_TOF_t32->SetTitle("Time of Flight vs time difference 3-2");
   g_TOF_t32->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_t32->GetXaxis()->SetTitle("Time difference 3-2[ns]");
   g_TOF_t32->DrawClone("AP");
   tp1->Draw();
   char n14[1024];
   strcpy(n14,name_32);
   strcat(n14,"_TOF_t32");
   char n14r[1024];
   strcpy(n14r,n14);
   strcat(n14,".png");
   strcat(n14r,".root");
   c_TOF_t32->SaveAs(n14r);
   c_TOF_t32->SaveAs(n14);

   auto c_TOF_min1 = new TCanvas("c_TOF_min1", "TOF_min1", 950, 800);

   auto g_TOF_min1 = df.Graph("Min_v1","tof15_cor");

   g_TOF_min1->SetTitle("Time of Flight vs minimum 1");
   g_TOF_min1->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_min1->GetXaxis()->SetTitle("Minimum 1 [mV]");
   g_TOF_min1->DrawClone("AP");
   tp1->Draw();
   char n15[1024];
   strcpy(n15,name_32);
   strcat(n15,"_TOF_min1");
   char n15r[1024];
   strcpy(n15r,n15);
   strcat(n15,".png");
   strcat(n15r,".root");
   c_TOF_min1->SaveAs(n15r);
   c_TOF_min1->SaveAs(n15);

   auto c_TOF_min2 = new TCanvas("c_TOF_min2", "TOF_min2", 950, 800);

   auto g_TOF_min2 = df.Graph("Min_v2","tof15_cor");

   g_TOF_min2->SetTitle("Time of Flight vs minimum 2");
   g_TOF_min2->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_min2->GetXaxis()->SetTitle("Minimum 2 [mV]");
   g_TOF_min2->DrawClone("AP");
   tp1->Draw();
   char n16[1024];
   strcpy(n16,name_32);
   strcat(n16,"_TOF_min2");
   char n16r[1024];
   strcpy(n16r,n16);
   strcat(n16,".png");
   strcat(n16r,".root");
   c_TOF_min2->SaveAs(n16r);
   c_TOF_min2->SaveAs(n16);

   auto c_TOF_min3 = new TCanvas("c_TOF_min3", "TOF_min3", 950, 800);

   auto g_TOF_min3 = df.Graph("Min_v3","tof15_cor");

   g_TOF_min3->SetTitle("Time of Flight vs minimum 3");
   g_TOF_min3->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_min3->GetXaxis()->SetTitle("Minimum 3 [mV]");
   g_TOF_min3->DrawClone("AP");
   tp1->Draw();
   char n17[1024];
   strcpy(n17,name_32);
   strcat(n17,"_TOF_min3");
   char n17r[1024];
   strcpy(n17r,n17);
   strcat(n17,".png");
   strcat(n17r,".root");
   c_TOF_min3->SaveAs(n17r);
   c_TOF_min3->SaveAs(n17);

   auto c_TOF_rise1 = new TCanvas("c_TOF_rise1", "TOF_rise1", 950, 800);

   auto g_TOF_rise1 = df.Graph("rise1","tof15_cor");

   g_TOF_rise1->SetTitle("Time of Flight vs rise time 1");
   g_TOF_rise1->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_rise1->GetXaxis()->SetTitle("Rise time 1 [ns]");
   g_TOF_rise1->DrawClone("AP");
   tp1->Draw();
   char n18[1024];
   strcpy(n18,name_32);
   strcat(n18,"_TOF_rise1");
   char n18r[1024];
   strcpy(n18r,n18);
   strcat(n18,".png");
   strcat(n18r,".root");
   c_TOF_rise1->SaveAs(n18r);
   c_TOF_rise1->SaveAs(n18);

   auto c_TOF_rise2 = new TCanvas("c_TOF_rise2", "TOF_rise2", 950, 800);

   auto g_TOF_rise2 = df.Graph("rise2","tof15_cor");

   g_TOF_rise2->SetTitle("Time of Flight vs rise time 2");
   g_TOF_rise2->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_rise2->GetXaxis()->SetTitle("Rise time 2 [ns]");
   g_TOF_rise2->DrawClone("AP");
   tp1->Draw();
   char n19[1024];
   strcpy(n19,name_32);
   strcat(n19,"_TOF_rise2");
   char n19r[1024];
   strcpy(n19r,n19);
   strcat(n19,".png");
   strcat(n19r,".root");
   c_TOF_rise2->SaveAs(n19r);
   c_TOF_rise2->SaveAs(n19);

   auto c_TOF_rise3 = new TCanvas("c_TOF_rise3", "TOF_rise3", 950, 800);

   auto g_TOF_rise3 = df.Graph("rise3","tof15_cor");

   g_TOF_rise3->SetTitle("Time of Flight vs rise time 3");
   g_TOF_rise3->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_rise3->GetXaxis()->SetTitle("Rise time 3 [ns]");
   g_TOF_rise3->DrawClone("AP");
   tp1->Draw();
   char n20[1024];
   strcpy(n20,name_32);
   strcat(n20,"_TOF_rise3");
   char n20r[1024];
   strcpy(n20r,n20);
   strcat(n20,".png");
   strcat(n20r,".root");
   c_TOF_rise3->SaveAs(n20r);
   c_TOF_rise3->SaveAs(n20);



   //auto d1 = df.Display({"x_15","x_50"},700);
   //d1->Print();
}

/*

         double w1_arr[1024];
         double w2_arr[1024];
         double t1_arr[1024];
         double t2_arr[1024];
         for (i = 0; i < 1024; i++)
         {
            w1_arr[i] = waveform[b][1][i];
            w2_arr[i] = waveform[b][2][i];
            t1_arr[i] = time[b][1][i];
            t2_arr[i] = time[b][2][i];
         }

         std::vector<double> w1_vec(std::begin(w1_arr), std::end(w1_arr));

         std::vector<double> w2_vec(std::begin(w2_arr), std::end(w2_arr));

         std::vector<double> t1_vec(std::begin(t1_arr), std::end(t1_arr));

         std::vector<double> t2_vec(std::begin(t2_arr), std::end(t2_arr));

         int fifty_1 = get50val(w1_vec, t1_vec);
         int fifty_2 = get50val(w2_vec, t2_vec);
         if (fifty_1 != 1 || fifty_2 != 1)
         {
            double diff = fifty_2 - fifty_1;
            timediff.push_back(diff);
            hist->Fill(diff);
         }
                  TCanvas *c1 = new TCanvas();

         TMultiGraph *mg = new TMultiGraph();
         TGraph *g1 = new TGraph(1024, time[b][1], waveform[b][1]);
         TGraph *g2 = new TGraph(1024, time[b][2], waveform[b][2]);
         g1->SetTitle("channel 1 (JIT)");
         g2->SetTitle("channel 2 (retarded)");
         g1->SetLineColor(kGreen);
         g2->SetLineColor(kBlue);
         mg->Add(g1,"lp");
         mg->Add(g2,"cp");
         mg->Draw("a");
         c1->BuildLegend();
         c1->Update();
         gPad->WaitPrimitive();

         */