#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"


void DrawMulti(){
   TFile f("Event15.root","update");
 
   auto tree= f.Get<TTree>("Event0");
   Int_t N = tree->GetEntries();

   Float_t t1[1024], t2[1024], t3[1024];
   Float_t v1[1024], v2[1024], v3[1024];

   tree->SetBranchAddress("t1", &t1);
   tree->SetBranchAddress("t2", &t2);
   tree->SetBranchAddress("t3", &t3);
   tree->SetBranchAddress("v1", &v1);
   tree->SetBranchAddress("v2", &v2);
   tree->SetBranchAddress("v3", &v3);


   for(int i=0;i<N;i++){
         tree->GetEntry(i);
         TCanvas *c1 = new TCanvas();

         TMultiGraph *mg = new TMultiGraph();
         TGraph *g1 = new TGraph(1024, t1, v1);
         TGraph *g2 = new TGraph(1024, t2, v2);
         TGraph *g3 = new TGraph(1024, t3, v3);
         g1->SetTitle("channel 1");
         g2->SetTitle("channel 2");
         g3->SetTitle("channel 3");
         g1->SetLineColor(kGreen);
         g2->SetLineColor(kBlue);
         g3->SetLineColor(kRed);
         mg->Add(g1,"lp");
         mg->Add(g2,"cp");
         mg->Add(g3,"cp");
         mg->Draw("a");
         c1->BuildLegend();
         c1->Update();
         gPad->WaitPrimitive();
   }

}