{
TCanvas * c1 = new TCanvas();
c1->SetLogy();
c1->SetTitle("Superimposed Histograms");
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
gStyle->SetOptStat(0);

TH1D* H1;
dir->GetObject("bigGE", H1);
H1->SetLineColor(kRed);
H1->SetTitle("Various Edep in Ge");
H1->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H1->GetYaxis()->SetTitle("Count");
H1->SetMaximum(1000);
H1->Draw("HIST");

TH1D* H2;
dir->GetObject("bigG-I", H2);
H2->SetLineColor(kGreen);
H2->Draw("HIST SAME");

TH1D* H3;
dir->GetObject("bigG-O", H3);
H3->SetLineColor(kYellow);
H3->Draw("HIST SAME");

TH1D* H4;
dir->GetObject("bigG-IO", H4);
H4->SetLineColor(kBlue);
H4->Draw("HIST SAME");

leg = new TLegend(0.7,0.7,0.9,0.9);
   leg->SetHeader("Legend");
//   leg->AddEntry(H5,"World","l");
//   leg->AddEntry(H6,"OSC","l");
//   leg->AddEntry(H7, "ISC", "l");
   leg->AddEntry(H1,"Edep in Ge","l");
   leg->AddEntry(H2,"Edep in Ge -ISC","l");
   leg->AddEntry(H3,"Edep in Ge -OSC","l");
   leg->AddEntry(H4,"Edep in Ge -ISC or -OSC","l");
   leg->Draw();

return c1;
}
