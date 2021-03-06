#ifndef Ntuple_Ntuple_H
#define Ntuple_Ntuple_H

#include <EventLoop/StatusCode.h>
#include <EventLoop/Algorithm.h>

#include "xAODAnaHelpers/Algorithm.h"

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include "TTree.h"
#include "TH1D.h"

#include <xAODTruth/TruthParticleContainer.h>

class HelpTreeBase;

class Ntuple : public xAH::Algorithm
{

public:

  std::string m_name;                  
  TH1D* m_cutflowHist;    //!
  TH1D* m_cutflowHistW;   //!
  int   m_cutflowFirst;   //!
  int   m_iCutflow;       //!
  bool  m_isMC;           //!

  xAOD::TEvent *m_event;               //!
  xAOD::TStore *m_store;               //!

  std::string m_container_muons;
  std::string m_container_electrons;
  std::string m_container_jets;

  std::string m_evtDetailStr;
  std::string m_trigDetailStr;
  std::string m_inTruthParticleName;

  bool m_debug;
  std::string m_eventCuts;
  std::string m_syst;

  std::map< std::string, HelpTreeBase* > m_helpTree; //!

  // muons
  int                b_mu_n;
  std::vector<float> b_mu_pt;
  std::vector<float> b_mu_eta;
  std::vector<float> b_mu_phi;
  std::vector<float> b_mu_m;
  std::vector<float> b_mu_d0;
  std::vector<float> b_mu_d0sig;
  std::vector<float> b_mu_z0;
  std::vector<float> b_mu_z0sintheta;
  std::vector<int>   b_mu_charge;
  std::vector<int>   b_mu_author;
  std::vector<int>   b_mu_muonType;
  std::vector<float> b_mu_ptcone20;
  std::vector<float> b_mu_ptcone30;
  std::vector<float> b_mu_ptcone40;
  std::vector<float> b_mu_ptvarcone20;
  std::vector<float> b_mu_ptvarcone30;
  std::vector<float> b_mu_ptvarcone40;
  std::vector<float> b_mu_topoetcone20;
  std::vector<float> b_mu_topoetcone30;
  std::vector<float> b_mu_topoetcone40;
  std::vector<int>   b_mu_IsoLooseTrackOnly;
  std::vector<int>   b_mu_IsoLoose;
  std::vector<int>   b_mu_IsoTight;
  std::vector<int>   b_mu_IsoGradient;
  std::vector<int>   b_mu_IsoGradientLoose;

  // electrons
  int                b_el_n;
  std::vector<float> b_el_pt;
  std::vector<float> b_el_eta;
  std::vector<float> b_el_phi;
  std::vector<float> b_el_m;
  std::vector<int>   b_el_charge;
  std::vector<float> b_el_d0;
  std::vector<float> b_el_d0sig;
  std::vector<float> b_el_z0;
  std::vector<float> b_el_z0sintheta;
  std::vector<int>   b_el_LHVeryLoose;
  std::vector<int>   b_el_LHLoose;
  std::vector<int>   b_el_LHMedium;
  std::vector<int>   b_el_LHTight;
  std::vector<float> b_el_ptcone20;
  std::vector<float> b_el_ptcone30;
  std::vector<float> b_el_ptcone40;
  std::vector<float> b_el_ptvarcone20;
  std::vector<float> b_el_ptvarcone30;
  std::vector<float> b_el_ptvarcone40;
  std::vector<float> b_el_topoetcone20;
  std::vector<float> b_el_topoetcone30;
  std::vector<float> b_el_topoetcone40;
  std::vector<int>   b_el_IsoLooseTrackOnly;
  std::vector<int>   b_el_IsoLoose;
  std::vector<int>   b_el_IsoTight;
  std::vector<int>   b_el_IsoGradient;
  std::vector<int>   b_el_IsoGradientLoose;

  // jets
  int                b_jet_ak4em_n;
  std::vector<float> b_jet_ak4em_pt;
  std::vector<float> b_jet_ak4em_eta;
  std::vector<float> b_jet_ak4em_phi;
  std::vector<float> b_jet_ak4em_m;
  std::vector<float> b_jet_ak4em_mv2c00;
  std::vector<float> b_jet_ak4em_mv2c10;
  std::vector<float> b_jet_ak4em_mv2c20;

  // met
  float b_met_et;
  float b_met_phi;
  float b_met_sumet;

  // mc
  float m_mcEventWeight; 
  float m_weight;
  float m_weight_xs;

  void passCut();

public:

  Ntuple();                                                 //!

  virtual EL::StatusCode setupJob (EL::Job& job);           //!
  virtual EL::StatusCode fileExecute ();                    //!
  virtual EL::StatusCode histInitialize ();                 //!
  virtual EL::StatusCode changeInput (bool firstFile);      //!
  virtual EL::StatusCode initialize ();                     //!
  virtual EL::StatusCode execute ();                        //!
  virtual EL::StatusCode postExecute ();                    //!
  virtual EL::StatusCode finalize ();                       //!
  virtual EL::StatusCode histFinalize ();                   //!

  virtual EL::StatusCode AddTree(std::string);  //!
  virtual EL::StatusCode ClearBranches();       //!

  EL::StatusCode executeSingle(std::string syst="", bool countEvents = false);

  // this is black magic
  ClassDef(Ntuple, 1); //!

};

#endif
