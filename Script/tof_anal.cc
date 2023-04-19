
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
   const char *filename = "120";
   const char *extension = ".root";
   char rootfile[256];
   strcpy(rootfile,path_root);
   strcat(rootfile,filename);
   strcat(rootfile,extension);


   ROOT::RDataFrame d("Events",rootfile);
   auto df = d.Filter("Min(v1)>-0.49","not saturated 1")
   .Filter("Min(v2)>-0.49","not saturated 2")
   .Filter("Min(v3)>-0.49","not saturated 3")
   .Filter("Min(v1)<0","minimum under 0 for 1")
   .Filter("Min(v2)<0","minimum under 0 for 2")
   .Filter("Min(v3)<0","minimum under 0 for 3")
   .Filter("Max(v1)<0.1","maximum under 0.1 for 1")
   .Filter("Max(v2)<0.1","maximum under 0.1 for 2")
   .Filter("Max(v3)<0.1","maximum under 0.1 for 3")
   .Define("Min_v1","Min(v1)")
   .Define("Min_v2","Min(v2)")
   .Define("v1_CF","Max(v1)+(Min(v1)-Max(v1))*0.25")
   .Define("v1_cut","v1<v1_CF")
   .Define("t1_cut","t1[v1_cut]")
   .Define("t1_CF","t1_cut[0]")
   .Define("v2_CF","Max(v2)+(Min(v2)-Max(v2))*0.25")
   .Define("v2_cut","v2<v2_CF")
   .Define("t2_cut","t2[v2_cut]")
   .Define("t2_CF","t2_cut[0]")
   .Define("t21","t2_CF-t1_CF")
   .Define("v3_CF","Max(v3)+(Min(v3)-Max(v3))*0.25")
   .Define("v3_cut","v3<v3_CF")
   .Define("t3_cut","t3[v3_cut]")
   .Define("t3_CF","t3_cut[0]")
   .Define("t31","t3_CF-t1_CF")
   .Define("t32","t3_CF-t2_CF")
   .Define("Min_v3","Min(v3)")
   //.Filter("Min_v3<-0.1","Min_v3<-0.1")
   //.Define("x_15_cor","(7.356-6.538-tCF15_2+tCF15_1)/(2*0.06414)")
   .Define("tof_un","t3_CF-(t1_CF+t2_CF)/2")
   .Define("my_tof_cor","tof_un+16.5") // 140*0.06414+(6.538 + 7.356)/2
   //.Define("x_new","(7.356-6.538-tCF15_2+tCF15_1)/(2*0.06414)")
   .Filter("t1_CF>5","t1_CF>5")
   .Filter("t2_CF>5","t2_CF>5")
   .Filter("t3_CF>5","t3_CF>5")
   .Filter("t1_CF<250","t1_CF<250")
   .Filter("t2_CF<250","t2_CF<250")
   .Filter("t3_CF<250","t3_CF<250")
   //.Filter("abs(x_new)<160","x_new<160")
   .Define("Min_21","Min_v2-Min_v1")
   .Define("Min_2su1","Min_v2/Min_v1")
   //.Filter("Min_v2>-0.3","Min_2>-0.3")
   //.Filter("myt21>-4+17*Min_21/0.4","reconstructed x ok");
   .Filter("Min_2su1<1.1","Min_2su1<1.1")
   .Filter("Min_2su1>0.6","Min_2su1>0.6");
   auto cutrep=d.Report();
   cutrep->Print();

   df.Snapshot("Event0","Event15.root");
   TString authors="G. Cordova, A. Giani";
   char position[1024] = "PMT 3 at ";
   strcat(position,filename);
   strcat(position," cm");
   char fig_path[256]="../Plots/Calibration/";

   auto tp1 = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp1->AddText("ToF");
   tp1->AddText(authors);
   tp1->AddText(position);
   tp1->AddText("Calibration 19/4/23");

