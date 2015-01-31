{
TCanvas * c1 = new TCanvas();
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
gStyle->SetOptStat(0);
TH1D* H1;
dir->GetObject("uncaughtTypes", H1);
H1->GetYaxis()->SetTitle("Count");
H1->Draw("HIST");
leg = new TLegend(0.7,0.7,0.9,0.9);
   leg->SetHeader("Particle Types");
   leg->AddEntry((TObject*)0,"1-neutron","");
   leg->AddEntry((TObject*)0,"2-proton","");
   leg->AddEntry((TObject*)0,"3-gamma","");
   leg->AddEntry((TObject*)0,"4-electron","");
   leg->AddEntry((TObject*)0,"5-positron","");
   leg->AddEntry((TObject*)0,"6-muon","");
   leg->AddEntry((TObject*)0,"7-pion","");
   leg->AddEntry((TObject*)0,"8-kaon","");
   leg->Draw();
}
