#ifndef B1TrackingAction_h
#define B1TrackingAction_h 1

#include "G4UserTrackingAction.hh"

class B1TrackingAction : public G4UserTrackingAction
{
  public:
	B1TrackingAction();
	virtual ~B1TrackingAction(){};

	virtual void PreUserTrackingAction(const G4Track*);
	virtual void PostUserTrackingAction(const G4Track*);
};

#endif
