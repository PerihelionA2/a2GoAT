#ifndef __CINT__

#include "PPhysics.h"

PPhysics::PPhysics() 
{ 
}

PPhysics::~PPhysics()
{
}

Bool_t	PPhysics::Init(Char_t* configfile)
{
	
	return kTRUE;
}

void	PPhysics::Reconstruct()
{
}

void PPhysics::MissingMassPDG(Int_t pdg, TH1* Hprompt, TH1* Hrandom)
{
	for (Int_t i = 0; i < GoATTree_GetNParticles(); i++)
	{
		if (GoATTree_GetPDG(i) != pdg) continue; 

		particle = GetGoATVector(i);
		
		for (Int_t j = 0; j < GetNTagged(); j++)
		{
			FillMissingMassPair(i, j, Hprompt, Hrandom);
		}
	}
}	

Bool_t PPhysics::FillMissingMass(Int_t particle_index, TH1* Hprompt, TH1* Hrandom)
{
	for (Int_t i = 0; i < GetNTagged(); i++)
	{
		if(GetPhotonBeam_E(i) < 275) continue;
		if(GetPhotonBeam_E(i) > 300) continue;	
	
		FillMissingMassPair(particle_index, i, Hprompt, Hrandom);
	}
	return kTRUE;	
}
Bool_t	PPhysics::FillTheta(Int_t particle_index, TH1* Tprompt, TH1* Trandom, TH1* Eprompt, TH1* Erandom)
{
	for (Int_t i = 0; i < GetNTagged(); i++)
	{	
		if (GetnErr() != 0) continue;
				
		FillEnergy(particle_index, i, Eprompt, Erandom);		
	
		if(GetPhotonBeam_E(i) < 275) continue;
		if(GetPhotonBeam_E(i) > 300) continue;
	
		FillThetaPair(particle_index, i, Tprompt, Trandom);
	}
	return kTRUE;	
}
Bool_t PPhysics::FillMissingMassPair(Int_t particle_index, Int_t tagger_index, TH1* Hprompt, TH1* Hrandom)
{
	time = GetTagged_t(tagger_index) - GoATTree_GetTime(particle_index);

	Prompt = IsPrompt(time);
	Random = IsRandom(time);
	
	if ((!Prompt) && (!Random)) return kFALSE;

	particle	= GetGoATVector(particle_index);			
	beam 		= TLorentzVector(0.,0.,GetPhotonBeam_E(tagger_index),GetPhotonBeam_E(tagger_index));
	missingp4 	= beam + target - particle;

	if (Prompt) Hprompt->Fill(missingp4.M());
	if (Random) Hrandom->Fill(missingp4.M());
		

	return kTRUE;
}
Bool_t PPhysics::FillThetaPair(Int_t particle_index, Int_t tagger_index,TH1* Tprompt, TH1* Trandom)
{
	time = GetTagged_t(tagger_index) - GoATTree_GetTime(particle_index);
	
	Prompt = IsPrompt(time);
	Random = IsRandom(time);
	
	if ((!Prompt) && (!Random)) return kFALSE;

	if (Prompt) Tprompt -> Fill(GoATTree_GetTheta(particle_index));
	if (Random) Trandom -> Fill(GoATTree_GetTheta(particle_index));

	return kTRUE;
}
Bool_t PPhysics::FillEnergy(Int_t particle_index, Int_t tagger_index, TH1* Eprompt, TH1* Erandom)
{
	if(GoATTree_GetTheta(particle_index) < 87) return kFALSE;
	if(GoATTree_GetTheta(particle_index) > 93) return kFALSE;

	time = GetTagged_t(tagger_index) - GoATTree_GetTime(particle_index);

	Prompt = IsPrompt(time);
	Random = IsRandom(time);
	
	if ((!Prompt) && (!Random)) return kFALSE;

	if (Prompt) Eprompt->Fill(GetPhotonBeam_E(tagger_index));
	if (Random) Erandom->Fill(GetPhotonBeam_E(tagger_index));
		

	return kTRUE;
}
Double_t PPhysics::CalcMissingMass(Int_t particle_index, Int_t tagger_index)
{
	missingp4 	= CalcMissingP4(particle_index, tagger_index);			

	return missingp4.M();
}
Double_t PPhysics::CalcMissingEnergy(Int_t particle_index, Int_t tagger_index)
{
	missingp4 	= CalcMissingP4(particle_index, tagger_index);						

	return missingp4.T();
}
TLorentzVector PPhysics::CalcMissingP4(Int_t particle_index, Int_t tagger_index)
{
	particle	= GetGoATVector(particle_index);			
	beam 		= TLorentzVector(0.,0.,GetPhotonBeam_E(tagger_index),GetPhotonBeam_E(tagger_index));
	missingp4 	= beam + target - particle;						

	return missingp4;
}

