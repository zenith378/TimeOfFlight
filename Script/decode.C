/*

   Name:           read_binary.C
   Created by:     Stefan Ritt <stefan.ritt@psi.ch>
   Date:           July 30th, 2014

   Purpose:        Example program under ROOT to read a binary data file written
                   by the DRSOsc program. Decode time and voltages from waveforms
                   and display them as a graph. Put values into a ROOT Tree for
                   further analysis.

                   To run it, do:

                   - Crate a file test.dat via the "Save" button in DRSOsc
                   - start ROOT
                   root [0] .L read_binary.C+
                   root [1] decode("test.dat");

*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "Getline.h"
#include <algorithm>
#include "TH1.h"

typedef struct
{
   char tag[3];
   char version;
} FHEADER;

typedef struct
{
   char time_header[4];
} THEADER;

typedef struct
{
   char bn[2];
   unsigned short board_serial_number;
} BHEADER;

typedef struct
{
   char event_header[4];
   unsigned int event_serial_number;
   unsigned short year;
   unsigned short month;
   unsigned short day;
   unsigned short hour;
   unsigned short minute;
   unsigned short second;
   unsigned short millisecond;
   unsigned short range;
} EHEADER;

typedef struct
{
   char tc[2];
   unsigned short trigger_cell;
} TCHEADER;

typedef struct
{
   char c[1];
   char cn[3];
} CHEADER;


/*-----------------------------------------------------------------------------*/

