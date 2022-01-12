///////////////////////////////////////////////
//Author: Sophia Devinyak (s.devinyak@gmail.com , sdevinya@uwaterloo.ca)
//Date created: November 2021 at TRIUMF


#ifndef __input_h__
#define __input_h__

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <TH1F.h>
#include <TH2F.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include "EventClass.h"
#include <fstream>

#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TStyle.h"
#include "TH1I.h"
#include "TString.h"
#include "TSpectrum.h"

// stuff for the GUI
#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include <TRandom.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TApplication.h>
#include <TObject.h>
#include <TGraph.h>
#include <TGListBox.h>
#include <TEllipse.h>
#include <TDatime.h>

////////////////////////////////////////////////////////////////////////////////
/// This class is for inputting the information about the .root file.
class input
{
        RQ_OBJECT("input")
        private:
        TGMainFrame *fMain;				///< Main window frame
        TGTextEntry *f;     				///< Text entry field for the path to the .root file
        TGTextEntry *f2;    				///< Text entry for the name of the canvas that contains the histogram to be analyzed
        TGTextEntry *f3;    				///< Text entry for the name of the histogram
        public:
        input() {fMain =0; f=0; f2=0; f3=0;};
        input(const TGWindow *p, UInt_t w, UInt_t h);	///< Class constructor: creates a window to input the information about the .root file.
        virtual ~input();				///< Class destructor
        void searchZet();				///< Initiate search for Z peaks
//      ClassDef(input,1);  
};
////////////////////////////////////////////////////////////////////////////////
/// This class is for searching for the Z peaks.
class zetSearch
{
        RQ_OBJECT("zetSearch")
        private:
        TGMainFrame *fMain;					///< Main window frame
        TRootEmbeddedCanvas *fEcanvas;				///< Embedded canvas that will contain the histogram with highlighted found peaks
        TCanvas *zSearch;					///< Alias for the fEcanvas
        TGNumberEntry *fN2;					///< Number entry to select a peak
        TGraph *selected;					///< Scatter plot that highlights the selected peak
        TGraph *zscat;						///< Scatter plot with all found Z peaks
        public:
        zetSearch(const TGWindow *p, UInt_t w, UInt_t h);	///< Class constructor: constructs a window that will display found Z peaks
        virtual ~zetSearch();					///< Class destructor
        void zetFit();						///< Initiates fitting gaussians to the Z peaks.
        void zDeletePeak();					///< Deletes a selected peak
        void zSelectPeak();					///< Selects a peak
};
////////////////////////////////////////////////////////////////////////////////
/// This class is for fitting gaussians to the Z peaks
class Fits
{
        RQ_OBJECT("Fits")
        private:
        TGMainFrame *fMain;				///< This is a main frame that will contain Z fits
        TRootEmbeddedCanvas *fEcanvas;			///< This is an embedded canvas that will contain Z fits
        TGNumberEntry *fN1;				///<
        TGGroupFrame *fGframe;				///<
        TGLabel *fLabel;				///<
        public:
        Fits(const TGWindow *p, UInt_t w, UInt_t h);	///< Class constructor: constructs a window that will display fitted Z peaks
        virtual ~Fits();				///< Class destructor
        void zetFitPeaks();				///<
        void aoqFitPeaks();				///<
        void aoqSearchPeaks();				///< Initiate search for AOQ peaks
};
////////////////////////////////////////////////////////////////////////////////
/// This class is for searching and fitting AOQ peaks.
class aoqSearch
{
        RQ_OBJECT("aoqSearch")
        private:
        TGMainFrame *fMain;					///< This is the main frame that will contain found AOQ peaks
        TRootEmbeddedCanvas *fEcanvas2;				///< This is the embedded canvas that will contain a histogram with found AOQ peaks
        TGNumberEntry *fN1;					///< Number selection to select a canvas
        TGNumberEntry *fN2;					///< Number selection to select a peak within a selected canvas
        TGGroupFrame *fGframe;					///<
        TGLabel *fLabel;					///<
        TCanvas *ace;						///< Alias for fEcanvas2
        TGraph *scat;						///< Scatter plot to display all found peaks marked with green triangles
        TGraph *selected;					///< Scatter plot to mark a selected peak
        public:
        aoqSearch(const TGWindow *p, UInt_t w, UInt_t h);	///< Class constructor that construct a window that will display found AOQ peaks
        virtual ~aoqSearch();					///< Class destructor
        void SelectCanvas();					///< Selects a canvas
        void SelectPeak();					///< Selects a peak within a selected canvas
        void DeletePeak();					///< Deletes a selected peak
        void aoqFitPeaks();					///< Fits found AOQ peaks
};
////////////////////////////////////////////////////////////////////////////////
/// This class is for plotting ellipses around each identified isotope.
class ellipse
{
        RQ_OBJECT("ellipse")
        private:
        TGMainFrame *fMain;				///< This is a main frame
        TRootEmbeddedCanvas *fEcanvas4;			///< This is an embedded canvas that will contain the PID plot and ellipses around each identified isotope
        TGListBox *fListBox;				///< Listbox to select an element
        TCanvas *all;					///< Alias for fEcanvas4
        TGNumberEntry *fN1;				///< Number entry to enter the mass of the isotope
        TGNumberEntry *fN2;				///< Number entry to enter the increment to increase the AOQ radius by
        TGNumberEntry *fN3;				///< Number entry to enter the increment to increase the Z radius by
        TGNumberEntry *fN4;				///<
        TEllipse *e1;					///< Ellipse selection
	TGTextEntry *text;				///< Text entry for customized .txt file name
	TGLabel *label;
        public:
        ellipse(const TGWindow *p, UInt_t w, UInt_t h);	///< Class constructor: creates a window that will display the PID plot and the ellipses plotted around each identified isotope
        virtual ~ellipse();				///< Class destructor
        void ellipsePlot();				///< 
        void ellipseHere();				///< This function selects the ellipse and highlights it in green. 
        void ellipseAllHere();				///< Plot all ellipses for all Z values and AOQ values that were identified during the peak search.
        void ProtonNum();				///< Empty function because nothing needs to be done when this number is changed. This is actually the atomic mass, not proton number.
        void fixEllipse();				///< Change the radius of the ellipse and display it on the screen. 
        void saveTXT();					///< Save ellipse parameters and export them as a .txt file
        void DeleteEllipse();				///< Delete selected ellipse
};
#endif