/*
   auto c_TOF_50 = new TCanvas("c_TOF_50", "TOF_50", 950, 800);

   auto h_TOF_50 = df.Histo1D({"h_TOF_50", "TOF_50",125,-5,15}, "my_tof_cor");

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


  auto c_TOF_1_50 = new TCanvas("c_TOF_50_50", "TOF_50_50", 950, 800);

  auto g_TOF_1_50 = df.Graph("tCF15_1","my_tof_cor");
   g_TOF_1_50->SetTitle("Time of Flight vs time1_50");
   g_TOF_1_50->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_1_50->GetXaxis()->SetTitle("time1_15 [ns]");
   g_TOF_1_50->DrawClone("AP");
   tp1->Draw();
   char n2[1024];
   strcpy(n2,name_32);
   strcat(n2,"_tof_time1");
   char n2r[1024];
   strcpy(n2r,n2);
   strcat(n2,".png");
   strcat(n2r,".root");
   //c_TOF_1_50->SaveAs(n2);
   //c_TOF_1_50->SaveAs(n2r);

   auto c_TOF_2_50 = new TCanvas("c_TOF_2_50", "TOF_2_50", 950, 800);

   auto g_TOF_2_50 = df.Graph("tCF15_2","my_tof_cor");
   g_TOF_2_50->SetTitle("Time of Flight vs time2_15");
   g_TOF_2_50->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_2_50->GetXaxis()->SetTitle("time2_15 [ns]");
   g_TOF_2_50->DrawClone("AP");
   tp1->Draw();
   char n3[1024];
   strcpy(n3,name_32);
   strcat(n3,"_tof_time2");
   char n3r[1024];
   strcpy(n3r,n3);
   strcat(n3,".png");
   strcat(n3r,".root");
   //c_TOF_2_50->SaveAs(n3r);
   //c_TOF_2_50->SaveAs(n3);


   auto c_TOF_3_50 = new TCanvas("c_TOF_3_50", "TOF_3_50", 950, 800);

   auto g_TOF_3_50 = df.Graph("tCF15_3","my_tof_cor");
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
   //c_TOF_3_50->SaveAs(n4r);
   //c_TOF_3_50->SaveAs(n4);



   auto c_TOF_x = new TCanvas("c_TOF_x", "TOF_x", 950, 800);


   auto g_TOF_x = df.Graph("x_new","my_tof_cor");
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
   //c_TOF_x->SaveAs(n5r);
   //c_TOF_x->SaveAs(n5);
   

   auto c_TOF_y = new TCanvas("c_TOF_y", "TOF_y", 950, 800);

   auto g_TOF_50_x = df.Graph("x_50","tof50");
   g_TOF_50_x->SetTitle("ToF vs x");
   g_TOF_50_x->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_50_x->GetXaxis()->SetTitle("x_{50} [cm]");
   g_TOF_50_x->DrawClone("AP");

   auto c_x_y = new TCanvas("c_x_y", "x_y", 950, 800);

   auto h_x = df.Histo1D({"h_x", "x",50,0,0}, "x_new");

   h_x->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_x->SetTitle("x");
   h_x->GetYaxis()->SetTitle("Counts");
   h_x->GetXaxis()->SetTitle("x [cm]");
   h_x->DrawClone();
   tp1->Draw();
   char n6[1024];
   strcpy(n6,name_32);
   strcat(n6,"/x/");
   strcat(n6,filename); 
   char n6r[1024];
   strcpy(n6r,n6);
   strcat(n6,".png");
   strcat(n6r,".root");
   //c_x_y->SaveAs(n6r);
   //c_x_y->SaveAs(n6);

*/
   auto c_21 = new TCanvas("c_21", "21", 950, 800);

   auto h_t21 = df.Histo1D({"h_t21", "t21",150,0,0}, "t21");

   h_t21->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t21->SetTitle("time difference 2-1");
   h_t21->GetYaxis()->SetTitle("Counts");
   h_t21->GetXaxis()->SetTitle("Time difference 2-1[ns]");
   h_t21->DrawClone();
   tp1->Draw();
   char n8[1024];
   strcpy(n8,fig_path);
   strcat(n8,"/t21/");
   strcat(n8,filename);
   char n8r[1024];
   strcpy(n8r,n8);
   strcat(n8,".png");
   strcat(n8r,".root");
   c_21->SaveAs(n8r);
   c_21->SaveAs(n8);

   auto c_31 = new TCanvas("c_31", "31", 950, 800);  

   auto h_t31 = df.Histo1D({"h_t31", "t31",150,0,0}, "t31");

   h_t31->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t31->SetTitle("time difference 3-1");
   h_t31->GetYaxis()->SetTitle("Counts");
   h_t31->GetXaxis()->SetTitle("Time difference 3-1[ns]");
   h_t31->DrawClone();
   tp1->Draw();
   char n9[1024];
   strcpy(n9,fig_path);
   strcat(n9,"/t31/");
   strcat(n9,filename);
   char n9r[1024];
   strcpy(n9r,n9);
   strcat(n9,".png");
   strcat(n9r,".root");
   c_31->SaveAs(n9r);
   c_31->SaveAs(n9);

   auto c_32 = new TCanvas("c_32", "32", 950, 800);

   auto h_t32 = df.Histo1D({"h_t32", "t32",150,0,0}, "t32");

   h_t32->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_t32->SetTitle("time difference 3-2");
   h_t32->GetYaxis()->SetTitle("Counts");
   h_t32->GetXaxis()->SetTitle("Time difference 3-2[ns]");
   h_t32->DrawClone();
   tp1->Draw();
   char n10[1024];
   strcpy(n10,fig_path);
   strcat(n10,"/t32/");
   strcat(n10,filename);
   char n10r[1024];
   strcpy(n10r,n10);
   strcat(n10,".png");
   strcat(n10r,".root");
   c_32->SaveAs(n10r);
   c_32->SaveAs(n10);

   auto c_tof_un = new TCanvas("c_tof_un", "tof_un", 950, 800);

   auto h_tof_un = df.Histo1D({"h_tof_un", "tof_un",125,0,0}, "tof_un");

   h_tof_un->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_tof_un->SetTitle("Time of Flight uncorrected");
   h_tof_un->GetYaxis()->SetTitle("Counts");
   h_tof_un->GetXaxis()->SetTitle("ToF [ns]");
   h_tof_un->DrawClone();
   tp1->Draw();
   char n11[1024];
   strcpy(n11,fig_path);
   strcat(n11,"/tof_un/");
   strcat(n11,filename);
   char n11r[1024];
   strcpy(n11r,n11);
   strcat(n11,".png");
   strcat(n11r,".root");
   c_tof_un->SaveAs(n11r);
   c_tof_un->SaveAs(n11);
