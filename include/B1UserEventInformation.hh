#include "G4VUserEventInformation.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#ifndef B1UserEventInformation_h
#define B1UserEventInformation_h 1

class B1UserEventInformation : public G4VUserEventInformation
{
   public:
	B1UserEventInformation()/*{initNeutronEnergy = 0;}*/;
	virtual ~B1UserEventInformation();
	inline virtual void Print()const{};

	inline void SetNeutronEnergy(G4double nEnergy){initNeutronEnergy = nEnergy;}
	inline G4double GetNeutronEnergy(){return initNeutronEnergy;}


   private:
	G4double initNeutronEnergy;

};

#endif
