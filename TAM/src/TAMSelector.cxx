//
// $Id: TAMSelector.cxx 4001 2007-05-17 22:14:23Z loizides $
//

#include "TAMSelector.h"


#ifndef ROOT_RVersion
#include <RVersion.h>
#endif
#ifndef ROOT_TFile
#include "TFile.h"
#endif
#ifndef ROOT_TTree
#include "TTree.h"
#endif
#ifndef ROOT_TError
#include "TError.h"
#endif
#ifndef ROOT_TROOT
#include "TROOT.h"
#endif
#ifndef ROOT_TList
#include "TList.h"
#endif
#ifndef ROOT_TClass
#include "TClass.h"
#endif
#ifndef ROOT_TProcessID
#include "TProcessID.h"
#endif
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,0,0) && \
    ROOT_VERSION_CODE <  ROOT_VERSION(5,11,2) 
#ifndef ROOT_TUrl
#include "TUrl.h"
#endif
#endif
#ifndef TAM_TAModule
#include "TAModule.h"
#endif
#ifndef TAM_TAMOutput
#include "TAMOutput.h"
#endif
#ifndef TAM_TAMVirtualLoader
#include "TAMVirtualLoader.h"
#endif
#ifndef TAM_TAMVirtualBranchLoader
#include "TAMVirtualBranchLoader.h"
#endif
#ifndef TAM_TAMTreeLoader
#include "TAMTreeLoader.h"
#endif


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TAMSelector                                                          //
//                                                                      //
// A selector class for modular processing of a tree (or chain).        //
// Processing is done by TAModule(s).                                   //
//                                                                      //
// Each TAModule contains pointers to the objects that will be read     //
// in from the tree. For each such pointer, it will call                //
// ReqBranch(name, pointer) to request that the branch be available for //
// processing by the module. The module's pointer would then be         //
// automatically set to point to the object for the current event by    //
// the TAMSelector, whenever the module calls LoadBranch.               //
//                                                                      //
// TAMSelector provides (and hides) all interaction with the tree       //
// for the TAModule(s).                                                 //
//                                                                      //
// Author : Corey Reed           07/20/2004                             //
//          Maarten Ballintijn   12/06/2005                             //
//          Constantin Loizides  12/06/2005                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

ClassImp(TAMSelector)


#if ROOT_VERSION_CODE < ROOT_VERSION(5,11,3)
#define R__ASSERT(e) \
   if (!(e)) Fatal("", kAssertMsg, _QUOTE_(e), __LINE__, __FILE__)
#endif


//______________________________________________________________________________
TAMSelector::TAMSelector() :
   fTree(0),
   fBranchTable(),
   fEventObjs(),
   fAModules(new TAModule("TAMTopModule",
                          "Top-most module containing all other modules.")),
   fCurEvt(-1),
   fOwnInput(0),
   fAnalysisAborted(kFALSE),
   fModAborted(kFALSE),
   fEventAborted(kFALSE),
   fActNotify(kFALSE),
   fObjCounter(0),
   fVerbosity(0),
   fLoaders()
{
   // Default constructor.

   fBranchTable.SetOwner(kTRUE);
   fEventObjs.SetOwner(kTRUE);
   gROOT->GetListOfBrowsables()->Add(fAModules,"Analysis Modules");
   fLoaders.SetName("TAM_LOADERS");
}


//______________________________________________________________________________
TAMSelector::~TAMSelector() 
{
   // Destructor.

   gROOT->GetListOfBrowsables()->Remove(fAModules);
   TList *submods = fAModules->GetListOfTasks();
   if (submods!=0) submods->Clear("nodelete");
   delete fAModules;
   delete fOwnInput;
}


//______________________________________________________________________________
void TAMSelector::AbortAnalysis() 
{
   // Aborts the analysis by aborting all modules and never executing
   // further module funtions.
   
   // no way known to propagate the stop of analysis in PROOF from 
   // one slave to the other slaves (and master).
   Fatal("AbortAnalysis", "Analysis aborted!");

   fAModules->DeactivateAll();
   fAnalysisAborted = kTRUE;
}


//______________________________________________________________________________
void TAMSelector::AbortEvent() 
{
   // Aborts the current event by setting all modules inactive.
   
   fAModules->DeactivateAll();
   fEventAborted = kTRUE;
}


//______________________________________________________________________________
void TAMSelector::AbortModule(TAModule *mod) 
{
   // Abort the specified module by setting it (and its sub modules) inactive.
   
   mod->DeactivateAll();
   fModAborted = kTRUE;
}