/*
   auto c_TOF_t21 = new TCanvas("c_TOF_t21", "TOF_t21", 950, 800);

   auto g_TOF_t21 = df.Graph("myt21","my_tof_cor");

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
   //c_TOF_t21->SaveAs(n12r);
   //c_TOF_t21->SaveAs(n12);

   auto c_TOF_t31 = new TCanvas("c_TOF_t31", "TOF_t31", 950, 800);

   auto g_TOF_t31 = df.Graph("myt31","my_tof_cor");

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
   //c_TOF_t31->SaveAs(n13r);
   //c_TOF_t31->SaveAs(n13);

   auto c_TOF_t32 = new TCanvas("c_TOF_t32", "TOF_t32", 950, 800);

   auto g_TOF_t32 = df.Graph("myt32","my_tof_cor");

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
   //c_TOF_t32->SaveAs(n14r);
   //c_TOF_t32->SaveAs(n14);

   auto c_TOF_min1 = new TCanvas("c_TOF_min1", "TOF_min1", 950, 800);

   auto g_TOF_min1 = df.Graph("Min_v1","my_tof_cor");

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
   //c_TOF_min1->SaveAs(n15r);
   //c_TOF_min1->SaveAs(n15);

   auto c_TOF_min2 = new TCanvas("c_TOF_min2", "TOF_min2", 950, 800);

   auto g_TOF_min2 = df.Graph("Min_v2","my_tof_cor");

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
   //c_TOF_min2->SaveAs(n16r);
   //c_TOF_min2->SaveAs(n16);

   auto c_TOF_min3 = new TCanvas("c_TOF_min3", "TOF_min3", 950, 800);

   auto g_TOF_min3 = df.Graph("Min_v3","my_tof_cor");

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
   //c_TOF_min3->SaveAs(n17r);
   //c_TOF_min3->SaveAs(n17);
*/
   auto c_t1 = new TCanvas("c_t1", "t1", 950, 800);

   auto h_t1 = df.Histo1D("t1_CF");

   h_t1->SetTitle("time 1");
   h_t1->GetYaxis()->SetTitle("Counts");
   h_t1->GetXaxis()->SetTitle("time 1 [ns]");
   h_t1->DrawClone("HIST");
   tp1->Draw();
   char n19[1024];
   strcpy(n19,fig_path);
   strcat(n19,"/t1/");
   strcat(n19,filename);
   char n19r[1024];
   strcpy(n19r,n19);
   strcat(n19,".png");
   strcat(n19r,".root");
   c_t1->SaveAs(n19r);
   c_t1->SaveAs(n19);

   auto c_t2 = new TCanvas("c_t2", "t2", 950, 800);

   auto h_t2 = df.Histo1D("t2_CF");

   h_t2->SetTitle("time 2");
   h_t2->GetYaxis()->SetTitle("Counts");
   h_t2->GetXaxis()->SetTitle("time 2 [ns]");
   h_t2->DrawClone("HIST");
   tp1->Draw();
   char n20[1024];
   strcpy(n20,fig_path);
   strcat(n20,"/t2/");
   strcat(n20,filename);
   char n20r[1024];
   strcpy(n20r,n20);
   strcat(n20,".png");
   strcat(n20r,".root");
   c_t2->SaveAs(n20r);
   c_t2->SaveAs(n20);

   auto c_t3 = new TCanvas("c_t3", "t3", 950, 800);

   auto h_t3 = df.Histo1D("t3_CF");

   h_t3->SetTitle("time 3");
   h_t3->GetYaxis()->SetTitle("Counts");
   h_t3->GetXaxis()->SetTitle("time 3 [ns]");
   h_t3->DrawClone("HIST");
   tp1->Draw();
   char n21[1024];
   strcpy(n21,fig_path);
   strcat(n21,"/t3/");
   strcat(n21,filename);
   char n21r[1024];
   strcpy(n21r,n21);
   strcat(n21,".png");
   strcat(n21r,".root");
   c_t3->SaveAs(n21r);
   c_t3->SaveAs(n21);


   auto c_t21_Min1 = new TCanvas("c_t21_Min1", "t21_Min1", 950, 800);

   auto g_t21_Min1 = df.Graph("Min_v1","t21");

   g_t21_Min1->SetTitle("time 21 vs minimum 1");
   g_t21_Min1->SetMarkerStyle(20);
   g_t21_Min1->GetYaxis()->SetTitle("time 21 [ns]");
   g_t21_Min1->GetXaxis()->SetTitle("Minimum 1 [mV]");
   g_t21_Min1->DrawClone("AP");
   tp1->Draw();


   auto c_t21_Min2 = new TCanvas("c_t21_Min2", "t21_Min2", 950, 800);

   auto g_t21_Min2 = df.Graph("Min_v2","t21");

   g_t21_Min2->SetTitle("time 21 vs minimum 2");
   //g_t21_Min2->SetMarkerStyle(20);
   g_t21_Min2->GetYaxis()->SetTitle("time 21 [ns]");
   g_t21_Min2->GetXaxis()->SetTitle("Minimum 2 [mV]");
   g_t21_Min2->DrawClone("AP");
   tp1->Draw();

   auto c_t21_Min3 = new TCanvas("c_t21_Min3", "t21_Min3", 950, 800);

   auto g_t21_Min3 = df.Graph("Min_v3","t21");

   g_t21_Min3->SetTitle("time 21 vs minimum 3");
   //g_t21_Min3->SetMarkerStyle(20);
   g_t21_Min3->GetYaxis()->SetTitle("time 21 [ns]");
   g_t21_Min3->GetXaxis()->SetTitle("Minimum 3 [mV]");
   g_t21_Min3->DrawClone("AP");
   tp1->Draw();

   auto c_Min1_Min2 = new TCanvas("c_Min1_Min2", "Min1_Min2", 950, 800);

   auto g_Min1_Min2 = df.Graph("Min_v2","Min_v1");

   g_Min1_Min2->SetTitle("minimum 1 vs minimum 2");
   //g_Min1_Min2->SetMarkerStyle(20);
   g_Min1_Min2->GetYaxis()->SetTitle("Minimum 1 [V]");
   g_Min1_Min2->GetXaxis()->SetTitle("Minimum 2 [V]");
   g_Min1_Min2->DrawClone("AP");
   tp1->Draw();

   auto c_t21_min21 = new TCanvas("c_t21_min21", "t21_min21", 950, 800);

   auto g_t21_min21 = df.Graph("Min_21","t21");

   g_t21_min21->SetTitle("time 2-1 vs minimum 2-1");
   //g_t21_min21->SetMarkerStyle(20);
   g_t21_min21->GetYaxis()->SetTitle("time 2-1 [ns]");
   g_t21_min21->GetXaxis()->SetTitle("Minimum 2-1 [V]");
   g_t21_min21->DrawClone("AP");
   tp1->Draw();

   auto c_t21_min2su1 = new TCanvas("c_t21_min2su1", "t21_min2su1", 950, 800);

   auto g_t21_min2su1 = df.Graph("Min_2su1","t21");

   g_t21_min2su1->SetTitle("time 2-1 vs minimum 2/ minimum1");
   //g_t21_min2su1->SetMarkerStyle(20);
   g_t21_min2su1->GetXaxis()->SetTitle("Minimum 2/1");
   g_t21_min2su1->GetYaxis()->SetTitle("time 2-1 [ns]");
   g_t21_min2su1->DrawClone("AP");
   tp1->Draw();

   auto c_Min21 = new TCanvas("c_Min21", "Min21", 950, 800);

   auto h_Min21 = df.Histo1D("Min_21");

   h_Min21->SetTitle("minimum 2-1");
   h_Min21->GetYaxis()->SetTitle("Counts");
   h_Min21->GetXaxis()->SetTitle("Minimum 2-1 [V]");
   h_Min21->DrawClone("HIST");
   

   auto c_Min2su1 = new TCanvas("c_Min2su1", "Min2su1", 950, 800);

   auto h_Min2su1 = df.Histo1D("Min_2su1");

   h_Min2su1->SetTitle("minimum 2/1");
   h_Min2su1->GetYaxis()->SetTitle("Counts");
   h_Min2su1->GetXaxis()->SetTitle("Minimum 2/1");
   h_Min2su1->DrawClone("HIST");
   tp1->Draw();
   char n22[1024];
   strcpy(n22,fig_path);
   strcat(n22,"Min_2su1/");
   strcat(n22,filename);
   char n22r[1024];
   strcpy(n22r,n22);
   strcat(n22,".png");
   strcat(n22r,".root");
   c_t3->SaveAs(n22r);
   c_t3->SaveAs(n22);



   auto c_Min2su1_t1 = new TCanvas("c_Min2su1_t1", "Min2su1_t1", 950, 800);

   auto g_Min2su1_t1 = df.Graph("t1_CF","Min_2su1");

   g_Min2su1_t1->SetTitle("minimum 2/1 vs time 1");
   //g_Min2su1_t1->SetMarkerStyle(20);
   g_Min2su1_t1->GetXaxis()->SetTitle("time 1 [ns]");
   g_Min2su1_t1->GetYaxis()->SetTitle("Minimum 2/1");
   g_Min2su1_t1->DrawClone("AP");

   auto c_Min2su1_t2 = new TCanvas("c_Min2su1_t2", "Min2su1_t2", 950, 800);

   auto g_Min2su1_t2 = df.Graph("t2_CF","Min_2su1");

   g_Min2su1_t2->SetTitle("minimum 2/1 vs time 2");
   //g_Min2su1_t2->SetMarkerStyle(20);
   g_Min2su1_t2->GetXaxis()->SetTitle("time 2 [ns]");
   g_Min2su1_t2->GetYaxis()->SetTitle("Minimum 2/1");
   g_Min2su1_t2->DrawClone("AP");

   auto c_Min2su1_t3 = new TCanvas("c_Min2su1_t3", "Min2su1_t3", 950, 800);

   auto g_Min2su1_t3 = df.Graph("t3_CF","Min_2su1");

   g_Min2su1_t3->SetTitle("minimum 2/1 vs time 3");
   //g_Min2su1_t3->SetMarkerStyle(20);
   g_Min2su1_t3->GetXaxis()->SetTitle("time 3 [ns]");
   g_Min2su1_t3->GetYaxis()->SetTitle("Minimum 2/1");
   g_Min2su1_t3->DrawClone("AP");

