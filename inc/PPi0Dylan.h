#ifndef __PPi0Dylan_h__
#define __PPi0Dylan_h__

#include <iostream>
#include <fstream>
using namespace std;
#include <cstdio>
#include <string> 

#include "PPhysics.h"

class	PPi0Dylan : public PPhysics
{
private:

	Int_t	bin_val;
	Int_t	bin_val_e;
	Double_t yeild_0;
	Double_t yeild_0_e;
	Double_t yeild_1;
	Double_t yeild_1_e;
	Double_t asym;
	Double_t asym_e;
	Double_t err;
	Double_t err_e;
	
	Double_t time;
	TH1* 	time_pi0;
	TH1* 	time_pi0_cuts;	

	TH1* 	MM_prompt_pi0;
	TH1* 	MM_random_pi0;
	TH1* 	MM_pi0;

	TH1*	Asymmetry_Theta;
	TH1*	Asymmetry_Energy;
	TH1*	Energy;	

	TH1* 	Theta_1;
	TH1*	Theta_0;
	TH1*	Theta_1_prompt;
	TH1*	Theta_1_random;
	TH1*	Theta_0_prompt;
	TH1*	Theta_0_random; 
	
	TH1*	Eng_1;
	TH1*	Eng_0;
	TH1*	Eng_1_prompt;
	TH1*	Eng_1_random;
	TH1*	Eng_0_prompt;
	TH1*	Eng_0_random;


	TH1* 	MM_prompt_pi0_n_2g_h0;
	TH1* 	MM_random_pi0_n_2g_h0;
	TH1* 	MM_pi0_n_2g_h0;	

	TH1*	MM_prompt_pi0_n_2g_h1;
	TH1*	MM_random_pi0_n_2g_h1;
	TH1*	MM_pi0_n_2g_h1;		

	Int_t 	N_pi0;

		
protected:


public:
    PPi0Dylan();
    virtual ~PPi0Dylan();

    virtual Bool_t	Init(const char* configfile);	
    virtual Bool_t	File(const char* file_in, const char* file_out);    
    virtual void 	Analyse();
	virtual void	Reconstruct();
	virtual void 	AsymmetryEnergy();
	virtual	void 	AsymmetryTheta();
	void	PostReconstruction();

	void	DefineHistograms();
	Bool_t	WriteHistograms(TFile* pfile);
	Bool_t	WriteHistograms() {return WriteHistograms(HistFile);}
};
#endif
