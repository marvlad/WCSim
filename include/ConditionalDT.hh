#pragma once
#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <stdexcept>
#include "Randomize.hh"

class ConditionalDT {
  public:
    ConditionalDT(const std::string& file,
                const std::string& hname); 

    ~ConditionalDT();

    bool isValid() const { return (file_ && h2_); }
    double dt_sample(float E); 

  private:
    TFile*     file_{nullptr};
    TH2F*      h2_{nullptr};
    //TRandom3*  rng_{nullptr};
};
