// unit_Job.h //                                   \author Logan Jones
///////////////                                      \date 02-12-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_JOB_H_
#define _UNIT_JOB_H_
/////////////////////////////////////////////////////////////////////


enum job_Type {
	job_Project,		// Consume resource(s) for a set amount of time while working a project
	job_Economy,		// Produce and/or consume some resource(s) while active
};

enum job_Flags {
	job_Originator		= 1,
	job_UseWind			= 2,
	job_WindInit		= 4,
};


////////////////////////////// unit_Job /////////////////////////////
//
struct unit_Job {

	// Update the job progress+status
	// Return whether the job is still valid
	bool Update( long lSurplus, class game_Player* pPlayer );

	job_Type					Type;
	DWORD						Flags;
	bool						Active;

	class unit_Object*			Worker;
	unit_Project*				Project;

	long						Requirements;
	float						MetalCost;
	float						EnergyCost;
	float						MetalIncome;
	float						EnergyIncome;

	list<unit_Job>::iterator	ID;

}; // End struct - unit_Job
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_JOB_H_)
