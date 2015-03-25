//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B1RunAction.cc 75216 2013-10-29 16:08:11Z gcosmo $
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
//#include "B1PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction()
{ 
  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);        

	//set printing event number
	G4RunManager::GetRunManager()->SetPrintProgress(1);

	//Create analysis manager with root
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	G4cout <<"Using " <<analysisManager->GetType() <<G4endl; //where on earth is GetType?
	//analysisManager->SetActivation(true);

	//Create directories
	analysisManager->SetHistoDirectoryName("Histograms");
	analysisManager->SetVerboseLevel(1);
	analysisManager->SetFirstHistoId(1);

	//Create histos
	analysisManager->CreateH1("GE", "Edep in Ge cylinder", 1000, 0., 3*MeV);//1
	analysisManager->CreateH1("OSC", "Edep in Out Scint", 1000, 0., 3*MeV);//2
	analysisManager->CreateH1("ISC", "Edep in In Scint", 1000, 0., 3*MeV);//3
	analysisManager->CreateH1("WORLD", "Edep in World", 1000, 0., 3*MeV);//4
	analysisManager->CreateH1("G-IO", "Edep in Ge with In and Out Events Removed", 1000, 0, 3*MeV);//5
	analysisManager->CreateH1("G-O", "Edep in Ge with Out Events Removed", 1000, 0, 3*MeV);//6
	analysisManager->CreateH1("G-I", "Edep in Ge with In Events Removed", 1000, 0, 3*MeV);//7
	analysisManager->CreateH1("IorO", "Edep with In or Out Events Removed", 1000, 0, 3*MeV);//8
	analysisManager->CreateH1("bigGE", "Edep in Ge cylinder", 1000, 0., 30*MeV);//9
	analysisManager->CreateH1("bigOSC", "Edep in Out Scint", 1000, 0., 100*MeV);//10
	analysisManager->CreateH1("bigISC", "Edep in In Scint", 1000, 0., 100*MeV);//11
	analysisManager->CreateH1("bigWORLD", "Edep in World", 1000, 0., 100*MeV);//12
	analysisManager->CreateH1("bigG-IO", "Edep in Ge with In and Out Events Removed", 1000, 0, 30*MeV);//13
	analysisManager->CreateH1("bigG-O", "Edep in Ge with Out Events Removed", 1000, 0, 30*MeV);//14
	analysisManager->CreateH1("bigG-I", "Edep in Ge with In Events Removed", 1000, 0, 30*MeV);//15
	analysisManager->CreateH1("bigIorO", "Edep with In or Out Events Removed", 1000, 0, 30*MeV);//16
	analysisManager->CreateH1("partTypes", "Distribution of Particle Types", 8, 0, 8);//17
	analysisManager->CreateH1("neutron", "Edep from Neutrons", 1000, 0, 3*MeV);//18
	analysisManager->CreateH1("bigNeutron", "Edep from Neutrons", 1000, 0, 100*MeV);//19
	analysisManager->CreateH1("proton", "Edep from Protons", 1000, 0, 3*MeV);//20
	analysisManager->CreateH1("bigProton", "Edep from Protons", 1000, 0, 100*MeV);//21
	analysisManager->CreateH1("gamma", "Edep from Gammas", 1000, 0, 3*MeV);//22
	analysisManager->CreateH1("bigGamma", "Edep from Gammas", 1000, 0, 100*MeV);//23
	analysisManager->CreateH1("electron", "Edep from Electrons", 1000, 0, 3*MeV);//24
	analysisManager->CreateH1("bigElectron", "Edep from Electrons", 1000, 0, 100*MeV);//25
	analysisManager->CreateH1("muon", "Edep from Muons", 1000, 0, 3*MeV);//26
	analysisManager->CreateH1("bigMuon", "Edep from Muons", 1000, 0, 100*MeV);//27
	analysisManager->CreateH1("pion", "Edep from Pions", 1000, 0, 3*MeV);//28
	analysisManager->CreateH1("bigPion", "Edep from Pions", 1000, 0, 100*MeV);//29
	//kaon hists below
	analysisManager->CreateH1("zPos", "z-Position of each Edep (by Step)", 1000, -5.5*m, 5.5*m);//30
	analysisManager->CreateH1("positron", "Edep from Positrons", 1000, 0, 3*MeV);//31
	analysisManager->CreateH1("bigPositron", "Edep from Positrons", 1000, 0, 100*MeV);//32
	analysisManager->CreateH1("initENeutron", "Energy of CRY-generated neutrons", 1000, 0, 100*MeV);//33
	analysisManager->CreateH1("initEProton", "Energy of CRY-generated protons", 1000, 0, 100*MeV);//34
	analysisManager->CreateH1("initEGamma", "Energy of CRY-generated gammas", 1000, 0, 100*MeV);//35
	analysisManager->CreateH1("initEElectron", "Energy of CRY-generated electrons", 1000, 0, 100*MeV);//36
	analysisManager->CreateH1("initEPositron", "Energy of CRY-generated positrons", 1000, 0, 100*MeV);//37
	analysisManager->CreateH1("initEMuon", "Energy of CRY-generated muons", 1000, 0, 100*MeV);//38
	analysisManager->CreateH1("initEPion", "Energy of CRY-generated pions", 1000, 0, 100*MeV);//39
	analysisManager->CreateH1("kaon", "Edep from Kaons", 1000, 0, 3*MeV);//40
	analysisManager->CreateH1("bigKaon", "Edep from Kaons", 1000, 0, 100*MeV);//41
	analysisManager->CreateH1("initEKaon", "Energy of CRY-generated kaons", 1000, 0, 100*MeV);//42
	analysisManager->CreateH1("initDirNeutron", "Initial Neutron Direction", 1000, -1, 1);//43
	analysisManager->CreateH1("initDirProton", "Initial Proton Direction", 1000, -1, 1);//44
	analysisManager->CreateH1("initDirGamma", "Initial Gamma Direction", 1000, -1, 1);//45
	analysisManager->CreateH1("initDirElectron", "Initial Electron Direction", 1000, -1, 1);//46
	analysisManager->CreateH1("initDirPositron", "Initial Positron Direction", 1000, -1, 1);//47
	analysisManager->CreateH1("initDirMuon", "Initial Muon Direction", 1000, -1, 1);//48
	analysisManager->CreateH1("initDirPion", "Initial Pion Direction", 1000, -1, 1);//49
	analysisManager->CreateH1("initDirKaon", "Initial Kaon Direction", 1000, -1, 1);//50

	analysisManager->CreateH1("neutron-IO", "Edep from Neutrons without In or Out Events", 1000, 0, 100*MeV);//51
	analysisManager->CreateH1("proton-IO", "Edep from Protons without In or Out Events", 1000, 0, 100*MeV);//52
	analysisManager->CreateH1("gamma-IO", "Edep from Gammas without In or Out Events", 1000, 0, 100*MeV);//53
	analysisManager->CreateH1("positron-IO", "Edep from Positrons without In or Out Events", 1000, 0, 100*MeV);//54
	analysisManager->CreateH1("electron-IO", "Edep from Electrons without In or Out Events", 1000, 0, 100*MeV);//55
	analysisManager->CreateH1("kaon-IO", "Edep from Kaons without In or Out Events", 1000, 0, 100*MeV);//56
	analysisManager->CreateH1("muon-IO", "Edep from Muons without In or Out Events", 1000, 0, 100*MeV);//57
	analysisManager->CreateH1("pion-IO", "Edep from Pions without In or Out Events", 1000, 0, 100*MeV);//58

	analysisManager->CreateH1("uncaughtTypes", "Distribution of Initial Particle Types of Uncaught Events", 8, 0, 8);//59

	analysisManager->CreateH1("geParticle", "Particle causing any Ge Edep", 8, 0, 8);//60 
	analysisManager->CreateH1("uncaughtGeParticle", "Particle causing uncaught Ge Edep", 8, 0, 8);//61
	analysisManager->CreateH1("uncaughtInitENeutron", "Initial Neutron Energy Spectrum for Uncaught Events", 5000, 0, 500*MeV);//62
	analysisManager->CreateH1("partInOut", "Particle Types Uncaught Coming Through Out Scint", 8, 0, 0);//63
	analysisManager->CreateH1("energyInOut", "Energy of Particles Uncaught Coming Through Out Scint", 1000, 0, 100*MeV);//64
	analysisManager->CreateH1("partInIn", "Particle Types Uncaught Coming Through In Scint", 8, 0, 0);//65
	analysisManager->CreateH1("energyInIn", "Energy of Particles Uncaught Coming Through In Scint", 1000, 0, 100*MeV);//66

	//for testing with additional (mid) scint layers within cheap layers
	analysisManager->CreateH1("G-IOM", "Ge Edep for events with no scint edep (I,O,M)", 1000, 0, 3*MeV);//67
	analysisManager->CreateH1("bigG-IOM", "Ge Edep for events with no scint edep (I,O,M)",1000, 0, 30*MeV);//68
	analysisManager->CreateH1("neutron-IOM", "Edep from neutrons with no scint edep (I,O,M)", 1000, 0, 100*MeV);//69
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
	delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* B1RunAction::GenerateRun()
{
  return new B1Run; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
 // G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	//Get Analysis Manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	//Open an output file
	//G4String fileName = "testHistograms";
	//analysisManager->OpenFile(fileName);
	analysisManager->OpenFile();
	G4cout<<"Output file opened and run begun"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  const B1Run* b1Run = static_cast<const B1Run*>(run);

  // Compute dose
  //
  G4double edep  = b1Run->GetEdep();
  G4double edep2 = b1Run->GetEdep2();
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const B1DetectorConstruction* detectorConstruction
   = static_cast<const B1DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
/*
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
 
G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
*/
	//print histo stats
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->GetH1(1)){
		G4cout <<"\n -----> print histograms statistic ";
		if(isMaster){
			G4cout <<"for the entire run \n" <<G4endl;
		}
		else{
			G4cout <<"for the local thread \n" <<G4endl;
		}
		G4cout <<"EGe: mean = " <<G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") << " rms = " <<G4BestUnit(analysisManager->GetH1(1)->rms(), "Energy") <<G4endl;
	}
	if (analysisManager->GetH1(2)){
		G4cout <<"\n -----> print histograms statistic ";
		if(isMaster){
			G4cout <<"for the entire run \n" <<G4endl;
		}
		else{
			G4cout <<"for the local thread \n" <<G4endl;
		}
		G4cout <<"EOutSc: mean = " <<G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy") << " rms = " <<G4BestUnit(analysisManager->GetH1(2)->rms(), "Energy") <<G4endl;
	}
	if (analysisManager->GetH1(3)){
		G4cout <<"\n -----> print histograms statistic ";
		if(isMaster){
			G4cout <<"for the entire run \n" <<G4endl;
		}
		else{
			G4cout <<"for the local thread \n" <<G4endl;
		}
		G4cout <<"EInSc: mean = " <<G4BestUnit(analysisManager->GetH1(3)->mean(), "Energy") << " rms = " <<G4BestUnit(analysisManager->GetH1(3)->rms(), "Energy") <<G4endl;
	}
	if (analysisManager->GetH1(4)){
		G4cout <<"\n -----> print histograms statistic ";
		if(isMaster){
			G4cout <<"for the entire run \n" <<G4endl;
		}
		else{
			G4cout <<"for the local thread \n" <<G4endl;
		}
		G4cout <<"EWorld: mean = " <<G4BestUnit(analysisManager->GetH1(4)->mean(), "Energy") << " rms = " <<G4BestUnit(analysisManager->GetH1(4)->rms(), "Energy") <<G4endl;
	}

	//save histograms
	analysisManager->Write();
	analysisManager->CloseFile();
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << "\n--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << "\n--------------------End of Local Run------------------------";
  }
  /*
  G4cout
     << "\n The run consists of " << nofEvents << " "<< runCondition
     << "\n Dose in scoring volume : " 
     << G4BestUnit(dose,"Dose") << " +- " << G4BestUnit(rmsDose,"Dose")
     << "\n------------------------------------------------------------\n"
     << G4endl;
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