void PPhysics::FillTimePDG(Int_t pdg, TH1* Htime)
{
	for (Int_t i = 0; i < GoATTree_GetNParticles(); i++)
	{
		if (GoATTree_GetPDG(i) != pdg) continue; 
		
		for (Int_t j = 0; j < GetNTagged(); j++)
		{
			time = GetTagged_t(j) - GoATTree_GetTime(i);
			Htime->Fill(time);
		}
	}
}

Bool_t PPhysics::IsPrompt(Double_t time, Double_t t_low, Double_t t_high)
{
	if ((time >= t_low) && (time <= t_high)) return kTRUE;
	
	return kFALSE;
}
	
Bool_t PPhysics::IsRandom(Double_t time, Double_t t_low1, Double_t t_high1, 
										 Double_t t_low2, Double_t t_high2 )
{

	if ((time >= t_low1) && (time <= t_high1)) return kTRUE;
	if ((time >= t_low2) && (time <= t_high2)) return kTRUE;
	
	return kFALSE;
}

Bool_t 	PPhysics::Write()
{
	return kTRUE;
}

void	PPhysics::RandomSubtraction(TH1* prompt, TH1* random, TH1* sub, Double_t ratio)
{
	sub->Add(prompt,1);
	sub->Add(random,-ratio);

}

void	PPhysics::RandomSubtraction(TH3* prompt, TH3* random, TH3* sub, Double_t ratio)
{
	sub->Add(prompt,1);
	sub->Add(random,-ratio);

}

void	PPhysics::ShowTimeCuts(TH1* timeH, TH1* cutsH, Double_t t1, Double_t t2, 
							 Double_t t3, Double_t t4, Double_t t5, Double_t t6)
{
	Double_t t;

	for (Int_t i = timeH->GetXaxis()->FindBin(t1); i < timeH->GetXaxis()->FindBin(t2); i++)
	{	
		t = timeH->GetBinContent(i);
		cutsH->SetBinContent(i,t);
	}	

	for (Int_t i = timeH->GetXaxis()->FindBin(t3); i < timeH->GetXaxis()->FindBin(t4); i++)
	{	
		t = timeH->GetBinContent(i);
		cutsH->SetBinContent(i,t);
	}	
	
	for (Int_t i = timeH->GetXaxis()->FindBin(t5); i < timeH->GetXaxis()->FindBin(t6); i++)
	{	
		t = timeH->GetBinContent(i);
		cutsH->SetBinContent(i,t);
	}	
	
}

Bool_t	PPhysics::OpenHistFile(const char* pfile, Option_t* option)
{
	HistFile = new TFile(pfile, option);
	if(!HistFile) return kFALSE;
    if(HistFile->IsZombie()) return kFALSE;
    
	cout << "HistFile " << pfile << " opened (option = "<< option << ")." << endl;
	
	return kTRUE;
}

Bool_t 	PPhysics::CloseHistFile()
{
	if(!HistFile) return kFALSE;
	HistFile->Close();
	
	return kTRUE;
}

#endif