//______________________________________________________________________________
void TAMSelector::AddInput(TAModule *mod) 
{
   // Adds the module to the top-most module in the hierarchy.
   // The "misnomer" on the function name is intentional:
   // This allows users to call gProof->AddInput(myMod) when 
   // using proof and mySelector->AddInput(myMod) when not 
   // using proof.
   
   fAModules->Add(mod);
}

//______________________________________________________________________________
Bool_t TAMSelector::AddObjThisEvt(TObject *obj)
{
   // Add this object to the list of objects stored for this event.
   // See further description below.

   if(obj)
      return AddObjThisEvt(obj,obj->GetName());
   else {
      Error("AddObjThisEvt",
            "Can not add null object to event.");
      return kFALSE;
   }
}


//______________________________________________________________________________
Bool_t TAMSelector::AddObjThisEvt(TObject *obj, const char *name) 
{
   // Add this object to the list of objects stored for this event.
   // NOTE:
   //    - The object must have a unique name.
   //    - The object must be on the heap.
   //    - The object will be owned by the selector and deleted at the
   //      end of the processing of the current event.
   //
   // Returns true iff the object meets the requirements and is added to
   // the list successfully.
   
   if (obj!=0) {
      if (FindObjThisEvt(name)==0) {
         if (obj->IsOnHeap()) {
            fEventObjs.Add(new TAMEvtObj(obj,name));
         } else {
            Error("AddObjThisEvt",
                  "Object [%s] does not appear to be on heap. "
                  "Can not add object.",
                  name);
         }
      } else {
         Error("AddObjThisEvt",
               "Object with name [%s] already added to this event.",
               name);
      }
   } else {
      Error("AddObjThisEvt",
            "Can not add null object to event.");
   }
   return kFALSE;
}


//______________________________________________________________________________
void TAMSelector::AddNewOutputLists() 
{
   // Make a new hierarchy of TAMOutput objects and add it to
   // the output of this selector. Note that this should only be called
   // ONCE (by SlaveBegin())!!
   
   fAModules->NewOutputList(GetOutputList());
}


//______________________________________________________________________________
void TAMSelector::Begin(TTree */*tree*/) 
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   // It checks the tree of modules to be sure each module is set 
   // up to use this selector.
   // Adds the tree of modules to the input list of this selector.
   // Builds the hierarchy of TAMOutput objects in the output list.
   // Then calls Begin() on the TAModule(s). At the end, it adds the 
   // list of loaders to the input list.

   if (fAnalysisAborted) {
      return;
   } else {
      if (fInput==0) {
         fOwnInput = new TList;
         SetInputList(fOwnInput);
      }
      CopyModsFromInput();
      fAModules->SetSelector(this);
      if (fAModules->CheckSelectors(this)) {
         if (fModAborted) {
            fAModules->ResetAllActiveFlags();
            fModAborted = kFALSE;
         }
         fAModules->ExecuteTask(&TAModule::kExecBegin);
         if (fEventObjs.IsEmpty()==kFALSE) fEventObjs.Delete();
      } else {
         Error("Begin",
               "One or more module is not set up to use this selector.");
         R__ASSERT(0);
      }
   }
   
   fInput->AddLast(&fLoaders);
}


//______________________________________________________________________________
void TAMSelector::ClearAllLoaders()
{
   // Go through the list of requested branches and clear the 	 
   // loaders that were used.
  	 
   TIter nextBr(fBranchTable.MakeIterator()); 	 
   while ( TAMBranchInfo *br = dynamic_cast<TAMBranchInfo*>(nextBr()))  {

      if (!br->fIsLoaded) continue;  
      // don't bother with branches that weren't loaded

      TAMVirtualBranchLoader *l = br->GetLoader();
      if (l) {
         l->Clear();
      } else {
         Error("ClearAllLoaders", 	 
               "Could not get loader for [%s]. Unable to " 	 
               "try to clear loader for this branch.", 	 
               br->GetName()); 	 
      } 	 
   } 	 
}


//______________________________________________________________________________
void TAMSelector::CopyModsFromInput() 
{
   // Find all TAModules in the input list and copy them to fAModules.

   R__ASSERT(fInput!=0);

   if (fInput->IsEmpty()) return;

   TObject *obj=fInput->First(), *tobj=0;
   while (obj!=0) {
      tobj = fInput->After(obj);
      if (obj->InheritsFrom(TAModule::Class())) {
         AddInput(dynamic_cast<TAModule*>(obj));
      }
      obj = tobj;
   }
}


