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
// $Id: B1SteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1Analysis.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVolume) { 
    const B1DetectorConstruction* detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  
  //Hist Filling
  G4AnalysisManager*anMan = G4AnalysisManager::Instance();
	std::string partName = step->GetTrack()->GetDefinition()->GetParticleName();
	std::string neutron = "neutron";
	std::string proton = "proton";
	std::string gamma = "gamma";
	std::string eNeg = "e-";
	std::string ePos = "e+";
	std::string muNeg = "mu-";
	std::string muPos = "mu+";
	std::string pion = "pion";
	std::string kNeg = "kaon-";	
	std::string kPos = "kaon+";
	std::string k0 = "kaon0";

  //Fill zPos Hist
  if(step->GetTotalEnergyDeposit() > 0*MeV){	
//	G4AnalysisManager* anMan = G4AnalysisManager::Instance();
	G4StepPoint* stepPoint = step->GetPostStepPoint();
	G4ThreeVector pos = stepPoint->GetPosition();
	G4double zCoord = pos.getZ();
	anMan->FillH1(30, zCoord);
  }

  //Fill uncaught within scints Hists
  G4string volume = step->GetTrack()->GetVolume()->GetName();
  G4string nextVolume = step->GetTrack()->GetNextVolume()->GetName();
  //out
  if((((volume.compare("outScint")==0) && (step->GetTotalEnergyDeposit()<0.5*MeV)) ||
	(volume.compare("World")=0)) && 
	(nextVolume.compare("outScint")!=0)){
	anMan->FillH1(64, step->GetTrack->GetTotalEnergy());
	if(partName.compare(neutron) == 0){
	   anMan->FillH1(63, 0.5);
	}
	if(partName.compare(proton) == 0){
	   anMan->FillH1(63, 1.5);
	}
	if(partName.compare(gamma) == 0){
	   anMan->FillH1(63, 2.5);
	}
	if(partName.compare(eNeg) == 0){
	   anMan->FillH1(63, 3.5);
	}
	if(partName.compare(ePos) == 0){
	   anMan->FillH1(63, 4.5);
	}
	if(partName.compare(muNeg) == 0){
	   anMan->FillH1(63, 5.5);
	}
	if(partName.compare(muPos) == 0){
	   anMan->FillH1(63, 5.5);
	}
	if(partName.compare(pion) == 0){
	   anMan->FillH1(63, 6.5);
	}
	if(partName.compare(kPos) == 0){
	   anMan->FillH1(63, 7.5);
	}
	if(partName.compare(kNeg) == 0){
	   anMan->FillH1(63, 7.5);
	}
	if(partName.compare(k0) == 0){
	   anMan->FillH1(63, 0.5);
	}

  }

  //in
  if((step->GetPostStepPoint()->GetPhysicalVolume()->GetName()=="Lead") && (
	(step->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="outScint") || (step->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="World"))){
	anMan->FillH1(64, step->GetTrack->GetTotalEnergy());
	if(partName.compare(neutron) == 0){
	   anMan->FillH1(63, 0.5);
	}
	if(partName.compare(proton) == 0){
	   anMan->FillH1(63, 1.5);
	}
	if(partName.compare(gamma) == 0){
	   anMan->FillH1(63, 2.5);
	}
	if(partName.compare(eNeg) == 0){
	   anMan->FillH1(63, 3.5);
	}
	if(partName.compare(ePos) == 0){
	   anMan->FillH1(63, 4.5);
	}
	if(partName.compare(muNeg) == 0){
	   anMan->FillH1(63, 5.5);
	}
	if(partName.compare(muPos) == 0){
	   anMan->FillH1(63, 5.5);
	}
	if(partName.compare(pion) == 0){
	   anMan->FillH1(63, 6.5);
	}
	if(partName.compare(kPos) == 0){
	   anMan->FillH1(63, 7.5);
	}
	if(partName.compare(kNeg) == 0){
	   anMan->FillH1(63, 7.5);
	}
	if(partName.compare(k0) == 0){
	   anMan->FillH1(63, 0.5);
	}

  }

 
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;


  //Fill Particle Type of Ge Edep Hist: caught or uncaught!!!!
  if(step->GetTotalEnergyDeposit() > 0.05*MeV){
//	G4AnalysisManager* anMan = G4AnalysisManager::Instance();

	if(partName.compare(neutron) == 0){
	   anMan->FillH1(60, 0.5);
	   fEventAction->AddParticle(0.5);
	}
	if(partName.compare(proton) == 0){
	   anMan->FillH1(60, 1.5);
	   fEventAction->AddParticle(1.5);
	}
	if(partName.compare(gamma) == 0){
	   anMan->FillH1(60, 2.5);
	   fEventAction->AddParticle(2.5);
	}
	if(partName.compare(eNeg) == 0){
	   anMan->FillH1(60, 3.5);
	   fEventAction->AddParticle(3.5);
	}
	if(partName.compare(ePos) == 0){
	   anMan->FillH1(60, 4.5);
	   fEventAction->AddParticle(4.5);
	}
	if(partName.compare(muNeg) == 0){
	   anMan->FillH1(60, 5.5);
	   fEventAction->AddParticle(5.5);
	}
	if(partName.compare(muPos) == 0){
	   anMan->FillH1(60, 5.5);
	   fEventAction->AddParticle(5.5);
	}
	if(partName.compare(pion) == 0){
	   anMan->FillH1(60, 6.5);
	   fEventAction->AddParticle(6.5);
	}
	if(partName.compare(kPos) == 0){
	   anMan->FillH1(60, 7.5);
	   fEventAction->AddParticle(7.5);
	}
	if(partName.compare(kNeg) == 0){
	   anMan->FillH1(60, 7.5);
	   fEventAction->AddParticle(7.5);
	}
	if(partName.compare(k0) == 0){
	   anMan->FillH1(60, 0.5);
	   fEventAction->AddParticle(7.5);
	}

  }

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

