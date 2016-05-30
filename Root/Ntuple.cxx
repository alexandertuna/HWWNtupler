#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <xAODJet/JetContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>
#include <SampleHandler/MetaFields.h>

#include "xAODParticleEvent/ParticleContainer.h"
#include <xAODTruth/TruthVertex.h>

#include <xAODAnaHelpers/HelpTreeBase.h>
#include <HWWNtupler/Ntuple.h>

#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>
#include <xAODAnaHelpers/tools/ReturnCheckConfig.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(Ntuple)

using std::cout;  
using std::endl;
using std::string; 
using std::vector;

Ntuple::Ntuple () :
    m_name(""),
    m_cutflowHist(0),
    m_cutflowHistW(0),
    m_cutflowFirst(0),
    m_iCutflow(0),
    m_isMC(false),
    
    m_container_muons(""),
    m_container_electrons(""),
    m_container_jets(""),
    
    m_evtDetailStr(""),
    m_trigDetailStr(""),
    
    m_inTruthParticleName(""),
    
    m_debug(false), 
    m_eventCuts(""),
    
    m_syst(""),
    m_weight(1.0),
    m_weight_xs(1.0)
{
    // needed if you want to retrieve this algo with wk()->getAlg(ALG_NAME) downstream
    this->SetName("Ntuple");
}

