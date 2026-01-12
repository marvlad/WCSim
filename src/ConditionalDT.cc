#include "ConditionalDT.hh"

ConditionalDT::ConditionalDT(const std::string& file,
                             const std::string& hname)
{
  file_ = TFile::Open(file.c_str(), "READ");
  if (!file_ || file_->IsZombie())
    throw std::runtime_error("Cannot open ROOT file");

  file_->GetObject(hname.c_str(), h2_);
  if (!h2_)
    throw std::runtime_error("TH2F not found");

  //rng_ = new TRandom3(seed); 
}

ConditionalDT::~ConditionalDT() {
  //delete rng_;
  if (file_) { file_->Close(); delete file_; }
}

double ConditionalDT::dt_sample(float E)
{
  const int xb = h2_->GetXaxis()->FindFixBin(E);
  TH1D* hdt = h2_->ProjectionY("hdt_tmp", xb, xb);

  const double sum = hdt->Integral();
  if (sum <= 0) { delete hdt; return 0.0; }

  const int nb = hdt->GetNbinsX();
  const double u = G4UniformRand() * sum;

  double c = 0.0;
  int yb = nb;
  for (int b = 1; b <= nb; ++b) {
    c += hdt->GetBinContent(b);
    if (u <= c) { yb = b; break; }
  }

  // uniform within selected bin
  const double xlow = hdt->GetXaxis()->GetBinLowEdge(yb);
  const double xup  = xlow + hdt->GetXaxis()->GetBinWidth(yb);
  const double dt = xlow + G4UniformRand() * (xup - xlow);

  delete hdt;
  return dt;
}
