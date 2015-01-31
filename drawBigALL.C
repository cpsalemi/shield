{
TCanvas * c1 = new TCanvas();
c1->SetLogy();
c1->SetTitle("Superimposed Histograms");
TFile f("testHistograms.root");
TDirectory* dir = gFile->GetDirectory("Histograms");
gStyle->SetOptStat(0);

TH1D* H6;
dir->GetObject("bigOSC", H6);
H6->SetLineColor(kRed);
H6->SetTitle("Superimposed Histograms");
H6->GetXaxis()->SetTitle("Energy Deposited (MeV)");
H6->GetYaxis()->SetTitle("Count");
H6->Draw("HIST");

TH1D* H5;
dir->GetObject("bigWORLD", H5);
H5->SetLineColor(kMagenta);
H5->Draw("HIST SAME");

TH1D* H1;
dir->GetObject("bigGE", H1);
H1->SetLineColor(kCyan);
H1->Draw("HIST SAME");

TH1D* H2;
dir->GetObject("bigG-I", H2);
H2->SetLineColor(kBlue);
H2->Draw("HIST SAME");

TH1D* H3;
dir->GetObject("bigG-O", H3);
H3->SetLineColor(kYellow);
H3->Draw("HIST SAME");

TH1D* H4;
dir->GetObject("bigG-IO", H4);
H4->SetLineColor(kGreen);
H4->Draw("HIST SAME");

TH1D* H7;
dir->GetObject("bigISC", H7);
H7->SetLineColor(kBlue-3);
H7->Draw("HIST SAME");

leg = new TLegend(0.7,0.7,0.9,0.9);
   leg->SetHeader("Legend");
   leg->AddEntry(H5,"World","l");
   leg->AddEntry(H6,"OSC","l");
   leg->AddEntry(H7, "ISC","l");
   leg->AddEntry(H1,"GE","l");
   leg->AddEntry(H2,"G-I","l");
   leg->AddEntry(H3,"G-O","l");
   leg->AddEntry(H4,"G-IO","l");
   leg->Draw();

return c1;
}