EL::StatusCode Ntuple::setupJob (EL::Job& job)
{
  Info("setupJob()", "Calling setupJob \n");
  job.useXAOD();
  xAOD::Init("Ntuple").ignore();

  EL::OutputStream outForTree(m_name.c_str());
  job.outputAdd(outForTree);
  Info("setupJob()", "Ready\n");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::initialize ()
{
  Info("initialize()", m_name.c_str());
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // isMC?
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("ResonanceAlgorithm::initialize()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store, m_debug), "");
  m_isMC = (eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION)) ? true : false;

  // cutflow
  TFile *file = wk()->getOutputFile("cutflow");
  m_cutflowHist  = (TH1D*)file->Get("cutflow");
  m_cutflowHistW = (TH1D*)file->Get("cutflow_weighted");

  m_cutflowFirst = m_cutflowHist->GetXaxis()->FindBin("XhhMiniTreeAll");
  m_cutflowHistW->GetXaxis()->FindBin("XhhMiniTreeAll");

  m_cutflowHist ->GetXaxis()->FindBin("XhhMiniTreePreSel");
  m_cutflowHistW->GetXaxis()->FindBin("XhhMiniTreePreSel");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::AddTree(string syst = "")
{
  Info("AddTree()", "%s", m_name.c_str());

  string treeName = "Ntuple";
  if (!syst.empty()) treeName += syst;

  TTree* tree = new TTree(treeName.c_str(), treeName.c_str());

  if(!tree) {
    Error("AddTree()", "Failed to instantiate output tree!");
    return EL::StatusCode::FAILURE;
  }

  TFile* treeFile = wk()->getOutputFile(m_name.c_str());
  tree->SetDirectory(treeFile);

  m_helpTree[syst] = new HelpTreeBase(m_event, tree, treeFile);

  m_helpTree[syst]->AddEvent(m_evtDetailStr);
  m_helpTree[syst]->AddTrigger(m_trigDetailStr);

  tree->Branch("mu_n",        &b_mu_n);
  tree->Branch("mu_pt",       &b_mu_pt);
  tree->Branch("mu_eta",      &b_mu_eta);
  tree->Branch("mu_phi",      &b_mu_phi);
  tree->Branch("mu_m",        &b_mu_m);
  tree->Branch("mu_ptcone20", &b_mu_ptcone20);
  tree->Branch("mu_ptcone30", &b_mu_ptcone30);
  tree->Branch("mu_ptcone40", &b_mu_ptcone40);

  tree->Branch("el_n",        &b_el_n);
  tree->Branch("el_pt",       &b_el_pt);
  tree->Branch("el_eta",      &b_el_eta);
  tree->Branch("el_phi",      &b_el_phi);
  tree->Branch("el_m",        &b_el_m);
  tree->Branch("el_ptcone20", &b_el_ptcone20);
  tree->Branch("el_ptcone30", &b_el_ptcone30);
  tree->Branch("el_ptcone40", &b_el_ptcone40);

  tree->Branch("weight",    &m_weight,    "weight/F");
  tree->Branch("weight_xs", &m_weight_xs, "weight_xs/F");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Ntuple::fileExecute    () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Ntuple::postExecute    () { return EL::StatusCode::SUCCESS; }
EL::StatusCode Ntuple::changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode Ntuple::ClearBranches()
{

    b_mu_n = 0;
    b_mu_pt       .clear();
    b_mu_eta      .clear();
    b_mu_phi      .clear();
    b_mu_m        .clear();
    b_mu_ptcone20 .clear();
    b_mu_ptcone30 .clear();
    b_mu_ptcone40 .clear();

    b_el_n = 0;
    b_el_pt       .clear();
    b_el_eta      .clear();
    b_el_phi      .clear();
    b_el_m        .clear();
    b_el_ptcone20 .clear();
    b_el_ptcone30 .clear();
    b_el_ptcone40 .clear();

    b_jet_ak4em_n = 0;
    b_jet_ak4em_pt     .clear();
    b_jet_ak4em_eta    .clear();
    b_jet_ak4em_phi    .clear();
    b_jet_ak4em_m      .clear();
    b_jet_ak4em_mv2c00 .clear();
    b_jet_ak4em_mv2c10 .clear();
    b_jet_ak4em_mv2c20 .clear();

    float b_met_et    = 0;
    float b_met_phi   = 0;
    float b_met_sumet = 0;

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::execute ()
{
    const xAOD::EventInfo* eventInfo(0);
    RETURN_CHECK("Ntuple::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store), "");
    
    m_iCutflow      = m_cutflowFirst;
    m_mcEventWeight = (m_isMC) ? eventInfo->mcEventWeight() : 1.0;
    
    executeSingle("", true);
    
    if(!m_syst.empty()){
        
        vector<string>* syst_names(nullptr);
        RETURN_CHECK("Ntuple::execute()::syst_names", HelperFunctions::retrieve(syst_names, m_syst, 0, m_store, m_debug) ,"");
        
        // Loop over resolved systematics 
        for (string& syst_name : *syst_names) {
            
            if(syst_name.empty()) continue;
            
            if(m_debug) Info("execute",  "systName %s", syst_name.c_str());
            
            executeSingle(syst_name, false);
        }
    }
    return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::executeSingle(string syst, bool countEvents) {

  ClearBranches();

  if(countEvents) passCut(); 
  
  if(m_helpTree.find(syst) == m_helpTree.end())
      AddTree(syst);

  const xAOD::EventInfo*       eventInfo(0);
  const xAOD::MuonContainer*       muons(0);
  const xAOD::VertexContainer*  vertices(0);
  const xAOD::Vertex*               pv = 0;

  RETURN_CHECK("Ntuple::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo",       m_event, m_store), "");
  RETURN_CHECK("Ntuple::execute()", HelperFunctions::retrieve(muons,     "Muons",           m_event, m_store), "");
  RETURN_CHECK("Ntuple::execute()", HelperFunctions::retrieve(vertices,  "PrimaryVertices", m_event, m_store), "");
  pv = vertices->at(HelperFunctions::getPrimaryVertexLocation(vertices));

  //  only fill ntup if:
  //    - blah

  // bool PassMuonsGE0 = (muons->size() >= 0);
  // if (!PassMuonsGE0)
  //     return EL::StatusCode::SUCCESS;

  if(countEvents) passCut(); 

  // event info
  if(m_debug) cout << " Filling event " << endl;  
  m_helpTree[syst]->FillEvent( eventInfo );

  double xs      = wk()->metaData()->castDouble(SH::MetaFields::crossSection    ,1); 
  double filtEff = wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency,1); 

  m_weight_xs = xs * filtEff;
  m_weight    = m_mcEventWeight * xs * filtEff;

  // trigger info
  if(m_debug) cout << " Filling trigger " << endl;  
  m_helpTree[syst]->FillTrigger( eventInfo );

  // muons
  for (auto muon: *muons){
      b_mu_n++;
      b_mu_pt       .push_back(muon->pt());
      b_mu_eta      .push_back(muon->eta());
      b_mu_phi      .push_back(muon->phi());
      b_mu_m        .push_back(muon->m());
      b_mu_ptcone20 .push_back(muon->isolation(xAOD::Iso::ptcone20));
      b_mu_ptcone30 .push_back(muon->isolation(xAOD::Iso::ptcone30));
      b_mu_ptcone40 .push_back(muon->isolation(xAOD::Iso::ptcone40));
  }

  // electrons
  // jets
  // met

  // fill the tree
  m_helpTree[syst]->Fill();
  return EL::StatusCode::SUCCESS;

}

EL::StatusCode Ntuple::finalize () {

  Info("finalize()", m_name.c_str());

  if (!m_helpTree.empty()){
    for(auto tree : m_helpTree) {
      if (tree.second) delete tree.second;
    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Ntuple::histFinalize ()
{
  TFile * treeFile = wk()->getOutputFile(m_name.c_str());
  
  // cutflow
  TH1F* thisCutflowHist = (TH1F*) m_cutflowHist->Clone();
  std::string thisName = thisCutflowHist->GetName();
  thisCutflowHist->SetName( (thisName+"_Ntuple").c_str() );
  thisCutflowHist->SetDirectory( treeFile );
  
  TH1F* thisCutflowHistW = (TH1F*) m_cutflowHistW->Clone();
  thisName = thisCutflowHistW->GetName();
  thisCutflowHistW->SetName( (thisName+"_Ntuple").c_str() );
  thisCutflowHistW->SetDirectory( treeFile );
  
  // metadata
  TFile *fileMD = wk()->getOutputFile("metadata");
  TH1D* m_histEventCount   = (TH1D*)fileMD->Get("MetaData_EventCount");
  TH1F* thisHistEventCount = (TH1F*) m_histEventCount->Clone();
  thisName = thisHistEventCount->GetName();
  thisHistEventCount->SetName( (thisName+"_Ntuple").c_str() );
  thisHistEventCount->SetDirectory( treeFile );

  return EL::StatusCode::SUCCESS;
}

// fill cutflow and increment counter
void Ntuple::passCut(){
    m_cutflowHist ->Fill(m_iCutflow, 1);
    m_cutflowHistW->Fill(m_iCutflow, m_mcEventWeight);
    m_iCutflow++;
}

