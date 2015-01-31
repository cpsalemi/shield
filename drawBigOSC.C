{
TCanvas * c1 = new TCanvas();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H2;
dir->GetObject("bigOSC", H2);
H2->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H2->GetYaxis()->SetTitle("Count");
H2->Draw("HIST");
}
