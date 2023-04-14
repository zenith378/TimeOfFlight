
#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include <iostream>
#include "TApplication.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TKey.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TPaveText.h"

using namespace std;
using namespace ROOT::VecOps;

double get50val(const ROOT::RVec<double>& v1, const ROOT::RVec<double>& t1)
{
   auto v_min = Min(v1);
   auto v_max = 0;
   for(int i=0;i<50;i++){
      v_max+=v1[i];
   }
   v_max=v_max/50;

   double fifty = (v_min-v_max) * 0.15;
   int index_fifty = -1;
   for (int i = 0; i < v1.size(); i++)
   {
      if (v1[i] < fifty)
      {
         index_fifty = i-1;
         break;
      }
   }
   if (index_fifty == -1)
      return 999;
   return t1[index_fifty];
}

double get50int(const ROOT::RVec<double>& v1, const ROOT::RVec<double>& t1)
{
   auto v_min = Min(v1);
   auto v_max = (v1[0] + v1[1])/2;
   double ten = (v_min - v_max) * 0.4;
   double ninghty = (v_min-v_max) * 0.6;

   int index_ten = -1;
   int index_ninghty = -1;

   if (v_min > 0.)
      return 99;
   for (int i = 0; i < v1.size(); i++)
   {

      if (v1[i] < ten)
      {
         index_ten = i;

         break;
      }
   }

   for (int i = 0; i < v1.size(); i++)
   {
      if (v1[i] < ninghty)
      {
         index_ninghty = i;
         break;
      }
   }
   if (index_ten == -1 || index_ninghty == -1)
   {
      std::cout << "something's gone wrong" << std::endl;
      exit(1);
   }


   double weight_v;
   double mean;

   for (int i = index_ten; i < index_ninghty; i++)
   {
      weight_v += v1[i];
      mean += (v1[i] * t1[i]);
   }

   double average = mean / weight_v;

   return average;
}