//______________________________________________________________________________
Bool_t TAMSelector::FindLoader(TAMBranchInfo *brInfo)
{
   // Find the loader responsible for the given branch info. Return kFALSE
   // if none could be found in the list of loaders.

   const char *bname = brInfo->GetName();
   TIter next(&fLoaders);

   while( TAMVirtualLoader *l = dynamic_cast<TAMVirtualLoader*>(next()) ) {
      TAMVirtualBranchLoader *bl = l->CreateBranchLoader(fTree, brInfo);
      if (bl != 0) {
	 Info("FindLoader","branch '%s' use loader %s",
              bname, bl->IsA()->GetName());
         brInfo->SetLoader(bl);
         return kTRUE;
      }
   }

   Error("FindLoader","No loader found for branch '%s'", bname);
   return kFALSE;
}


//______________________________________________________________________________
TAMOutput *TAMSelector::FindModOutput(const TAModule *mod) 
{
   // Find the TAMOutput object corresponding to the given module.
   
   TAMOutput *obj=0, *tobj=0;
   TIter nextObj(GetOutputList());
   while ( (obj = dynamic_cast<TAMOutput*>(nextObj())) ) {
      tobj = obj->FindModOutput(mod); // search mod and sub mods
      if (tobj!=0) return tobj;
   }
   return 0;
}


//______________________________________________________________________________
TObject *TAMSelector::FindObjThisEvt(const Char_t *name) const 
{
   // Looks for the object with the specified name that was added to 
   // this event. If not found, returns zero.
   
   TAMEvtObj *eo = dynamic_cast<TAMEvtObj*>(fEventObjs.FindObject(name));
   if (eo!=0) {
      return eo->fObj;
   }
   return 0;
}


//______________________________________________________________________________
TObject *TAMSelector::FindPublicObj(const Char_t *name) const 
{
   // Looks for the public object with the specified name. If not found,
   // returns zero.
   // Note: TAModules are not public objects and will not be found by this
   // function.
   
   if (fInput!=0) {
      TIter nextObj(fInput);
      TObject *obj=0;
      TString nm(name);
      while ( (obj = nextObj()) ) {
         if ( (nm.CompareTo(obj->GetName())==0) &&
              (obj->InheritsFrom(TAModule::Class())==kFALSE) ) {
            return obj;
         }
      }
   } else {
      Error("FindPublicObj",
            "Input list is null. No public objects exist.");
   }
   return 0;
}


//______________________________________________________________________________
TFile *TAMSelector::GetCurrentFile() const 
{
   // Returns the current file that the tree is in.
   
   return (fTree) ? (fTree->GetCurrentFile()) : 0;
}


//______________________________________________________________________________
const TAMOutput *TAMSelector::GetModOutput() const
{
   // Return the top-most module output object.

   return fAModules->GetModOutput();
}


//______________________________________________________________________________
TAMOutput *TAMSelector::GetModOutput() 
{
   // Return the top-most module output object.

   return fAModules->GetModOutput();
}


//______________________________________________________________________________
void TAMSelector::Init(TTree *tree) {
   // Set the tree for this selector. The Init() function is called 
   // when the selector needs to initialize a new tree (or chain). 
   // It will be called many times when running with PROOF. The 
   // list of requested branches must been constructed 
   // already.

   if (tree==0) {
      Error("Init", "Specified tree is null.");
      AbortAnalysis();
   } 

   fTree = tree;

   TIter nextBranch(fBranchTable.MakeIterator());
   while ( TAMBranchInfo *brInfo = 
             dynamic_cast<TAMBranchInfo*>(nextBranch()) ) {
      brInfo->Init();
   }
}


