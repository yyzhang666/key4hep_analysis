#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/utils/kinematics.h"

#include "podio/Frame.h"
#include "podio/ROOTFrameReader.h"

#include "TFile.h"
#include "TH1D.h"

#include <string>
#include <vector>

void Example1()
{
  //gSystem->Load("libDelphes");

  // Create chain of root trees
  //TChain chain("events");
  //chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  //ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  //Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  //TClonesArray *branchJet = treeReader->UseBranch("Jet");
  //TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  //TClonesArray *branchEvent = treeReader->UseBranch("Event");
  const std::vector<std::string> inputFile={"delphes_events_edm4hep.root"};

  auto reader = podio::ROOTFrameReader();
  reader.openFiles(inputFile);
  //use Reader from podio

  // Book histograms
  TH1 *histJetPT = new TH1F("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histMass = new TH1F("mass", "M_{inv}(e_{1}, e_{2})", 100, 40.0, 140.0);


  // Loop over all events

  /*for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    
    //HepMCEvent *event = (HepMCEvent*) branchEvent -> At(0);
    //LHEFEvent *event = (LHEFEvent*) branchEvent -> At(0);
    //Float_t weight = event->Weight;

    // If event contains at least 1 jet
    if(branchJet->GetEntries() > 0)
    {
      // Take first jet
      Jet *jet = (Jet*) branchJet->At(0);

      // Plot jet transverse momentum
      histJetPT->Fill(jet->PT);

      // Print jet transverse momentum
      cout << "Jet pt: "<<jet->PT << endl;
    }

    Electron *elec1, *elec2;

    // If event contains at least 2 electrons
    //if(branchElectron->GetEntries() > 1)
    //{
    //  // Take first two electrons
    //  elec1 = (Electron *) branchElectron->At(0);
    //  elec2 = (Electron *) branchElectron->At(1);

      // Plot their invariant mass
    //  histMass->Fill(((elec1->P4()) + (elec2->P4())).M());
    //}
  }*/
  for (size_t i = 0; i < reader.getEntries("events"); ++i) {
    const auto event = podio::Frame(reader.readNextEntry("events"));

    const auto &jets =
        event.get<edm4hep::ReconstructedParticleCollection>("Jet");
    if(jets.size() > 0)
    {
      auto jet_pt=edm4hep::utils::p4(jets[0]);

      // Plot jet transverse momentum
      histJetPT->Fill(jet_pt.Pt());

      // Print jet transverse momentum
      std::cout << "Jet pt: "<<jet_pt.Pt() << std::endl;
    }

    //Electron *elec1, *elec2;
    const auto &electrons =
        event.get<edm4hep::ReconstructedParticleCollection>("Electrons");
    // If event contains at least 2 electrons
    //if(branchElectron->GetEntries() > 1)
    //{
    //  // Take first two electrons
    //  elec1 = (Electron *) branchElectron->At(0);
    //  elec2 = (Electron *) branchElectron->At(1);
      if (electrons.size()>1){
        const auto e1 = edm4hep::utils::p4(electrons[0]);
        const auto e2 = edm4hep::utils::p4(electrons[1]);

        const auto e_p4 = e1 + e2;
        histMass->Fill(e_p4.M());
      }
      // Plot their invariant mass
    //  histMass->Fill(((elec1->P4()) + (elec2->P4())).M());
    //}
  }

  // Show resulting histograms
  histJetPT->Draw();
  histMass->Draw();
}
