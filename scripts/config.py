import ROOT
from xAH_config import xAH_config
import sys, os

sys.path.insert(0, os.environ['ROOTCOREBIN']+"/user_scripts/HWWNtupler/")

c = xAH_config()

triggers = ['HLT_e24_lhtight_iloose',
            'HLT_mu26_imedium',
            ]
triggers = ",".join(triggers)

systName = "Nominal"
systVal  = 0

# basic setup
c.setalg("BasicEventSelection", { "m_name"                  : "basicEventSel",
                                  "m_applyGRLCut"           : False,
                                  "m_doPUreweighting"       : False, 
                                  "m_vertexContainerName"   : "PrimaryVertices",
                                  "m_PVNTrack"              : 2,
                                  "m_truthLevelOnly"        : False,
                                  "m_applyPrimaryVertexCut" : True,
                                  "m_derivationName"        : "HIGG3D1",
                                  "m_applyEventCleaningCut" : True,
                                  "m_applyCoreFlagsCut"     : True,
                                  "m_triggerSelection"      : triggers, 
                                  "m_storeTrigDecisions"    : True,
                                  #"m_useMetaData"           : True,
                                  "m_useMetaData"           : False,
                                  "m_applyTriggerCut"       : False,
                                  "m_storePassL1"           : True,
                                  "m_storePassHLT"          : True,
                                  "m_storeTrigKeys"         : True,
                                  } )

# jet calibrators and selectors
c.setalg("JetCalibrator", { "m_name"                   : "AntiKt4TopoEM", 
                            "m_systName"               : "", 
                            "m_systVal"                : 1,
                            "m_inContainerName"        : "AntiKt4EMTopoJets",
                            "m_outContainerName"       : "AntiKt4EMTopoJets_Calib", 
                            "m_sort"                   : True,
                            "m_jetAlgo"                : "AntiKt4EMTopo",
                            "m_outputAlgo"             : "AntiKt4EMTopoJets_Calib_Algo",
                            "m_calibSequence"          : "JetArea_Residual_Origin_EtaJES_GSC",
                            "m_calibConfigFullSim"     : "JES_2015dataset_recommendation_Feb2016.config",
                            "m_calibConfigData"        : "JES_2015dataset_recommendation_Feb2016.config",
                            "m_calibConfigAFII"        : "JES_MC15Prerecommendation_AFII_June2015.config",
                            "m_jetCleanCutLevel"       : "LooseBad",
                            "m_JESUncertConfig"        : "$ROOTCOREBIN/data/JetUncertainties/JES_2015/Moriond2016/JES2015_AllNuisanceParameters.config",
                            "m_JESUncertMCType"        : "MC15",
                            "m_saveAllCleanDecisions"  : True,                         
                            "m_setAFII"                : False,
                            "m_JERUncertConfig"        : "JetResolution/Prerec2015_xCalib_2012JER_ReducedTo9NP_Plots_v2.root",
                            "m_JERApplyNominal"        : False,
                            "m_redoJVT"                : True,
                            "m_systName"               : systName,
                            "m_systVal"                : systVal,
                            } )

c.setalg("JetSelector", { "m_name"                    :  "preSelJetsEMTopoJets",
                          "m_inContainerName"         :  "AntiKt4EMTopoJets_Calib",
                          "m_inputAlgo"               :  "AntiKt4EMTopoJets_Calib_Algo",
                          "m_outContainerName"        :  "AntiKt4EMTopoJets_Calib_Presel",
                          "m_outputAlgo"              :  "AntiKt4EMTopoJets_Calib_Presel_Algo",
                          "m_decorateSelectedObjects" :  False, 
                          "m_createSelectedContainer" :  True, 
                          "m_cleanJets"               :  False, 
                          "m_pT_min"                  :  25e3,
                          "m_eta_max"                 :  5.0,
                          "m_useCutFlow"              :  True,
                          "m_doJVT"                   :  True,
                          "m_jetScaleType"            :  "JetConstitScaleMomentum",
                          } )