//______________________________________________________________________________
void TAMSelector::LoadBranch(const Char_t *bname) 
{
   // Loads the selected branch and get the current entry (number fCurEvt)
   // if the branch has not yet been loaded for this event. It then makes 
   // sure each module's pointer to the branch object point to the address
   // of this branch.
   
   if(fCurEvt==-1) {
      Error("LoadBranch",
            "Can not load branch with name [%s] at this point (fCurEvt==1).",
            bname);
      AbortAnalysis();
   }

   TAMBranchInfo *brInfo = 
      dynamic_cast<TAMBranchInfo*>( fBranchTable.FindObject(bname) );

   if (brInfo==0) {
      Error("LoadBranch",
            "Could not find branch with name [%s] in requested branch list.",
            bname);
      AbortAnalysis();
   } 

   if (brInfo->IsLoaded() ) 
      return;

   // find loader for branch info if needed and notify
   if (brInfo->GetLoader() == 0) {

      if ( !FindLoader(brInfo) ) {
         Error("LoadBranch","Branch [%s] FindLoader() failed in file [%s].",
               brInfo->GetName(),
               (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName()) : "null");

         AbortAnalysis();
      }

      if ( !brInfo->Notify(fTree) ) 
         Error("LoadBranch","Branch [%s] Notify() failed in file [%s].",
               brInfo->GetName(),
               (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName()) : "null");

      // have to reset our object counter to take care of objects
      // created in the notify of the loader
      fObjCounter=TProcessID::GetObjectCount();

   }
                            
   // load the entry
   Int_t ret = brInfo->GetEntry(fCurEvt);
   if(ret<0) {
      Error("LoadBranch",
            "Error in file [%s] when accessing branch with name [%s] in "
            "requested branch list.",
            (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName()) 
                                  : "null", bname);

      AbortAnalysis();
   }
}


//______________________________________________________________________________
Bool_t TAMSelector::Notify() 
{
   // Sets the branch pointers, checks that the types of the module's
   // pointers correspond to what is in the tree and calls SetBranchAddress 
   // if necessary. Finally, it also notifies the modules.
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when a new TTree
   // is started when using PROOF.

   //we are just in Notify(), therefore we ignore this call
   if(fActNotify) return kTRUE;
   fActNotify = kTRUE; //"lock" to protect for recursive calls

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,0,0) && \
    ROOT_VERSION_CODE <  ROOT_VERSION(5,11,2)
 
   // Workaround for older ROOT: give file name to 
   // TFile that accidentally stripped the protocol 

      const TUrl *url = (GetCurrentFile()!=0) 
         ? GetCurrentFile()->GetEndpointUrl() : 0;

      if (url==0) {
	 Warning("Notify","Could not get URL for file [%s].",
		 (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName())
		 : "null");
      } else {

	 if(GetCurrentFile()!=0)
	    GetCurrentFile()->SetName((const_cast<TUrl*>(url)->GetUrl()));
	                               // the const_cast is for some version of 
                                       // 5.08.00 that didn't have GetUrl const.
      }
#endif

   if (fVerbosity>0) {
      Info("Notify","Opening file [%s] at current event [%d].",
           (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName()) : "null",
           fCurEvt);
   }


   Bool_t notifyStat = kTRUE;

   // no event yet processed eg, no loaders assigned,
   // so that the notify is being delayed to LoadBranch()
   if(fCurEvt>=0) {

      TIter nextBranch(fBranchTable.MakeIterator());
      
      while ( TAMBranchInfo *brInfo = 
	      dynamic_cast<TAMBranchInfo*>(nextBranch()) ) {

	 // notify the branchinfo and its corresponding loader
	 notifyStat &= brInfo->Notify(fTree);

         // have to reset our object counter to take care of objects
         // created in the notify of the loader
         fObjCounter=TProcessID::GetObjectCount();;

	 if (notifyStat==kFALSE) {
	    Error("Notify","Branch [%s] Notify() failed in file [%s].",
		  brInfo->GetName(),
		  (GetCurrentFile()!=0) ? (GetCurrentFile()->GetName()) 
                                        : "null");

	    AbortAnalysis();
	 }
      }

   }

   if (notifyStat && (fAnalysisAborted==kFALSE)) {
      notifyStat &= fAModules->NotifyAll();
   }

   //release "lock" on notify
   fActNotify = kFALSE;

   return notifyStat;
}


//______________________________________________________________________________
Bool_t TAMSelector::Process(Long64_t entry) 
{
   // Zero's all TAModule(s)'s branch addresses and calls Process()
   // on the TAModule(s). Then clear any requested clones arrays and
   // clear the list of event-stored objects.
   //
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed.
   
   if (fAnalysisAborted) {
      // There is currently no reliable way to exit cleanly back to the
      // interpreter, so simply return immediately.
      return kFALSE;

   } else {

      // Store object counter
      fObjCounter=TProcessID::GetObjectCount();;

      fCurEvt = entry;
      ZeroAllBranches();
      if (fModAborted) {
         fAModules->ResetAllActiveFlags();
         fModAborted = kFALSE;
      } else if (fEventAborted) {
         fAModules->ResetAllActiveFlags();
         fEventAborted = kFALSE;
      }
      
      if (fVerbosity>9) {
         if ((entry % 100)==0) {
            fprintf(stderr,"Processing entry %lld...                       \r",
                    entry);
         }
      }
      fAModules->ExecuteTask(&TAModule::kExecProcess);

      ClearAllLoaders();
      if (fEventObjs.IsEmpty()==kFALSE) fEventObjs.Delete();

      // Restore object counter
      TProcessID::SetObjectCount(fObjCounter);
   }

   return kTRUE;
}


