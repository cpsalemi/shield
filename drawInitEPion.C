{
TCanvas * c1 = new TCanvas();
c1->Update();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H1;
dir->GetObject("initEPion", H1);
H1->GetXaxis()->SetTitle("Energy (MeV)");
H1->GetYaxis()->SetTitle("Count");
H1->Draw("HIST");
}
