{
TCanvas * c1 = new TCanvas();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H4;
dir->GetObject("bigWORLD", H4);
H4->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H4->GetYaxis()->SetTitle("Count");
H4->Draw("HIST");
}