/*
   auto c_TOF_timeCF1_15 = new TCanvas("c_TOF_timeCF1_15", "TOF_timeCF1_15", 950, 800);

   auto g_TOF_timeCF1_15 = df.Graph("timeCF1_15","my_tof_cor");

   g_TOF_timeCF1_15->SetTitle("Time of Flight vs time 1");
   g_TOF_timeCF1_15->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_timeCF1_15->GetXaxis()->SetTitle("time 1 [ns]");
   g_TOF_timeCF1_15->DrawClone("AP");
   tp1->Draw();
   char n18[1024];
   strcpy(n18,name_32);
   strcat(n18,"_TOF_timeCF1_15");
   char n18r[1024];
   strcpy(n18r,n18);
   strcat(n18,".png");
   strcat(n18r,".root");
   c_TOF_timeCF1_15->SaveAs(n18r);
   c_TOF_timeCF1_15->SaveAs(n18);

   auto c_TOF_timeCF2_15 = new TCanvas("c_TOF_timeCF2_15", "TOF_timeCF2_15", 950, 800);

   auto g_TOF_timeCF2_15 = df.Graph("timeCF2_15","tof15_cor");

   g_TOF_timeCF2_15->SetTitle("Time of Flight vs time 2");
   g_TOF_timeCF2_15->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_timeCF2_15->GetXaxis()->SetTitle("time 2 [ns]");
   g_TOF_timeCF2_15->DrawClone("AP");
   tp1->Draw();
   char n19[1024];
   strcpy(n19,name_32);
   strcat(n19,"_TOF_timeCF2_15");
   char n19r[1024];
   strcpy(n19r,n19);
   strcat(n19,".png");
   strcat(n19r,".root");
   c_TOF_timeCF2_15->SaveAs(n19r);
   c_TOF_timeCF2_15->SaveAs(n19);

   auto c_TOF_timeCF3_15 = new TCanvas("c_TOF_timeCF3_15", "TOF_timeCF3_15", 950, 800);

   auto g_TOF_timeCF3_15 = df.Graph("timeCF3_15","tof15_cor");

   g_TOF_timeCF3_15->SetTitle("Time of Flight vs time 3");
   g_TOF_timeCF3_15->GetYaxis()->SetTitle("ToF [ns]");
   g_TOF_timeCF3_15->GetXaxis()->SetTitle("time 3 [ns]");
   g_TOF_timeCF3_15->DrawClone("AP");
   tp1->Draw();
   char n20[1024];
   strcpy(n20,name_32);
   strcat(n20,"_TOF_timeCF3_15");
   char n20r[1024];
   strcpy(n20r,n20);
   strcat(n20,".png");
   strcat(n20r,".root");
   c_TOF_timeCF3_15->SaveAs(n20r);
   c_TOF_timeCF3_15->SaveAs(n20);

*/

   //auto d1 = df.Display({"x_15","x_50"},700);
   //d1->Print();
   
}
