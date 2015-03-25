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
// $Id: B1EventAction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1Run.hh"
#include "B1Analysis.hh"
#include "B1UserPrimaryVertexInformation.hh"
//#include "PrimaryGeneratorAction.hh"
//#include "CRYParticle.h"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4UImanager.hh"

#include <iomanip>
//#include <list>
//#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction()
: G4UserEventAction(),
  fEdep(0.),
  fGeEdepHCID(-1), //why? where?
  fOScEdepHCID(-1),
  fIScEdepHCID(-1),
  fWorldEdepHCID(-1),
  fMid1ScEdepHCID(-1)
{
  partNumbers.clear();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....................................

G4THitsMap<G4double>* B1EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const //why extra const??
{
	G4THitsMap<G4double>* hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));
	if (!hitsCollection){
		G4ExceptionDescription msg;
		msg << "Cannot access hitsCollection ID" <<hcID;
		G4Exception("B1EventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
	}
	return hitsCollection;
}

//.....................................

G4double B1EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
	G4double sumValue = 0;
	std::map<G4int, G4double*>::iterator it;
	for (it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++){
		sumValue += *(it->second); //second?
	}
	return sumValue;
}

//........................................

void B1EventAction::PrintEventStatistics(G4double geEdep, G4double outScEdep, G4double inScEdep, G4double worldEdep) const
{
	G4cout <<"Cylinder total energy: " <<std::setw(7) <<G4BestUnit(geEdep, "Energy") <<G4endl;
	G4cout <<"Out Scint total energy: " <<std::setw(7) <<G4BestUnit(outScEdep, "Energy") <<G4endl;
	G4cout <<"In Scint total energy: " <<std::setw(7) <<G4BestUnit(inScEdep, "Energy") <<G4endl;
	G4cout <<"World total energy: " <<std::setw(7) <<G4BestUnit(worldEdep, "Energy") <<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//............................................

void B1EventAction::EndOfEventAction(const G4Event* event) //need event object name? (like in B4)
{   
	//get hist collection ID's
	if (fGeEdepHCID == -1){
		fGeEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("Cylinder/Edep");
	}
	if (fIScEdepHCID == -1){
		fIScEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("inScint/InEdep");
	}
	if (fOScEdepHCID == -1){
		fOScEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("outScint/OutEdep");
	}
	if (fWorldEdepHCID == -1){
		fWorldEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("World/WorldEdep");
	}
	if (fMid1ScEdepHCID == -1){
		fMid1ScEdepHCID = G4SDManager::GetSDMpointer()->GetCollectionID("mid1Scint/Mid1Edep");

	//get sum values from hits collection
	G4double geEdep = GetSum(GetHitsCollection(fGeEdepHCID, event));
	G4double inScEdep = GetSum(GetHitsCollection(fIScEdepHCID, event));
	G4double outScEdep = GetSum(GetHitsCollection(fOScEdepHCID, event));
	G4double worldEdep = GetSum(GetHitsCollection(fWorldEdepHCID, event));
	G4double mid1ScEdep = GetSum(GetHitsCollection(fMid1ScEdepHCID, event));
	//get Analysis Manager
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	//fill histograms!

	G4int uncaught = 0;

	analysisManager->FillH1(1, geEdep);
	if(outScEdep >= 0.*MeV){analysisManager->FillH1(2, outScEdep);}
	if(inScEdep >= 0.*MeV){analysisManager->FillH1(3, inScEdep);}
	analysisManager->FillH1(4, worldEdep);
	if((inScEdep < 2.*MeV) && (outScEdep < 2.*MeV)){
		analysisManager->FillH1(5, geEdep);
		uncaught=1;
	}//G-IO
	if((inScEdep < 2.*MeV) && (outScEdep < 2.*MeV) && (mid1ScEdep < 2.*MeV)){
		analysisManager->FillH1(67, geEdep);
	}//G-IOM
	if(outScEdep < 2*MeV){
		analysisManager->FillH1(6, geEdep);
	}
	if(inScEdep < 2*MeV){
		analysisManager->FillH1(7, geEdep);
	}
	if(((inScEdep < 2*MeV) && (outScEdep >= 2*MeV)) || ((inScEdep >= 2*MeV) && (outScEdep < 2*MeV)) || ((inScEdep < 2*MeV) && (outScEdep < 2*MeV))){//weird
		analysisManager->FillH1(8, geEdep);
	}
	analysisManager->FillH1(9, geEdep);
	if(outScEdep >= 0*MeV){analysisManager->FillH1(10, outScEdep);}
	if(inScEdep >= 0*MeV){analysisManager->FillH1(11, inScEdep);}
	analysisManager->FillH1(12, worldEdep);
	if((inScEdep < 2*MeV) && (outScEdep < 2*MeV) && (geEdep > 0.05*MeV)){
		analysisManager->FillH1(13, geEdep);
	}//bigG-IO
	if((inScEdep < 2*MeV) && (outScEdep < 2*MeV) && (mid1ScEdep < 2*MeV) && (geEdep > 0.05*MeV)){
		analysisManager->FillH1(68, geEdep);
	}//bigG-IOM
	if(outScEdep < 2*MeV && geEdep > 0.05*MeV){
		analysisManager->FillH1(14, geEdep);
	}
	if(inScEdep < 2*MeV && geEdep > 0.05*MeV){
		analysisManager->FillH1(15, geEdep);
	}
	if(((inScEdep < 2*MeV) && (outScEdep >= 2*MeV)) || ((inScEdep >= 2*MeV) && (outScEdep < 2*MeV))){//weird things
		analysisManager->FillH1(16, geEdep);
	}

	//Filling Edep by particle type---------------------
	G4int nVertex = event->GetNumberOfPrimaryVertex();
	
	for(G4int i=0;i<nVertex;i++){
		G4PrimaryVertex* primVertex = event->GetPrimaryVertex(i);
		G4PrimaryParticle* primPart = primVertex->GetPrimary(0);
  		G4ParticleDefinition* partDef = primPart->GetG4code();
  		std::string partName = partDef->GetParticleName();	
		//std::string name = event->GetPrimaryVertex(i)->GetPrimary(0)->GetG4code->GetParticleName();
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

		if(partName.compare(neutron) == 0){
		  analysisManager->FillH1(18, geEdep);
		  analysisManager->FillH1(19, geEdep);
		  analysisManager->FillH1(17, .5);
			if(uncaught == 1){
			  analysisManager->FillH1(51, geEdep);//neutron-IO
			  if(geEdep>0){
				analysisManager->FillH1(59, .5);
			  }
			}
			if((uncaught == 1) && (mid1ScEdep < 2.*MeV)){
			  analysisManager->FillH1(69, geEdep);//neutron-IOM
			}
		}else if(partName.compare(proton) == 0){
		  analysisManager->FillH1(20, geEdep);
		  analysisManager->FillH1(21, geEdep);
		  analysisManager->FillH1(17, 1.5);
			if(uncaught == 1){
			  analysisManager->FillH1(52, geEdep);
			  if(geEdep>0){
			  	analysisManager->FillH1(59, 1.5);
			  }
			}
		}else if(partName.compare(gamma) == 0){
		  analysisManager->FillH1(22, geEdep);
		  analysisManager->FillH1(23, geEdep);
		  analysisManager->FillH1(17, 2.5);
			if(uncaught == 1){
			  analysisManager->FillH1(53, geEdep);
			  if(geEdep>0){			  
				analysisManager->FillH1(59, 2.5);
			  }
			}
		}else if(partName.compare(eNeg) == 0){
		  analysisManager->FillH1(24, geEdep);
		  analysisManager->FillH1(25, geEdep);
		  analysisManager->FillH1(17, 3.5);
			if(uncaught == 1){
			  analysisManager->FillH1(55, geEdep);
			  if(geEdep>0){
			  	analysisManager->FillH1(59, 3.5);
			  }
			}
		}else if(partName.compare(ePos) == 0){
		  analysisManager->FillH1(31, geEdep);
		  analysisManager->FillH1(32, geEdep);
		  analysisManager->FillH1(17, 4.5);
			if(uncaught == 1){
			  analysisManager->FillH1(54, geEdep);
			  if(geEdep>0){
			  	analysisManager->FillH1(59, 4.5);
			  }
			}
		}else if(partName.compare(muNeg) == 0){
		  analysisManager->FillH1(26, geEdep);
		  analysisManager->FillH1(27, geEdep);
		  analysisManager->FillH1(17, 5.5);
			if(uncaught == 1){
			  analysisManager->FillH1(57, geEdep);
			  if(geEdep>0){
			  	analysisManager->FillH1(59, 5.5);
			  }
			}
		}else if(partName.compare(muPos) == 0){
		  analysisManager->FillH1(26, geEdep);
		  analysisManager->FillH1(27, geEdep);
		  analysisManager->FillH1(17, 5.5);
			if(uncaught == 1){
			  analysisManager->FillH1(57, geEdep);
			  if(geEdep>0){
			  	analysisManager->FillH1(59, 5.5);
			  }
			}
		}else if(partName.compare(pion) == 0){
		  analysisManager->FillH1(28, geEdep);
		  analysisManager->FillH1(29, geEdep);
		  analysisManager->FillH1(17, 6.5);
			if(uncaught == 1){
			  analysisManager->FillH1(58, geEdep);
			  if(geEdep>0){
			    analysisManager->FillH1(59, 6.5);
			  }
			}
		}else if((partName.compare(kPos) == 0) || (partName.compare(kNeg) == 0) || (partName.compare(k0) ==0)){
		  analysisManager->FillH1(40, geEdep);
		  analysisManager->FillH1(41, geEdep);
		  analysisManager->FillH1(17, 7.5);
			if(uncaught == 1){
			  analysisManager->FillH1(56, geEdep);
			  if(geEdep>0){
			    analysisManager->FillH1(59, 7.5);
			  }
			}
		}

	} 
	if(uncaught == 1 && geEdep > 0.05*MeV){
	   G4PrimaryVertex* primVertex = event->GetPrimaryVertex(0);
	   B1UserPrimaryVertexInformation* vertexInfo = (B1UserPrimaryVertexInformation*)primVertex->GetUserInformation(); 
	   analysisManager->FillH1(62, vertexInfo->GetNeutronEnergy()); 
	   std::list<G4double>::iterator it; 
	   for(it = partNumbers.begin(); it != partNumbers.end(); ++it){
		analysisManager->FillH1(61, *it);
	   }
	}
	//--------------------------------------------------
	//Edep by zPos filled in SteppingAction (Hist #30)
	//**H31+32 filled above!
	//H33-39,42 (initE) filled in PrimaryGeneratorAction
	//H43-47 (initDir) filled in PrimaryGeneratorAction
	//H60 (any Ge Edep Particle) filled in SteppingAction
	//H63-66 (about particles uncaught coming thru scints) filled in SteppingAction

	 // accumulate statistics in B1Run
	  B1Run* run  = static_cast<B1Run*>(
  	      G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	  run->AddEdep(fEdep);

	//Trajectories
   if(geEdep>0.05*MeV){

	G4UImanager * trajUI = G4UImanager::GetUIpointer();
	trajUI->ApplyCommand("/control/execute vis2.mac");

	G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
	G4int n_traj = 0;
	if (trajectoryContainer) n_traj = trajectoryContainer->entries();
	for(G4int i=0; i<n_traj; i++){
		G4Trajectory* trj = (G4Trajectory*)((*(event->GetTrajectoryContainer()))[i]);
		trj->ShowTrajectory();
		trj->DrawTrajectory();
	}
	
   }

	//print per event
	G4int eventID = event ->GetEventID();
	G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
//	if((printModulo > 0) && (eventID % printModulo == 1)){
//		if(eventID % 100000 == 0){
//			G4cout <<"----> End of Event: " <<eventID <<" <------" <<G4endl;
			//PrintEventStatistics(geEdep, outScEdep, inScEdep, worldEdep);
//		}
//	}
}
}
/*
void B1EventAction::AddParticle(G4double partNum)
{
	partNumbers.push_back(partNum);
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