void decode()
{
   FHEADER fh;
   THEADER th;
   BHEADER bh;
   EHEADER eh;
   TCHEADER tch;
   CHEADER ch;
   const char *path_name = "../Dati/Binary/";
   const char *path_root = "../Dati/Root/";
   const char *filename= "run0.dat";
   char run_string[256];
   strcpy(run_string,path_name);
   strcat(run_string,filename); 
   unsigned int scaler;
   unsigned short voltage[1024];
   double waveform[16][4][1024], time[16][4][1024];
   float bin_width[16][4][1024];
   char rootfile[256];
   char rootfile_tmp[256];
   int i, j, b, chn, n, chn_index, n_boards;
   double t1, t2, dt;
   std::vector<double> timediff;
   TH1D *hist = new TH1D("hist", "difference form c1 and c2", 100, 0., 0.);

   // open the binary waveform file
   FILE *f = fopen(Form("%s", run_string), "r");
   if (f == NULL)
   {
      printf("Cannot find file \'%s\'\n", run_string);
      return;
   }

   // open the root file
   strcpy(rootfile_tmp, filename);
   for (i = 0; i < strlen(rootfile_tmp); i++)
      if (rootfile_tmp[i] == '.')
      {
         rootfile_tmp[i] = 0;
         break;
      }
   strcat(rootfile,path_root);
   strcat(rootfile,rootfile_tmp);
   strcat(rootfile, ".root");
   TFile *outfile = new TFile(rootfile, "RECREATE");


   // read time header
   fread(&th, sizeof(th), 1, f);
   if (memcmp(th.time_header, "TIME", 4) != 0)
   {
      printf("Invalid time header in file \'%s\', aborting.\n", filename);
      return;
   }

   for (b = 0;; b++)
   {
      // read board header
      fread(&bh, sizeof(bh), 1, f);
      if (memcmp(bh.bn, "B#", 2) != 0)
      {
         // probably event header found
         fseek(f, -4, SEEK_CUR);
         break;
      }

      printf("Found data for board #%d\n", bh.board_serial_number);

      // read time bin widths
      memset(bin_width[b], 0, sizeof(bin_width[0]));
      for (chn = 0; chn < 5; chn++)
      {
         fread(&ch, sizeof(ch), 1, f);
         if (ch.c[0] != 'C')
         {
            // event header found
            fseek(f, -4, SEEK_CUR);
            break;
         }
         i = ch.cn[2] - '0' - 1;
         printf("Found timing calibration for channel #%d\n", i + 1);
         fread(&bin_width[b][i][0], sizeof(float), 1024, f);
         // fix for 2048 bin mode: double channel
         if (bin_width[b][i][1023] > 10 || bin_width[b][i][1023] < 0.01)
         {
            for (j = 0; j < 512; j++)
               bin_width[b][i][j + 512] = bin_width[b][i][j];
         }
      }
   }
   n_boards = b;
   // loop over all events in the data file
   for (n = 0;; n++)
   {
      // read event header
      i = (int)fread(&eh, sizeof(eh), 1, f);
      if (i < 1)
         break;

      printf("Found event #%d %d %d\n", eh.event_serial_number, eh.second, eh.millisecond);
      // define the rec tree
      char tree_name[15] = "Event_";
      char evt_num[10];
      sprintf(evt_num, "%d", eh.event_serial_number);
      strcat(tree_name, evt_num);

      // printf("Initialize tree #%d, with name %s, event number: %d\n", n, tree_name, eh.event_serial_number);

      TTree *rec = new TTree(tree_name, tree_name);

      rec->Branch("t0", time[0][0], "t0[1024]/D");
      rec->Branch("t1", time[0][1], "t1[1024]/D");
      rec->Branch("t2", time[0][2], "t2[1024]/D");
      rec->Branch("t3", time[0][3], "t3[1024]/D");

      rec->Branch("w0", waveform[0][0], "w0[1024]/D");
      rec->Branch("w1", waveform[0][1], "w1[1024]/D");
      rec->Branch("w2", waveform[0][2], "w2[1024]/D");
      rec->Branch("w3", waveform[0][3], "w3[1024]/D");

      // loop over all boards in data file
      for (b = 0; b < n_boards; b++)
      {

         // read board header
         fread(&bh, sizeof(bh), 1, f);
         if (memcmp(bh.bn, "B#", 2) != 0)
         {
            printf("Invalid board header in file \'%s\', aborting.\n", filename);
            return;
         }

         // read trigger cell
         fread(&tch, sizeof(tch), 1, f);
         if (memcmp(tch.tc, "T#", 2) != 0)
         {
            printf("Invalid trigger cell header in file \'%s\', aborting.\n", filename);
            return;
         }

         if (n_boards > 1)
            printf("Found data for board #%d\n", bh.board_serial_number);

         // reach channel data
         for (chn = 0; chn < 4; chn++)
         {

            // read channel header
            fread(&ch, sizeof(ch), 1, f);
            if (ch.c[0] != 'C')
            {
               // event header found
               fseek(f, -4, SEEK_CUR);
               break;
            }
            chn_index = ch.cn[2] - '0' - 1;
            fread(voltage, sizeof(short), 1024, f);

            for (i = 0; i < 1024; i++)
            {
               // convert data to volts
               waveform[b][chn_index][i] = (voltage[i] / 65536. + eh.range / 1000.0 - 0.5);

               // calculate time for this cell
               for (j = 0, time[b][chn_index][i] = 0; j < i; j++)
               {
                  // printf("time pre conversion: %f\n",time[b][chn_index][i]);
                  time[b][chn_index][i] += bin_width[b][chn_index][(j + tch.trigger_cell) % 1024];
                  // printf("time after conversion: %f\n",time[b][chn_index][i]);
               }
            }
         }

         // align cell #0 of all channels
         t1 = time[b][0][(1024 - tch.trigger_cell) % 1024];
         for (chn = 1; chn < 4; chn++)
         {
            t2 = time[b][chn][(1024 - tch.trigger_cell) % 1024];
            dt = t1 - t2;
            for (i = 0; i < 1024; i++)
               time[b][chn][i] += dt;
         }
        /*
         TCanvas *c1 = new TCanvas();


         TMultiGraph *mg = new TMultiGraph();
         TGraph *g1 = new TGraph(1024, time[b][0], waveform[b][0]);
         TGraph *g2 = new TGraph(1024, time[b][1], waveform[b][1]);
         TGraph *g3 = new TGraph(1024, time[b][2], waveform[b][2]);
         g1->SetTitle("channel 1 ");
         g2->SetTitle("channel 2 ");
         g3->SetTitle("channel 3 ");
         g1->SetLineColor(kGreen);
         g2->SetLineColor(kBlue);
         mg->Add(g1,"lp");
         mg->Add(g2,"cp");
         mg->Add(g3,"lp");
         mg->Draw("a");
         //mg->GetXaxis()->SetLimits(370,400);
         c1->BuildLegend();
         c1->Update();
         gPad->WaitPrimitive();
*/

/*
                  for (chn = 0; chn < 4; chn++)
                  {

                     // create graph
                     TGraph *g = new TGraph(1024, time[b][chn], waveform[b][chn]);
                     // fill graph
                     //for (i = 0; i < 1024; i++)
                     //   g->SetPoint(i, time[b][chn][i], waveform[b][chn][i]);
                     std::cout << "Drawing board" << b << " chn " << chn << " event: " << std::endl;

                     // draw graph and wait for user click
                     g->Draw("ACP");
                     c1->Update();
                     gPad->WaitPrimitive();

                  }
                  */
                  
      }
      // fill root tree
      rec->Fill();
      rec->Write();
   }

   // print number of events
   printf("%d events processed, \"%s\" written.\n", n, rootfile);

   // save and close root file
   outfile->Close();

}