void main0()
{
   ROOT::EnableImplicitMT();
   // Open root file
   const char *path_root = "../Dati/Root/";
   const char *filename = "run0";
   const char *extension = ".root";
   char rootfile[256];
   strcpy(rootfile,path_root);
   strcat(rootfile,filename);
   strcat(rootfile,extension);
   TFile *f = new TFile(rootfile);
   if (f == NULL)
   {
      std::cout <<"Cannot find file " << rootfile << std::endl;
      return;
   }
   //store all tree in file inside a TChain
   TChain c;
   for (auto &&keyAsObj : *f->GetListOfKeys())
   {
      auto key = (TKey *)keyAsObj;
      TString namechain;
      namechain.Form("%s/%s",rootfile,key->GetName());
      c.Add(namechain);
     // if(!strcmp(key->GetName(),"Event_500")) break;
   }
   //store the tchain inside Dataframe

   auto vGen = [&]() {
      RVec<int> pedestal_ind_v={};
   for(int i=50;i<1024;i++) pedestal_ind_v.push_back(i);
      return pedestal_ind_v;
   };

   ROOT::RDataFrame df(c);
   auto df_set = df.Filter("Min(w0)>=-0.43","not saturated wave 1")
                   .Filter("Min(w1)>=-0.43","not saturated wave 2")
                   .Filter("Min(w2)>=-0.43","not saturated wave 3")
                   .Filter("Min(w0)<-0.","event in 1")
                   .Filter("Min(w1)<-0.","event in 2")
                   .Filter("Min(w2)<-0.","event in 3")
                   .Define("pedestal_ind",vGen)
                   .Define("pedestal_1","Drop(w0,pedestal_ind)")
                   .Define("v_max_1","Mean(pedestal_1)")
                   .Define("v1_50","(Min(w0)-v_max_1)*0.15")
                   .Define("v1_cut","w0<v1_50")
                   .Define("val50_1","t0[v1_cut]")
                   .Define("val50_t1","val50_1[0]")
                   .Define("pedestal_2","Drop(w1,pedestal_ind)")
                   .Define("v_max_2","Mean(pedestal_2)")
                   .Define("v2_50","(Min(w1)-v_max_2)*0.15")
                   .Define("v2_cut","w1<v2_50")
                   .Define("val50_2","t1[v2_cut]")
                   .Define("val50_t2","val50_2[0]")
                   .Define("pedestal_3","Drop(w2,pedestal_ind)")
                   .Define("v_max_3","Mean(pedestal_3)")
                   .Define("v3_50","(Min(w2)-v_max_3)*0.15")
                   .Define("v3_cut","w2<v3_50")
                   .Define("val50_3","t2[v3_cut]")
                   .Define("val50_t3","val50_3[0]")
                   //.Define("val50_t1",get50val,{"w0","t0"})
                   //.Define("val50_t2",get50val,{"w1","t1"})
                   //.Define("val50_t3",get50val,{"w2","t2"})
                   //.Filter("abs(val50_t1)<205","t1 read correctly")
                   //.Filter("abs(val50_t2)<205","t2 read correctly")
                   //.Filter("abs(val50_t3)<205","t3 read correctly")
                   //.Define("int50_t1",get50int,{"w0","t0"})
                   //.Define("int50_t2",get50int,{"w1","t1"})
                   //.Define("int_diff","int50_t2-int50_t1")
                   .Define("t2_t1","val50_t2-val50_t1")
                   .Define("t3_t1","val50_t3-val50_t1")
                   .Define("t3_t2","val50_t3-val50_t2")
                   .Filter("abs(t2_t1)<50","t1 read correctly")
                   .Filter("abs(t3_t1)<50","t2 read correctly")
                   .Filter("abs(t3_t2)<50","t3 read correctly")
                   .Define("TOF","val50_t3-(val50_t1+val50_t2)/2+140*0.06414+(6.538+7.356)/2");
                   //.Filter("abs(int_diff)<30 || abs(val_diff)<30");

   df_set.Snapshot("Events","../Dati/Root/Run0long_df.root");

   TString authors="G. Cordova, A. Giani";
   char position[1024] = "PMT 3 at ";
   strcat(position,filename);
   strcat(position," cm");
/*
   auto c_int = new TCanvas("c_int", "Integrated diff", 950, 800);

   auto h_int = df_set.Histo1D({"h", "Integrated diff", 100,-50, 50}, "int_diff");
   h_int->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_int->SetTitle("Time difference between PMT 2 and PMT1");
   h_int->GetYaxis()->SetTitle("Counts");
   h_int->GetXaxis()->SetTitle("Time difference [ns]");
   
   auto tp = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp->AddText("ToF");
   tp->AddText(authors);
   tp->AddText("calibration 28/03/23 10min");
   tp->AddText(position);
   tp->AddText("Mean Method");


   h_int->DrawClone();
   tp->Draw();

   char cname_mean[256]="../Plots/Calibration/";
   strcat(cname_mean,filename); 
   strcat(cname_mean,"mean.pdf");
   //c_int->SaveAs(cname_mean);

   
   auto c_val = new TCanvas("c_val", "Exact diff", 950, 800);


   auto h_val = df_set.Histo1D({"h", "Exact diff",100,0,0}, "t2_t1");
   h_val->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_val->SetTitle("Time difference between PMT 2 and PMT1");
   h_val->GetYaxis()->SetTitle("Counts");
   h_val->GetXaxis()->SetTitle("Time difference [ns]");
   char position1[1024] = "PMT 3 at ";
   strcat(position1,filename);

   strcat(position1," cm");
   auto tp1 = new TPaveText(0.65, 0.4, 0.85, 0.6, "NDC");
   tp1->AddText("ToF");
   tp1->AddText(authors);
   tp1->AddText("calibration 28/03/23 10min");
   tp1->AddText(position);
   tp1->AddText("Exact Method");
   h_val->DrawClone();
   //tp1->Draw();

   char name_21[256]="../Plots/Calibration/";
   strcat(name_21,filename); 
   strcat(name_21,"_21.pdf");
   c_val->SaveAs(name_21);
*/

/*


   auto c_t3_t1 = new TCanvas("c_t3_t1", "Exact diff", 950, 800);


   auto h_31 = df_set.Histo1D({"h31", "Exact diff", 100,0,0}, "t3_t1");
   h_31->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_31->SetTitle("Time difference between PMT 3 and PMT1");
   h_31->GetYaxis()->SetTitle("Counts");
   h_31->GetXaxis()->SetTitle("Time difference [ns]");

   h_31->DrawClone();
   tp1->Draw();

   char name_31[256]="../Plots/Calibration/";
   strcat(name_31,filename); 
   strcat(name_31,"_31.pdf");
   c_t3_t1->SaveAs(name_31);


   auto c_t2_t1 = new TCanvas("c_t2_t1", "Exact diff", 950, 800);


   auto h_32 = df_set.Histo1D({"h32", "Exact diff", 100,0,0}, "t3_t2");
   h_32->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
   h_32->SetTitle("Time difference between PMT 3 and PMT2");
   h_32->GetYaxis()->SetTitle("Counts");
   h_32->GetXaxis()->SetTitle("Time difference [ns]");

   h_32->DrawClone();
   tp1->Draw();
   char name_32[256]="../Plots/Calibration/";
   strcat(name_32,filename); 
   strcat(name_32,"_32.pdf");
   c_t2_t1->SaveAs(name_32);
*/
    auto c_TOF = new TCanvas("c_TOF", "TOF", 950, 800);


   auto h_TOF = df_set.Histo1D({"h_TOF", "TOF",150,-5,25}, "TOF");
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