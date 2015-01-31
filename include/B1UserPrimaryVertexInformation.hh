#include "G4VUserPrimaryVertexInformation.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#ifndef B1UserPrimaryVertexInformation_h
#define B1UserPrimaryVertexInformation_h 1

class B1UserPrimaryVertexInformation : public G4VUserPrimaryVertexInformation
{
   public:
	B1UserPrimaryVertexInformation(){initNeutronEnergy = 0;}
	virtual ~B1UserPrimaryVertexInformation(){;}
	inline virtual void Print()const{};

	inline void SetNeutronEnergy(G4double nEnergy){initNeutronEnergy = nEnergy;}
	inline G4double GetNeutronEnergy(){return initNeutronEnergy;}


   private:
	G4double initNeutronEnergy;

};

#endif