//______________________________________________________________________________
Bool_t TAMSelector::PublishObj(TObject *obj) 
{
   // Adds an object to a list of objects which is outside the module
   // hierarchy. This can be used to pass objects (for example, calibrations)
   // between modules. Objects in this list are available before Begin
   // until the end of SlaveTerminate. They are not guaranteed to be available
   // during or after Terminate.
   // Checks (by name) if the object is already in the list. If it is, returns
   // kFALSE and does not publish the object.
   // NOTE: These objects are NOT owned by the list! Whatever creates these
   // objects must take care to (1) remove the object from the list using
   // RetractObj()  and (2) delete the object.
   // Also NOTE: This will not publish TAModule objects.
   
   if (obj!=0) {
      TObject *ob = FindPublicObj(obj->GetName());
      if (ob==0) {
         if (obj->InheritsFrom(TAModule::Class())) {
            // Disallow publishing of TAModules since it breaks the assumption
            // that all TAModules in the input list were added by AddInput and
            // are intended to be executed. (Such modules would be found by
            // TakeModsFromInput and CopyModsFromInput.)
            Warning("PublishObj",
                    "Can not publish a TAModule. Object named [%s] not "
                    "published.",
                    obj->GetName());
         } else {
            if (fInput!=0) {
               fInput->Add(obj);
               return kTRUE;
            } else {
               Error("PublishObj",
                     "Input list is null. Could not publish object named [%s].",
                     obj->GetName());
            }
         }
      } else {
         Warning("PublishObj",
                 "Public object named [%s] already exists.",
                 obj->GetName());
      }
   } else {
      Error("PublishObj",
            "Can not publish null object.");
   }
   return kFALSE;
}


//______________________________________________________________________________
TObject *TAMSelector::RemoveObjThisEvt(const Char_t *name) 
{
   // Finds the  object with the specified name and removes it from
   // the list of objects added to this event.
   // Returns the object that was removed.
   
   TAMEvtObj *eo = dynamic_cast<TAMEvtObj*>(fEventObjs.FindObject(name));
   if (eo!=0) {
      fEventObjs.Remove(eo);
      TObject *obj = eo->fObj;
      eo->fObj = 0; // so it won't delete the object
      delete eo;
      return obj;
   }
   return 0;
}


//______________________________________________________________________________
TObject *TAMSelector::RetractObj(const Char_t *name) 
{
   // Finds the public object with the specified name and removes it from
   // the list of public objects.
   // Returns the object that was retracted.
   // Note: TAmodules are not public objects and will not be removed by this
   // function.
   
   TObject *obj = FindPublicObj(name);
   if (obj!=0) {
      return fInput->Remove(obj);
   }
   return 0;
}


//______________________________________________________________________________
void TAMSelector::SlaveBegin(TTree *tree) 
{
   // Inits the tree and calls SlaveBegin() on the TAModule(s)
   //
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   if (fAnalysisAborted) {
      return;
   } else {
      // call SlaveBegin first so the modules can call ReqBranch and
      // build the fBranchTable  
      if (fModAborted) {
         fAModules->ResetAllActiveFlags();
         fModAborted = kFALSE;
      }
      // remove the modules from the input list and put them in the top-most
      // module of this selector. make sure they use this selector
      TakeLoadersFromInput();
      TakeModsFromInput();
      fAModules->SetSelector(this);
      if (fAModules->CheckSelectors(this)) {
         // build the module output hierarchy
         AddNewOutputLists();
         // set up the module's output members
         if (fAModules->fOutput != 0) {
            fAModules->fOutput->SetAllOutputMembers(kFALSE);
         }

         fAModules->ExecuteTask(&TAModule::kExecSlaveBegin);
      } else {
         Error("SlaveBegin",
               "One or more module is not set up to use this selector.");
         R__ASSERT(0);
      }

      // init requires fBranchTable to be built already
      // in Proof, this is called automatically when a new tree is loaded
      if (tree) Init(tree);
      if (fEventObjs.IsEmpty()==kFALSE) fEventObjs.Delete();
   }
}