# c.setalg("BJetEfficiencyCorrector", { "m_name"                    : "BJetEffCor_AntiKt4EMTopoJets",
#                                       "m_inContainerName"         : "AntiKt4EMTopoJets_Calib_Presel",
#                                       "m_inputAlgo"               : "AntiKt4EMTopoJets_Calib_Presel_Algo",
#                                       "m_debug"                   : False, 
#                                       # a bit special for b-tagging SF, since "Nominal" will NOT be converted to "" internally
#                                       "m_systName"                : ("" if systName == "Nominal" else systName),   
#                                       "m_systVal"                 : systVal,
#                                       "m_outputSystName"          : "AntiKt4EM_FTSys",
#                                       "m_operatingPt"             : "FixedCutBEff_70",
#                                       "m_operatingPtCDI"          : "FixedCutBEff_70",
#                                       "m_corrFileName"            : "$ROOTCOREBIN/data/XhhCommon/2016-Winter-13TeV-MC15-CDI-March10_v1.root",
#                                       "m_jetAuthor"               : "AntiKt4EMTopoJets",
#                                       "m_taggerName"              : "MV2c20",
#                                       "m_decor"                   : "BTag",
#                                       "m_debug"                   : False,
#                                       } )

# muon calibrators and selectors
c.setalg("MuonCalibrator", { "m_name"                : "MuonCalibrator", 
                             "m_inContainerName"     : "Muons", 
                             "m_outContainerName"    : "Muons_Calib", 
                             "m_outputAlgoSystNames" : "MuonCalibrator_Syst",
                             "m_debug"               : False,
                             } )

c.setalg("MuonSelector", { "m_name"                    : "MuonSelector", 
                           "m_inContainerName"         : "Muons_Calib", 
                           "m_outContainerName"        : "Muons_Selected", 
                           "m_createSelectedContainer" : True,
                           "m_pT_min"                  : 8*1000,
                           "m_eta_max"                 : 2.5,
                           "m_muonType"                : "Combined",
                           "m_muonQualityStr"          : "Medium",
                           } )

# electron calibrators and selectors
c.setalg("ElectronCalibrator", { "m_name"                : "ElectronCalibrator", 
                                 "m_inContainerName"     : "Electrons", 
                                 "m_outContainerName"    : "Electrons_Calib", 
                                 "m_outputAlgoSystNames" : "ElectronCalibrator_Syst",
                                 "m_esModel"             : "es2015PRE",
                                 "m_decorrelationModel"  : "FULL_ETACORRELATED_v1",
                                 "m_debug"               : False,
                                 } )

c.setalg("ElectronSelector", { "m_name"                    : "ElectronSelector",
                               "m_inContainerName"         : "Electrons_Calib",
                               "m_outContainerName"        : "Electrons_Selected",
                               "m_createSelectedContainer" : True,
                               "m_pT_min"                  : 8*1000,
                               "m_eta_max"                 : 2.5,
                               "m_LHOperatingPoint"        : "VeryLoose",
                               } )

# MET builder
# c.setalg("METConstructor", {"m_name"                  : "METBuilding",
#                             "m_mapName"               : "METAssoc_AntiKt4LCTopo",
#                             "m_coreName"              : "MET_Core_AntiKt4LCTopo",
#                             "m_outputContainer"       : "RefFinalLC",
#                             "m_inputJets"             : "AntiKt4LCTopoJets_Calib",
#                             "m_inputMuons"            : "Muons_Selected", 
#                             "m_doMuonCuts"            : False,
#                             "m_doMuonEloss"           : True,
#                             "m_doIsolMuonEloss"       : True
#                             } )

# ntuples
c.setalg("Ntuple", { "m_name"                       : "Ntupler",
                     "m_debug"                      : False,
                     "m_trigDetailStr"              : "passTriggers",
                     "m_evtDetailStr"               : "pileup",
                     "m_inTruthParticleName"        : "TruthParticles",
                     "m_container_muons"            : "Muons_Selected", 
                     "m_container_electrons"        : "Electrons_Selected", 
                     "m_container_jets"             : "AntiKt4EMTopoJets_Calib_Presel", 
                     } )



