////////////////////////////////////////////////
//Author: Sophia Devinyak (s.devinyak@gmail.com , sdevinya@uwaterloo.ca)
//Date created: November 2021 at TRIUMF under the supervision of Dr. Roger Caballero-Folch


//Header file containing classes defining Branches of Trees for implant, decays, correlated events and background events.

#ifndef EVENTCLASS_H
#define EVENTCLASS_H

#include "TObject.h"
#include "TString.h"

///Stores scaler information for a single event.
class sScaler //: TObject 
{
	public: 
//		virtual ~sScaler();
//		virtual void Clear();
		ULong64_t evttime;//Stores current eventTime with highest available precision
		char* header;
		sScaler() : header(Form("evttime/l")) {}
	//ClassDef(sScaler,1)
};

class sID 
{
	public: 
		Float_t zet;
		Float_t aoq;
		char* header;
		sID() : header(Form("zet/F:aoq/F")) {}
	//ClassDef(sID,1)
};

class sImp//sSIEvent 
{
	public: 
		ULong64_t imptime;//Stores current eventTime with highest available precision
		Float_t posx;
		Float_t posy;
		Float_t posz;
		Float_t Ex;
		Float_t Ey;
		Float_t E;
		
		char* header;
		sImp() : header(Form("imptime/l:posx/F:posy/F:posz/F:Ex/F:Ey/F:E/F")) {}

	//ClassDef(sImp,1)
};

class sDec//sSIEvent //time is in sScaler class with evttime
{
	public: 
		Float_t posx;
		Float_t posy;
		Float_t posz;
		Float_t Ex;
		Float_t Ey;
		Float_t E;
		
		char* header;
		sDec() : header(Form("posx/F:posy/F:posz/F:Ex/F:Ey/F:E/F")) {}

	//ClassDef(sImp,1)
};
/*
class sCorrEvent {
	public: 
		Int_t impNum;
		Int_t decNum;
		Float_t dec_time;
		char* header;

		sCorrEvent() : header(Form("impNum/I:decNum/I:dec_time/F")) {}
	//ClassDef(sCorrEvent,1)
};
*/
class sNeu //: TObject 
{
	public: 
		ULong64_t neu_T;
		Float_t neu_E;
		Int_t neu_Id;
		Int_t neu_Ring;
		char* header;
		sNeu() : header(Form("neu_T/l:neu_E/F:neu_Id/I:neu_Ring/I")) {}
	//ClassDef(sNeu,1)
};
class sGam //: TObject 
{
	public: 
		ULong64_t gam_T;
		Float_t gam_E;
		Int_t gam_Id;
		char* header;
		sGam() : header(Form("gam_T/l:gam_E/F:gam_Id/I")) {}
	//ClassDef(sGam,1)
};
class sAnc //: TObject 
{
	public: 
		ULong64_t anc_T;
		Float_t anc_E;
		Int_t anc_Id;
		char* header;
		sAnc() : header(Form("anc_T/l:anc_E/F:anc_Id/I")) {}
	//ClassDef(sAnc,1)
};
/*
class sTpc //: TObject 
{
	public: 
		ULong64_t tpc_t;
		Float_t tpc_en;
		bool inspill;
		char* header;
		sTpc() : header(Form("tpc_t/l:tpc_en/F:inspill/O")) {}
	//ClassDef(sTpc,1)
};
class sEsr //: TObject 
{
	public: 
		ULong64_t esr_t;
		Float_t esr_en;
		bool inspill;
		char* header;
		sEsr() : header(Form("esr_t/l:esr_en/F:inspill/O")) {}
	//ClassDef(sEsr,1)
};
*/
#endif

