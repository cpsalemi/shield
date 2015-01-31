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
// $Id: B1DetectorConstruction.cc 75117 2013-10-28 09:38:37Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
//#include "G4Cons.hh"
//#include "G4Orb.hh"
//#include "G4Sphere.hh"
//#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Color.hh"

#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager and SDManager
  G4NistManager* nist = G4NistManager::Instance();
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  //G4VisAttributes();

  //Declare inches
  static const G4double in = 2.54*cm;

  //Sizes and positions
  G4double env_sizeXY = 8*in, env_sizeZ = 8*in;
  G4double cuSize = env_sizeXY + 10*in;
  G4double pbSize = cuSize + 24*in;
  G4double inScintSize = pbSize + 4*in;
  G4double cheapSize = inScintSize + 24*in;
  G4double outScintSize = cheapSize + 4*in;
  G4double world_sizeXY = 10*m, world_sizeZ  = 10*m;
 
  G4ThreeVector center = G4ThreeVector(0,0, -3*m);
 
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
 G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

	//Scorer**********************************
	G4MultiFunctionalDetector* WorldDetector = new G4MultiFunctionalDetector("World");

	G4VPrimitiveScorer* primitiveW;
	primitiveW = new G4PSEnergyDeposit("WorldEdep");
	WorldDetector->RegisterPrimitive(primitiveW);
	SetSensitiveDetector("World",WorldDetector);


   //Out Scint
  G4Material* scint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Box* solidOutScint = new G4Box("outScint", 0.5*outScintSize, 0.5*outScintSize, 0.5*outScintSize);

//   void G4LogicalVolume::SetVisAttributes(const G4VisAttributes* pva);
 //   G4Colour green(0.,1.,0.);
  //G4VisAttributes outAtt = new G4VisAttributes(green);
  //G4VisAttributes* outAttrib = new G4VisAttributes(G4Colour(0.,1.,0.));
  G4LogicalVolume* logicOutScint = new G4LogicalVolume(solidOutScint, scint, "outScint");
 // logicOutScint->SetVisAttributes(outAtt);
  new G4PVPlacement(0, center, logicOutScint, "outScint", logicWorld, false, 0, checkOverlaps);
	//Scorer
	G4MultiFunctionalDetector* OutScDetector = new G4MultiFunctionalDetector("outScint");

	G4VPrimitiveScorer* primitiveOut;
	primitiveOut = new G4PSEnergyDeposit("OutEdep");
	OutScDetector->RegisterPrimitive(primitiveOut);
	SetSensitiveDetector("outScint",OutScDetector);

  //Cheap (steel)
  G4Material* steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Box* solidCheap = new G4Box("Cheap", 0.5*cheapSize, 0.5*cheapSize, 0.5*cheapSize);
  G4LogicalVolume* logicCheap = new G4LogicalVolume(solidCheap, steel, "Cheap");
  new G4PVPlacement(0, G4ThreeVector(), logicCheap, "Cheap", logicOutScint, false, 0, checkOverlaps);

  //In Scint
  G4Box* solidInScint = new G4Box("inScint", 0.5*inScintSize, 0.5*inScintSize, 0.5*inScintSize);
  G4LogicalVolume* logicInScint = new G4LogicalVolume(solidInScint, scint, "inScint");
  new G4PVPlacement(0, G4ThreeVector(), logicInScint, "inScint", logicCheap, false, 0, checkOverlaps);
	
	//Scorer
	G4MultiFunctionalDetector* InScDetector = new G4MultiFunctionalDetector("inScint");

	G4VPrimitiveScorer* primitiveIn;
	primitiveIn = new G4PSEnergyDeposit("InEdep");
	InScDetector->RegisterPrimitive(primitiveIn);
	SetSensitiveDetector("inScint",InScDetector);

  //Lead
  G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
  G4Box* solidPb = new G4Box("Lead", 0.5*pbSize, 0.5*pbSize, 0.5*pbSize);
  G4LogicalVolume* logicPb = new G4LogicalVolume(solidPb, lead, "Lead");
  new G4PVPlacement(0, G4ThreeVector(), logicPb, "Lead", logicInScint, false, 0, checkOverlaps);

  //Copper
  G4Material* copper = nist->FindOrBuildMaterial("G4_Cu");
  G4Box* solidCu = new G4Box("Copper", 0.5*cuSize, 0.5*cuSize, 0.5*cuSize);
  G4LogicalVolume* logicCu = new G4LogicalVolume(solidCu, copper, "Copper");
  new G4PVPlacement(0, G4ThreeVector(), logicCu, "Copper", logicPb, false, 0, checkOverlaps);

  //     
  // Envelope
  //  
    
 G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
 
 G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicCu,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //     
  // Shape 2-cylinder
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Ge");
  //G4ThreeVector pos2 = G4ThreeVector(0, 0, 0);
   
  G4double innerRadius = 0.*cm, outerRadius = 3.11*cm;
  G4double halfHeightZ = 3.11*cm;
  G4double startAngle  = 0.*deg, spanningAngle = 360.*deg;      
  G4Tubs* solidShape2 =    
    new G4Tubs("Shape2",                      //its name
              innerRadius, outerRadius, 
              halfHeightZ, startAngle, spanningAngle); //its size
                
  G4LogicalVolume* logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape2;

	//Scorer
	G4MultiFunctionalDetector* GeDetector = new G4MultiFunctionalDetector("Cylinder");

	G4VPrimitiveScorer* primitive;
	primitive = new G4PSEnergyDeposit("Edep");
	GeDetector->RegisterPrimitive(primitive);
	SetSensitiveDetector("Shape2",GeDetector);



  //
  //always return the physical World
  //
  return physWorld;
}
/*
void B1DetectorConstruction::ConstructSD()
{
	G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

	//Scorer
	G4MultiFunctionalDetector* GeDetector = new G4MultiFunctionalDetector("Cylinder");

	G4VPrimitiveScorer* primitive;
	primitive = new G4PSEnergyDeposit("Edep");
	GeDetector->RegisterPrimitive(primitive);

	//Do I need the charged filter?
	SetSensitiveDetector("Shape2",GeDetector);

}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