//______________________________________________________________________________
void TAMSelector::SlaveTerminate() 
{
   // Calls SlaveTerminate() on the TAModule(s)
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   if (fAnalysisAborted) {
      return;
   } else {
      if (fModAborted) {
         fAModules->ResetAllActiveFlags();
         fModAborted = kFALSE;
      }
      fAModules->ExecuteTask(&TAModule::kExecSlaveTerminate);
      if (fEventObjs.IsEmpty()==kFALSE) fEventObjs.Delete();
   }
}


//______________________________________________________________________________
void TAMSelector::TakeModsFromInput() 
{
   // Find all TAModules in the input list and move them to fAModules.
   
   R__ASSERT(fInput!=0);

   if (fInput->IsEmpty()==kFALSE) {
      TObject *obj=fInput->First(), *tobj=0;
      while (obj!=0) {
         tobj = fInput->After(obj);
         if (obj->InheritsFrom(TAModule::Class())) {
            AddInput(dynamic_cast<TAModule*>(obj));
            fInput->Remove(obj);
         }
         obj = tobj;
      }
   }
}


//______________________________________________________________________________
void TAMSelector::TakeLoadersFromInput() 
{
   // Find all TAModules in the input list and copy them to fAModules.

   R__ASSERT(fInput!=0);
   
   TList *loaders = dynamic_cast<TList*>(fInput->FindObject("TAM_LOADERS"));   
   if (loaders != 0) {
      TIter next(loaders);
      while ( TAMVirtualLoader *l = 
                    dynamic_cast<TAMVirtualLoader*>(next()) ) {
         if (loaders != &fLoaders) fLoaders.AddLast(l);
      }
   }
   
   fLoaders.AddLast(new TAMTreeLoader());
}


//______________________________________________________________________________
void TAMSelector::Terminate() 
{
   // Calls Terminate() on the TAModule(s).
   // When running under Proof, will copy the TAMOutput object
   // from the fOutput list to the top most TAModule object. Assumes
   // that the only TAMOutput object in fOutput is the one belonging
   // to the top most module.
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   if (fAnalysisAborted) {
      return;
   } 

   if (fModAborted) {
      fAModules->ResetAllActiveFlags();
      fModAborted = kFALSE;
   }
      
   if (fAModules->GetModOutput()==0) {
      // When running under Proof, copy the module output hierarchy to
      // this selector's top most module.
      TIter nextObj(GetOutputList());
      TAMOutput *tout=0;
      TObject *obj=0;
      Bool_t alreadySet=kFALSE;
      while ( (obj = nextObj()) ) {
         if (obj->InheritsFrom(TAMOutput::Class())) {
            tout = dynamic_cast<TAMOutput*>(obj);
            if (alreadySet) {
               Warning("Terminate",
                       "Found more than one TAMOutput object in the "
                       "output list. Assuming the first one is from the "
                       "top-most module. The output list contains:");
               GetOutputList()->ls();
            } else {
               // copy module output hierarchy
               fAModules->SetAllModOutput(tout);
               // try to set module's pointers to their output objects
               tout->SetAllOutputMembers(kTRUE);
               alreadySet=kTRUE;
            }
         }
      }
      if (alreadySet==kFALSE) {
         Error("Terminate",
               "Did not find TAMOutput object in the output list! "
               "The output list contains:");
         GetOutputList()->ls();
      }
   }
      
   // move output objs from current objects to stored objects
   // incase the module wants to write output to a file
   if (fAModules->GetModOutput()!=0) {
      fAModules->GetModOutput()->StoreAllOutputObjs();
   } else {
      Error("Terminate",
            "Could not store output objects from this process run.");
   }
      
   fAModules->ExecuteTask(&TAModule::kExecTerminate);
   if (fEventObjs.IsEmpty()==kFALSE) fEventObjs.Delete();

   // store output objects again, in case a module added any output
   // objects during terminate
   if (fAModules->GetModOutput()!=0) {
      fAModules->GetModOutput()->StoreAllOutputObjs();
   } else {
      Error("Terminate",
            "Could not store output objects after terminate.");
   }
}


//______________________________________________________________________________
void TAMSelector::ZeroAllBranches() 
{
   // Loops through all branches in fBranchTable and sets all user addresses
   // for each branch to zero.
   
   TIter nextBranch(fBranchTable.MakeIterator());
   while ( TAMBranchInfo *brInfo = 
              dynamic_cast<TAMBranchInfo*>(nextBranch()) ) {
         brInfo->ZeroUsrAddrs();
   }
}
