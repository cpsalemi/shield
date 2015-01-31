{
TCanvas * c1 = new TCanvas();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H3;
dir->GetObject("ISC", H3);
H3->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H3->GetYaxis()->SetTitle("Count");
H3->Draw("HIST");
}
