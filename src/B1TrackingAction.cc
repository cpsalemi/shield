#include "B1TrackingAction.hh"

#include "G4Trajectory.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"

B1TrackingAction::B1TrackingAction() : G4UserTrackingAction()
{;}

void B1TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
	fpTrackingManager->SetStoreTrajectory(true);
	fpTrackingManager->SetTrajectory(new G4Trajectory(aTrack));
	//B1TrackInformation* info = new B1TrackInformation(aTrack); **here??
}

void B1TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
	//std::list<std::string> aTrackInfoList;**maybe don't need b/c Trajectory?
	
}
