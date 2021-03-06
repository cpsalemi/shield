{
TCanvas * c1 = new TCanvas();
c1->Update();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H1;
dir->GetObject("initDirPositron", H1);
H1->GetXaxis()->SetTitle("Cos(angle below xy plane)");
H1->GetYaxis()->SetTitle("Count");
H1->Draw("HIST");
}
