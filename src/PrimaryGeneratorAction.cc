//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include <cmath>
#include <ctime>
#include "PrimaryGeneratorAction.hh"
#include "B1Analysis.hh"
#include "B1UserPrimaryVertexInformation.hh"

using namespace std;

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleMomentum.hh"
#include "Randomize.hh"
//----------------------------------------------------------------------------//
PrimaryGeneratorAction::PrimaryGeneratorAction(const char *inputfile)
{
    // define a particle gun
  particleGun = new G4ParticleGun();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,".");

    gen = new CRYGenerator(setup);

    // set random number generator
/*    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
	G4cout <<"Seed CRY constructor: " <<RNGWrapper<CLHEP::HepRandomEngine>::getSeed() <<G4endl;
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
*/
	CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
	G4cout <<"Seed CRY constructor: " <<CLHEP::HepRandom::getTheSeed() <<G4endl;
	G4double myFlat = CLHEP::HepRandom::getTheEngine()->flat();
	//G4cout <<"myFlat= " <<myFlat <<G4endl;
	InputState=0;

  }
  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new PrimaryGeneratorMessenger(this);
  }

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,".");

  gen = new CRYGenerator(setup);

  // set random number generator
	CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
	long theTime = time(NULL);
	long seeds[2];
	seeds[0] = (long) theTime;
	seeds[1] = (long) theTime*G4UniformRand();
	CLHEP::HepRandom::setTheSeeds(seeds);

  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
	G4cout <<"rng " <<RNGWrapper<CLHEP::HepRandomEngine>::rng <<G4endl;
  InputState=0;

//WRONG VERSION!! USE ABOVE RANDOM SEED GENERATOR
/*	CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
	long theTime = time(NULL);
	long seeds[2];
	seeds[0] = (long) theTime;
	seeds[1] = (long) theTime*G4UniformRand();
	CLHEP::HepRandom::setTheSeeds(seeds);
*/	//G4cout <<"Seed UpdateCRY: " <<CLHEP::HepRandom::getTheSeed() <<G4endl;
	G4double myFlat = CLHEP::HepRandom::getTheEngine()->flat();
	//G4cout <<"myFlat= " <<myFlat <<G4endl;
	setup->setRandomFunction(myFlat);
	InputState=0;

}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,".");

    gen = new CRYGenerator(setup);

  // set random number generator
/*    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
	G4cout <<"Seed CRYFromFile: " <<RNGWrapper<CLHEP::HepRandomEngine>::getTheSeed() <<G4endl;
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
*/
	CLHEP::HepRandom::setTheEngine(new CLHEP::HepJamesRandom);
	G4cout <<"Seed CRYfromFile: " <<CLHEP::HepRandom::getTheSeed() <<G4endl;
	G4double myFlat = CLHEP::HepRandom::getTheEngine()->flat();
	G4cout <<"myFlat= " <<myFlat <<G4endl;
	InputState=0;
  }
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
    if (InputState != 0) {
    G4String* str = new G4String("CRY library was not successfully initialized");
    //G4Exception(*str);
    G4Exception("PrimaryGeneratorAction", "1",
                RunMustBeAborted, *str);
  }
  G4String particleName;
  vect->clear();
  gen->genEvent(vect);

  //....debug output
 /* G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size()
         << G4endl;
*/
  for ( unsigned j=0; j<vect->size(); j++) {
    particleName=CRYUtils::partName((*vect)[j]->id());
    //B1EventAction::SetParticleType(particleName);
/*
    //....debug output  
    cout << "  "          << particleName << " "
         << "charge="      << (*vect)[j]->charge() << " "
         << setprecision(4)
         << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
         << "pos (m)"
         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
         << " " << "direction cosines "
         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
         << " " << endl;
*/
    particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
    particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x()*m, (*vect)[j]->y()*m, (*vect)[j]->z()*m));
    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
    particleGun->SetParticleTime((*vect)[j]->t());
    particleGun->GeneratePrimaryVertex(anEvent);

    G4PrimaryVertex* primVertex = anEvent->GetPrimaryVertex(0);
    primVertex->SetUserInformation(new B1UserPrimaryVertexInformation);

    G4AnalysisManager* anMan = G4AnalysisManager::Instance();

    std::string neutron = "neutron";
    std::string proton = "proton";
    std::string gamma = "gamma";
    std::string electron = "electron";
    std::string muon = "muon";
    std::string pion = "pion";    
    std::string kaon = "kaon";

    G4ParticleMomentum partMomentum = particleGun->GetParticleMomentumDirection();
    G4double x = partMomentum.getX();
    G4double y = partMomentum.getY();
    G4double z = partMomentum.getZ();
    G4double cosDir = sqrt((x*x + y*y)/(x*x + y*y + z*z)); //cos(theta) below xy plane
	if(particleName.compare(neutron) == 0){
	  B1UserPrimaryVertexInformation* userInfo = (B1UserPrimaryVertexInformation*)primVertex->GetUserInformation();
	  userInfo->SetNeutronEnergy(particleGun->GetParticleEnergy());
          anMan->FillH1(33, particleGun->GetParticleEnergy());
	  anMan->FillH1(43, cosDir);
        }else if(particleName.compare(proton) == 0){
          anMan->FillH1(34, particleGun->GetParticleEnergy());
	  anMan->FillH1(44, cosDir);
        }else if(particleName.compare(gamma) == 0){
          anMan->FillH1(35, particleGun->GetParticleEnergy());
	  anMan->FillH1(45, cosDir);
        }else if((particleName.compare(electron) == 0) && (particleGun->GetParticleCharge() == -1)){
          anMan->FillH1(36, particleGun->GetParticleEnergy());
	  anMan->FillH1(46, cosDir);
	}else if((particleName.compare(electron) == 0) && (particleGun->GetParticleCharge() == 1)){
	  anMan->FillH1(37, particleGun->GetParticleEnergy());
	  anMan->FillH1(47, cosDir);
        }else if(particleName.compare(muon) == 0){
          anMan->FillH1(38, particleGun->GetParticleEnergy());
	  anMan->FillH1(48, cosDir);
        }else if(particleName.compare(pion) == 0){
          anMan->FillH1(39, particleGun->GetParticleEnergy());
	  anMan->FillH1(49, cosDir);
        }else if(particleName.compare(kaon) == 0){
	  anMan->FillH1(42, particleGun->GetParticleEnergy());
	  anMan->FillH1(50, cosDir);
	}

    delete (*vect)[j];
  }

}

