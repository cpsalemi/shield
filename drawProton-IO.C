{
TCanvas * c1 = new TCanvas();
c1->Update();
c1->SetLogy();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
TH1D* H1;
dir->GetObject("proton-IO", H1);
H1->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H1->GetYaxis()->SetTitle("Count");
std::cout <<H1  <<std::endl;
H1->Draw("HIST");
}
