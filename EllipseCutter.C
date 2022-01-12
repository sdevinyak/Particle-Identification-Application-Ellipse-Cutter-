////////////////////////////////////////////////
//Author: Sophia Devinyak (s.devinyak@gmail.com , sdevinya@uwaterloo.ca)
//Date created: November 2021 at TRIUMF under the supervision of Dr. Roger Caballero-Folch



#include "input.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
/// A list of all the elements
std::vector<string> elements {"","Hydrogen","Helium","Lithium","Berylium","Boron","Carbon","Nitrogen","Oxygen","Fluorine","Neon","Sodium","Magnesium","Aluminum","Silicon","Phosphorus","Sulfur","Chlorine","Argon","Potassium","Calcium","Scandium","Titanium","Vanadium","Chromium","Manganese","Iron","Cobalt","Nickel","Copper","Zinc","Gallium","Germanium","Arsenic","Selenium","Bromine","Krypton","Rubidium","Strontium","Yttrium","Zirconium","Niobium","Molybdenum","Technetium","Ruthenium","Rhodium","Palladium","Silver","Cadmium","Indium","Tin","Antimony","Tellurium","Iodine","Xenon","Caesium","Barium","Lanthanum","Cerium","Praseodymium","Neodymium","Promethium","Samarium","Europium","Gadolinium","Terbium","Dysprosium","Holmium","Erbium","Thulium","Ytterbium","Lutetium","Hafnium","Tantalum","Tungsten","Rhenium","Osmium","Iridium","Platinum","Gold","Mercury","Thallium","Lead","Bismuth","Polonium","Astatine","Radon","Francium","Radium","Actinium","Thorium","Protactinium","Uranium","Neptunium","Plutonium","Americium","Curium","Berkelium","Californium","Einsteinium","Fermium","Mendelevium","Nobelium","Lawrencium","Rutherfordium","Dubnium","Seaborgium","Bohrium","Hassium","Meitherium","Darmstadtium","Roentgenium","Copernicium","Nihonium","Flerovium","Moscovium","Livermorium","Tennessine","Oganesson"};  

Double_t gausbkg(Double_t *x, Double_t *par);			///< This function is the definition of the gauss function used to fit peaks
Double_t bkg(Double_t *x, Double_t *par);			///< This is the function used by gausbkg used to describe the background 

// parameters obtained from searching for zet
std::vector<float> zHeight;					///< This is the vector to store the heights of all the found Z peaks 
std::vector<float> zMean;					///< This is the vector to store the centers of all the found Z peaks
TH1D *ZET = new TH1D("ZET","ZET",10000,20,40);			///< This is a histogram of all the Z values
TH2D *itot;							///< This is the value that will store the histogram from the .root file
TCanvas *cimp;							///< This is the canvas that contains the histogram of the Z vs AOQ plot from the .root file
TSpectrum *zg;			
Int_t zfound;							///< Number of found Z peaks
Double_t *zpeaks;						///< A vector of all the x-values of the Z peaks
std::vector<TH1D *> zet_vec;					

// parameters obtained from fitting zet 
std::vector<float> zetCenter;					///< Centroid of the Z gaussian fit 					
std::vector<float> lowLim;					///< Centroid of the Z gaussian fit minus one sigma
std::vector<float> highLim;					///< Centroid of the Z gaussian fit plut one sigma
std::vector<float> zetSigma;					///< This is actually sigma multiplied by cz; this makes it easier for future use
const int cz = 3; 						///< Constant to multiply sigmas by when it comes to Z values
const int ca = 3;						///< Constant to multiply sigmas by when it comes to AOQ values
// parameters obtained from searching for aoq
std::vector<std::vector<float>> height;				///< Heights of all found AOQ peaks for each Z value
std::vector<std::vector<float>> mean;				///< Centers of all found AOQ peaks for each Z value
TH1D *AOQ2;							///< This is for fitting AOQ peaks. This is a vector that contains the projections of AOQ values for each Z value
std::vector<TSpectrum *> g;					///< A vector of TSpectrums for each AOQ peak search for each Z value
std::vector<Int_t> found;					///< Vector of numbers of found AOQ peaks for each Z value
std::vector<Double_t *>peaks;					///< Vector of vectors of all x-positions of AOQ peaks for each Z value

//parameters obtained from fitting aoq
std::vector<std::vector<float>> aoqCenter;			///< Vector of vectors of all x-positions of AOQ centers
std::vector<std::vector<float>> zSigma;   			///< This vector will store the final Z values that will be used later. It will be modified when the ellipse radius is changed
std::vector<std::vector<float>> zSigma2; 			///< This vector will be used for initial graphing of the ellipses. 
std::vector<std::vector<float>> aoqLowLim;			///< AOQ center minus 3 sigmas
std::vector<std::vector<float>> aoqHighLim;			///< AOQ center plus 3 sigmas
std::vector<std::vector<float>> aoqSigma;			///< AOQ sigma times ca. The difference between aoqSigma and aoqSigma2 is the same as with zSigma
std::vector<std::vector<float>> aoqSigma2;  			

/*
// ellipse parameters
std::vector<float> z_val;					
std::vector<float> z_center;					
std::vector<std::vector<float>> a_val;				
std::vector<float> a_center;					
std::vector<float> a_radius;					
std::vector<float> z_radius;					
*/

////////////////////////////////////////////////////////////////////////////////
/// Creates a window with buttons and input fields. This window is designed for a user to input the information about their .root file that contains the PID plot (Z vs AOQ plot) to be analyzed
input::input(const TGWindow *p, UInt_t w, UInt_t h)
{
	// create a main frame
	fMain = new TGMainFrame (p,w,h);
	
	// create horizontal frames with text entry fields, labels and buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,40);   // creating a horizontal frame
        TGLabel *fL = new TGLabel(hframe,"Enter the path to the .root file containing the histogram");  // creating a label
	hframe->AddFrame(fL, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));  // adding the label to the window
	f = new TGTextEntry(hframe);
	hframe->AddFrame(f, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2)); // add hframe to parent widget. The order in which the frames are added is the order they will appear in the window

	TGHorizontalFrame *hframe2 = new TGHorizontalFrame(fMain,200,40);
        TGLabel *fL2 = new TGLabel(hframe2,"Enter the name of the canvas containing the histogram");
	hframe2->AddFrame(fL2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	f2 = new TGTextEntry(hframe2);
	hframe2->AddFrame(f2, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	TGHorizontalFrame *hframe3 = new TGHorizontalFrame(fMain,200,40);
        TGLabel *fL3 = new TGLabel(hframe3,"Enter the name of the histogram");
	hframe3->AddFrame(fL3, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	f3 = new TGTextEntry(hframe3);
	hframe3->AddFrame(f3, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe3, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

        TGHorizontalFrame *hframe4 = new TGHorizontalFrame(fMain,200,40);
	TGTextButton *zSear = new TGTextButton(hframe4, "&Search for Zet Peaks");//,this,"searchZet()");  // creating a button
	zSear->Connect("Clicked()","input",this,"searchZet()");						   // connecting the button to a function
	hframe4->AddFrame(zSear, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));				   // adding the button to the window
	TGTextButton *exit = new TGTextButton(hframe4, "&Exit", "gApplication->Terminate(0)");
	hframe4->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe4, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	f->SetText("Total.root");
	f2->SetText("cimp");
	f3->SetText("itot");

	fMain->SetCleanup(kDeepCleanup);

        // Set a name to the main frame
        fMain->SetWindowName("EllipseCutter - File Input");
        // next 3 lines make the widgets visible        
        // Map all subwindows of main frame
        fMain->MapSubwindows();
        // Initialize the layout algorithm
        fMain->Resize(fMain->GetDefaultSize());  // execute all layout specs for widgets before the top-level window is shown on screen
        // map main frame
        fMain->MapWindow();
}
////////////////////////////////////////////////////////////////////////////////
/// This function takes all the input values in the EllipseCutter - File Input window and uses them to retrieve the histogram with the PID plot to be analyzed.
/// This function also initiates the function that will search for Z peaks
void input::searchZet()
{
	// retrieving the histogram from the .root file specified by the user
	const char *totFile = f->GetText();
cout<<totFile<<endl;
	const char *canvName = f2->GetText();
cout<<canvName<<endl;
	const char *histName = f3->GetText();
cout<<histName<<endl;
	TFile *_file0 = TFile::Open(totFile);
	cimp = (TCanvas*)_file0->Get(canvName);
	itot = (TH2D*)cimp->GetPrimitive(histName);
	_file0->TFile::Close();
	new zetSearch(gClient->GetRoot(),200,200);  // initiating search for Z peaks
}

////////////////////////////////////////////////////////////////////////////////
/// Constructs a window to display the search of Z peaks.
/// The Z vs AOQ plot is projected onto the y-axis to get a histogram of Z values. This histogram will be searched for the Z values. All found peaks will be marked with a green triangle
zetSearch::zetSearch(const TGWindow *p, UInt_t w, UInt_t h)
{
	// create a main frame
	fMain = new TGMainFrame (p,w,h);

	// create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,800,600);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10,1));

	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,40);
	TGTextButton *del = new TGTextButton(hframe,"&Delete This Peak");
	del->Connect("Clicked()","zetSearch",this,"zDeletePeak()");
	hframe->AddFrame(del, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	TGTextButton *zetFit = new TGTextButton(hframe, "&Fit Zet Peaks");
	zetFit->Connect("Clicked()","zetSearch",this,"zetFit()");
	hframe->AddFrame(zetFit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	TGTextButton *exit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	TGHorizontalFrame *hframe4 = new TGHorizontalFrame(fMain,200,40);
	fN2 = new TGNumberEntry(hframe4, 1,9,999,TGNumberFormat::kNESInteger,
						TGNumberFormat::kNEANonNegative,
						TGNumberFormat::kNELLimitMinMax,
						1,99999);
	fN2->Connect("ValueSet(Long_t)","zetSearch",this,"zSelectPeak()");
	fN2->GetNumberEntry()->Connect("ReturnPressed()","zetSearch",this,"zSelectPeak()");
        TGLabel *fL2 = new TGLabel(hframe4,"Select the number of the peak to be removed (counted along the x-axis)");
	hframe4->AddFrame(fL2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	hframe4->AddFrame(fN2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	fMain->AddFrame(hframe4, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	fMain->SetCleanup(kDeepCleanup);

	// Set a name to the main frame
	fMain->SetWindowName("EllipseCutter - Z Search");

	// next 3 lines make the widgets visible	
	// Map all subwindows of main frame
	fMain->MapSubwindows();
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());  // execute all layout specs for widgets before the top-level window is shown on screen
	// map main frame
	fMain->MapWindow();
	
	
	ZET= itot->ProjectionY();   	// create zet histogram
	ZET->SetTitle("ZET");
	zSearch=fEcanvas->GetCanvas();
	zSearch->SetLogy(1);
	zSearch->SetTitle("Zet Search");
	zSearch->Draw();
	ZET->Draw();
	zg = new TSpectrum(20); 	// create TSpectrum for peak search
	double lastPar=0.000005;         // last par = smallest peak size / biggest peak size
	int binmax = ZET->GetMaximumBin();
	double x = ZET->GetBinContent(binmax);
	cout <<"Tallest peak " <<x << endl;
	if (x<100) {lastPar=0.015;}
	else if (x<1000) {lastPar = 0.04;}
	else if (x<20000) {lastPar = 0.00035;}
	else if (x>200000) {lastPar=0.000009;}
	zfound = zg->Search(ZET,0.9,"",lastPar);   // CHANGE THE LAST ARGUMENT (lastPar) IF NOT ALL PEAKS ARE FOUND
	zpeaks = zg->GetPositionX();   // getting x positions of all peaks
	// sorting the peaks in the order of appearance along the x axis
	int j = zfound;
	int ind[j];
	TMath::Sort(j,zpeaks,ind,false);
	for(int p=0;p<zfound;p++)  
	{
		float xp = zpeaks[ind[p]];
		Int_t bin = ZET->GetXaxis()->FindBin(xp);
		float yp = ZET->GetBinContent(bin);	// getting the y value of each peak
		zHeight.push_back(yp);
		zMean.push_back(xp);
	}
	//plotting a scatter plot of all peaks	
//	&zMean[0];
//	&zHeight[0];
	zscat = new TGraph(zMean.size(),zMean.data(),zHeight.data());
	zscat->SetMarkerStyle(23);
	zscat->SetMarkerColor(3);
	zscat->Draw("p");
	for (int p=0;p<zfound;p++) {cout<<zMean[p]<<endl;}
	//creating a "selected" peak
	float x1[1]={0};
	x1[0]=zMean[0];
	float y1[1]={0};
	y1[0]=zHeight[0];
	selected = new TGraph(1,x1,y1);
	selected->SetMarkerStyle(23);
	selected->SetMarkerColor(1);
	selected->Draw("p");
	zSearch->Update();
}

unsigned int zpk =0;			///< This variable stores an index of the selected peak
////////////////////////////////////////////////////////////////////////////////
/// Selects a Z peak and marks it with a red triangle.
void zetSearch::zSelectPeak()
{
	// get the number entry and convert it to 0 = 1st peak base
	TObject *sender = (TObject *)gTQSender;
	if (sender && sender->InheritsFrom("TGNumberEntry"))
	zpk = fN2->GetNumberEntry()->GetIntNumber() -1;
	if (zpk>=(zMean.size()-1))   //making sure that the selection is limited to the number of the peaks
	{
		fN2->SetNumber(zMean.size());
		zpk = fN2->GetNumberEntry()->GetIntNumber() -1;
	}
	if (!(zpk<zMean.size()))    // just a preaution
	{
		zpk = zMean.size()-1;
	}
	TObject *des=zSearch->FindObject(selected);
	des->Delete();  // delete the last selection to create a new one
	float x[1] = {0};  
	x[0] = zMean[zpk];
	float y[1] = {0};
	y[0] = zHeight[zpk];
	cout<<"Peak selected: "<< zMean[zpk]<<endl;
	selected= new TGraph(1,x,y);
	selected->SetMarkerStyle(23);
	selected->SetMarkerColor(1);
	selected->Draw("p");
	zSearch->Update();
}
////////////////////////////////////////////////////////////////////////////////
/// Deletes a selected Z peak and marks it with a black triangle.
void zetSearch::zDeletePeak()
{
	if (zMean.size()==1) {cout<<"Cannot delete the only Z peak."<<endl;}
	else
	{
		float delx[1] = {zMean[zpk]};    // record deleted coordinates in temporary memory
		float dely[1] = {zHeight[zpk]};
		zMean.erase(zMean.begin()+zpk);  // delete the coordinates from the vectors that hold all peak info
		zHeight.erase(zHeight.begin()+zpk);
		cout<<"Erased"<<endl;
		TGraph *del = new TGraph(1,delx,dely); // plot the deleted peak as a red triangle
		del->SetMarkerStyle(23);
		del->SetMarkerColor(2);
		del->Draw("p");	
		TObject *des=zSearch->FindObject(selected);   // delete the last selection to select the new peak
		des->Delete();
		if (zpk>=(zMean.size()-1))   // limiting the selection to the number of peaks
		{
			fN2->SetNumber(zMean.size());
			zpk = fN2->GetNumberEntry()->GetIntNumber() -1;
		}
		if (!(zpk<zMean.size())) 
		{
			zpk = zMean.size()-1;
		}
		float x[1] = {0};
		x[0] = zMean[zpk];
		float y[1] = {0};
		y[0] = zHeight[zpk];
		cout<<"Peak selected: "<< zMean[zpk]<<endl;
		selected= new TGraph(1,x,y);
		selected->SetMarkerStyle(23);
		selected->SetMarkerColor(1);
		selected->Draw("p");
		zSearch->Update();
	}
}
////////////////////////////////////////////////////////////////////////////////
/// Initiates fitting gaussians to the Z peaks.
void zetSearch::zetFit()
{
	new Fits(gClient->GetRoot(),200,200);
}
////////////////////////////////////////////////////////////////////////////////
/// Creates a window that will display all fitted Z peaks. 
/// The canvas is divided according to the number of peaks that are baing fitted.
/// Retrieved valus of the centroid and sigma are stored in the appropriate vectors.
Fits::Fits(const TGWindow *p, UInt_t w, UInt_t h)
{
	// create a main frame
	fMain = new TGMainFrame (p,w,h);

	// create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,800,600);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10,1));
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,40);
	TGTextButton *zetFit = new TGTextButton(hframe, "&Search for AOQ Peaks");
	zetFit->Connect("Clicked()","Fits",this,"aoqSearchPeaks()");
	hframe->AddFrame(zetFit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	TGTextButton *exit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	fMain->SetCleanup(kDeepCleanup);

	// Set a name to the main frame
	fMain->SetWindowName("EllipseCutter - Z Fits");

	// next 3 lines make the widgets visible	
	// Map all subwindows of main frame
	fMain->MapSubwindows();
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());  // execute all layout specs for widgets before the top-level window is shown on screen
	// map main frame
	fMain->MapWindow();

	cout<<"Fitting zet peaks now. Please be patient..."<<endl;
	TCanvas *zet = fEcanvas->GetCanvas();
	unsigned int x = 0;
	while (zMean.size()>x*x || zMean.size()>x*(x+1)) {x+=1;}  // find out how to divide the canvas
	if (zMean.size()<=x*(x-1))    // divide the canvas
	{
		zet->Divide(x,x-1);
	}
	else
	{
		zet->Divide(x,x);
	}
	for (unsigned int n=0;n<zMean.size();n++)
	{	
		zet->cd(n+1);   // select the canvas
	        float center = zMean[n];          // center of the peak
	        float ll = center - 0.36; // lower limit
	        float hl = center + 0.36;// higher limit
		int cen = round(center);
		TH1D *z;
		z=itot->ProjectionY(Form("z%i",cen));  // create the histogram to fit the peak
		z->SetTitle(Form("Zet%i",cen));
		z->Draw();   // goff doesnt work
		z->GetXaxis()->SetRangeUser(ll,hl);
	        zet->SetGridx(1);
		zet->SetGridy(1);
		z->Fit("gaus",Form("z%i",cen),"same",ll,hl);  // fit the gaussian
	        gStyle->SetOptFit();
		TF1 *fitpeak = (TF1*)z->GetListOfFunctions()->FindObject("gaus");  // retrieve the gaussian
		fitpeak->Draw("same");    // plot it again bc Fit() doesn't plot all the peaks
		float p1=fitpeak->GetParameter(1);   // getting mean
	        float p2=TMath::Abs(fitpeak->GetParameter(2));  // getting sigma
		if (p2<0.5)
		{
			zetSigma.push_back(p2*cz);
			zetCenter.push_back(p1);
			lowLim.push_back(p1-1.0*p2);
			highLim.push_back(p1+1.0*p2);
		        cout<< Form("Center of the peak at %f = %f",center,p1)<<endl;
		}
		else {cout<<"Peak at z = "<<p1<<" wasn't fitted properly. Sigma too big. This peak was omitted"<<endl;}
		zet->Update();
	}
}
////////////////////////////////////////////////////////////////////////////////
/// Initiate search for AOQ peaks.
void Fits::aoqSearchPeaks()
{
	new aoqSearch(gClient->GetRoot(),200,200);
}
////////////////////////////////////////////////////////////////////////////////
/// Create a window that will display all found AOQ peaks.
/// The window will be divided into an appropriate number of subcanvas according to how many Z values are searched for AOQ peaks.
aoqSearch::aoqSearch(const TGWindow *p, UInt_t w, UInt_t h)
{
	// create a main frame
	fMain = new TGMainFrame (p,w,h);

	// create canvas widget
	fEcanvas2 = new TRootEmbeddedCanvas("Ecanvas2",fMain,800,600);
	fMain->AddFrame(fEcanvas2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10,1));
	
	ace = fEcanvas2->GetCanvas();

	TGHorizontalFrame *hframe3 = new TGHorizontalFrame(fMain,200,40);
	fMain->AddFrame(hframe3, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	TGHorizontalFrame *hframe4 = new TGHorizontalFrame(fMain,200,40);
	fMain->AddFrame(hframe4, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	TGHorizontalFrame *hframe2 = new TGHorizontalFrame(fMain,200,40);
	fMain->AddFrame(hframe2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	fN1 = new TGNumberEntry(hframe3, 1,9,999,TGNumberFormat::kNESInteger,
						TGNumberFormat::kNEANonNegative,
						TGNumberFormat::kNELLimitMinMax,
						1,99999);
	fN1->Connect("ValueSet(Long_t)","aoqSearch",this,"SelectCanvas()");
	fN1->GetNumberEntry()->Connect("ReturnPressed()","aoqSearch",this,"SelectCanvas()");
	TGLabel *fL1 = new TGLabel(hframe3,"Select the canvas where you want to remove peaks");
	hframe3->AddFrame(fL1, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	hframe3->AddFrame(fN1, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	
	fN2 = new TGNumberEntry(hframe4, 1,9,999,TGNumberFormat::kNESInteger,
						TGNumberFormat::kNEANonNegative,
						TGNumberFormat::kNELLimitMinMax,
						1,99999);
	fN2->Connect("ValueSet(Long_t)","aoqSearch",this,"SelectPeak()");
	fN2->GetNumberEntry()->Connect("ReturnPressed()","aoqSearch",this,"SelectPeak()");
        TGLabel *fL2 = new TGLabel(hframe4,"Select the number of the peak to be removed (counted along the x-axis)");
	hframe4->AddFrame(fL2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));

	hframe4->AddFrame(fN2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));

	TGTextButton *del = new TGTextButton(hframe2,"&Delete This Peak");
	del->Connect("Clicked()","aoqSearch",this,"DeletePeak()");
	hframe2->AddFrame(del, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	TGTextButton *fit = new TGTextButton(hframe2,"&Fit Found AOQ Peaks");
	fit->Connect("Clicked()","aoqSearch",this,"aoqFitPeaks()");
	hframe2->AddFrame(fit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	TGTextButton *exit = new TGTextButton(hframe2, "&Exit", "gApplication->Terminate(0)");
	hframe2->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	fMain->SetCleanup(kDeepCleanup);

	// Set a name to the main frame
	fMain->SetWindowName("EllipseCutter - AoQ Search");

	// next 3 lines make the widgets visible	
	// Map all subwindows of main frame
	fMain->MapSubwindows();
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());  // execute all layout specs for widgets before the top-level window is shown on screen
	// map main frame
	fMain->MapWindow();

	unsigned int x=0;
	cout<<"Searching for aoq peaks..."<<endl;
	while (zMean.size()>x*x || zMean.size()>x*(x+1)) {x+=1;}
	if (zMean.size()<=x*(x-1))   // dividing the canvas
	{
		ace->Divide(x,x-1);
	}
	else
	{
		ace->Divide(x,x);
	}
	for (unsigned int n=0;n<zetCenter.size();n++)   // searching for aoq peaks in the same way as the zet peaks
	{
		float p1 = zetCenter[n];
		int cen = round(p1);
		ace->cd(n+1);
		gPad->SetLogy(1);
		ace->SetLogy(1);
		TH1D *AOQ;
		int lb = itot->GetYaxis()->FindBin(lowLim[n]);
		int hb = itot->GetYaxis()->FindBin(highLim[n]);
		AOQ = itot->ProjectionX(Form("aoq at z=%i",cen),lb,hb,""); // create the histogram
		AOQ->SetTitle(Form("aoq at z=%i",cen));
		AOQ->Draw();
		g.push_back(new TSpectrum(20));
		double lastPar=0.000005;
		int binmax = AOQ->GetMaximumBin();
		double x = AOQ->GetBinContent(binmax);
		if (x<100) {lastPar=0.015;}
		else if (x<1000) {lastPar = 0.04;}
		else if (x<20000) {lastPar = 0.00035;}
		else if (x>200000) {lastPar=0.0000045;}
		found.push_back(g[n]->Search(AOQ,0.9,"",lastPar));   // CHANGE THE LAST ARGUMENT (lastPar) IF NOT ALL PEAKS ARE PLOTTED
		peaks.push_back(g[n]->GetPositionX());
		int j = found[n];        
		int ind[j];
		TMath::Sort(j,peaks[n],ind,false);    // sorting the peaks in the order of their appearance on the x axis
		std::vector<float> height_temp;
		std::vector<float> mean_temp;
		for(int p=0;p<found[n];p++)  
		{
			float xp = peaks[n][ind[p]];
			Int_t bin = AOQ->GetXaxis()->FindBin(xp);
			float yp = AOQ->GetBinContent(bin);
			height_temp.push_back(yp);
			mean_temp.push_back(xp);
		}
		scat = new TGraph(mean_temp.size(),mean_temp.data(),height_temp.data());    // scatter plot of found peaks
		scat->SetMarkerStyle(23);
		scat->SetMarkerColor(3);
		scat->Draw("p");
		height.push_back(height_temp);
		mean.push_back(mean_temp);
		ace->Update();
	}
	float x1[1]={0};
	x1[0]=mean[0][0];
	float y1[1]={0};
	y1[0]=height[0][0];
	ace->cd(1);
	selected = new TGraph(1,x1,y1);  // plot selected peak
	selected->SetMarkerStyle(23);
	selected->SetMarkerColor(1);
	selected->Draw("p");
	ace->Update();
}
unsigned int canv = 0;			///< This variable stores the index of the selected canvas
unsigned int pk = 0;			///< This variable stores the index of the selected peak
////////////////////////////////////////////////////////////////////////////////
/// Selects a subcanvas. A peak number pk will be highlighted with a black triangle in that canvas.
void aoqSearch::SelectCanvas()
{
	TObject *sender = (TObject *)gTQSender;
	if (sender && sender->InheritsFrom("TGNumberEntry"))
	canv = fN1->GetNumberEntry()->GetIntNumber() -1;          // get the entry
	if (canv>=(zetCenter.size()-1))   // limit the selection to the number of subcanvases
	{
		fN1->SetNumber(zetCenter.size());
		canv = fN1->GetNumberEntry()->GetIntNumber()-1;
	}
	if (pk>=(mean[canv].size()-1))  // precaution
	{
		fN2->SetNumber(mean[canv].size());
		pk = fN2->GetNumberEntry()->GetIntNumber()-1;
	}
	if (!(canv<zetCenter.size()))    // precaution
	{ 
		canv = zetCenter.size()-1;		
	}
	TObject *des=ace->FindObject(selected);   // delete the selected peak
	des->Delete();
	float x[1] = {0};
	x[0] = mean[canv][pk];
	float y[1] = {0};
	y[0] = height[canv][pk];
	cout<<"Canvas selected: zet = "<<zetCenter[canv]<<endl;
	ace->cd(canv+1);		   // select canvas
	selected= new TGraph(1,x,y);     // plot new selection
	selected->SetMarkerStyle(23);
	selected->SetMarkerColor(1);
	selected->Draw("p");
	ace->Update();
}
////////////////////////////////////////////////////////////////////////////////
/// Select an AOQ peak. The selected peak will be highlighted with a black triangle.
void aoqSearch::SelectPeak()
{
	TObject *sender = (TObject *)gTQSender;
	if (sender && sender->InheritsFrom("TGNumberEntry"))
	pk = fN2->GetNumberEntry()->GetIntNumber() -1;       // retrieve the number entry
	if (pk>=(mean[canv].size()-1))				// limit the selection to the number of peaks in that subcanvas
	{
		fN2->SetNumber(mean[canv].size());
		pk = fN2->GetNumberEntry()->GetIntNumber()-1;
	}
	if (!(pk<mean[canv].size())) 
	{
		pk = mean[canv].size()-1;
	}
	TObject *des=ace->FindObject(selected);   // delete last selection
	des->Delete();
	float x[1] = {0};
	x[0] = mean[canv][pk];
	float y[1] = {0};
	y[0] = height[canv][pk];
	cout<<"Peak selected: "<< mean[canv][pk]<<endl;
	ace->cd(canv+1);	// select canvas	
	selected= new TGraph(1,x,y);    // plot new selection
	selected->SetMarkerStyle(23);
	selected->SetMarkerColor(1);
	selected->Draw("p");
	ace->Update();
}
////////////////////////////////////////////////////////////////////////////////
/// Deletes the selected AOQ peak, marks it with a red triangle and selects the next peak.
void aoqSearch::DeletePeak()
{
	if (mean[canv].size()==1) {cout<<"Cannot delete the only AoQ peak for this Z. Delete the corresponding Z peak when searching for Z Peaks instead"<<endl;}
	else
	{
		float delx[1] = {mean[canv][pk]};   // temporary memory for the deleted peak
		float dely[1] = {height[canv][pk]};
		mean[canv].erase(mean[canv].begin()+pk);    // erase the coordinates of the deleted peak
		height[canv].erase(height[canv].begin()+pk);
		cout<<"Erased"<<endl;
		ace->cd(canv+1);
		TGraph *del = new TGraph(1,delx,dely);              // plot the deleted peak as a red triangle
		del->SetMarkerStyle(23);
		del->SetMarkerColor(2);
		del->Draw("p");	
		TObject *des=ace->FindObject(selected);          // delete the last selection
		des->Delete();
		if (pk>=(mean[canv].size()-1))			// limiting the selection to the number of peaks available
		{
			fN2->SetNumber(mean[canv].size());
			pk = fN2->GetNumberEntry()->GetIntNumber()-1;
		}
		if (!(pk<mean[canv].size())) 
		{
			pk = mean[canv].size()-1;
		}
		float x[1] = {0};
		x[0] = mean[canv][pk];
		float y[1] = {0};
		y[0] = height[canv][pk];
		cout<<"Peak selected: "<< mean[canv][pk]<<endl;
		ace->cd(canv+1);		
		selected= new TGraph(1,x,y);   // plot the new selection
		selected->SetMarkerStyle(23);
		selected->SetMarkerColor(1);
		selected->Draw("p");
		ace->Update();
	}
}
////////////////////////////////////////////////////////////////////////////////
/// Fitting found AOQ peaks. Each Z value creates a new canvas. 
/// The canvas are divided into an appropriate number of subcanvases to fit all peaks.
/// Retrieved valued of peak centroids and sigmas are stored in appropriate vectors.
/// Vectors for plotting the ellipses and exporting the data are created.
void aoqSearch::aoqFitPeaks()
{
	for (unsigned int n=0;n<mean.size();n++)   // fitting aoq peaks
	{
		std::vector<float> c_temp;  // temporary vectors to create vector for ellipse plotting and exporting data
		std::vector<float> l_temp;
		std::vector<float> h_temp;
		std::vector<float> s_temp;
		std::vector<float> z_temp;
		float p1 = zetCenter[n];
		int cen = round(p1);
		TCanvas *afit = new TCanvas(Form("aoqFit z=%f",p1),Form("aoqFit z=%f",p1));
		unsigned int x =0;
		while (mean[n].size()>x*x || mean[n].size()>x*(x+1)) {x+=1;}
		if (mean[n].size()<=x*(x-1))   // dividing the canvas
		{
			afit->Divide(x,x-1);
		}
		else
		{
			afit->Divide(x,x);
		}
		int index = 1;
		for (unsigned int p=0;p<mean[n].size();p++)
		{
			afit->cd(index);
			gStyle->SetOptStat(1001111);
			float center = mean[n][p];
			float ll = center - 0.012;		// limits of the fit
			float hl = center + 0.012;
			int lb = itot->GetYaxis()->FindBin(lowLim[n]);   // finding the bin of the lower bound
			int hb = itot->GetYaxis()->FindBin(highLim[n]);
			AOQ2 = itot->ProjectionX(Form("z=%i, aoq=%f",cen,center),lb,hb,"");  // making the histogram of the aoq for a given zet value
			AOQ2->SetTitle(Form("z=%i, aoq=%f",cen,center));
			AOQ2->GetXaxis()->SetRangeUser(ll,hl);
			AOQ2->Draw("same");
//			if (integral<1000)
//			{
//				ll = center - 0.01;
//				hl = center + 0.01;
//				AOQ2[n][p] = new TH1D(Form("AOQ2%f-%f",p1,mean[n][p]),Form("AOQ2%f-%f",p1,mean[n][p]),1000,ll,hl);
//			}
			Double_t integral = 0;
			// if there are not a lot of data points, rebin for better fit
			integral = AOQ2->Integral();  
			if (integral<1000) 
			{
				AOQ2->Rebin(2);
				AOQ2->Rebin(2);
				AOQ2->Rebin(2);
				AOQ2->Rebin(2);
			}

			afit->SetGridx(1);
			afit->SetGridy(1);
			TF1 *f1 = new TF1("f1","gaus");  // defining the fitting function
			f1->SetParameter(1,mean[n][p]);  // pre setting the parameters
			AOQ2->Fit("f1",Form("z=%i, aoq=%f",cen,center),"same",ll,hl);    // fitting the peak
			f1->SetTitle("Fit AOQ");
			TF1 *fitpeak = (TF1*)AOQ2->GetListOfFunctions()->FindObject("f1");  // retrieving the fitted curve from the canvas
			fitpeak->Draw("same");  // drawing the fitted curve over again - if this is not done, not all curves will be displayed for some reason
			afit->Update();
			AOQ2->GetXaxis()->SetRangeUser(ll,hl);
			float m = f1->GetParameter(1);  //getting mean
			float s = f1->GetParameter(2);  //getting sigma
			if (s<0.03)   // checking if the fit isn't too broad. If it is, the values will not be stored. This is to eliminate bad fits
			{	
				c_temp.push_back(m);
				l_temp.push_back(m-3*TMath::Abs(s));
				h_temp.push_back(m+3*TMath::Abs(s));
				s_temp.push_back(s*ca);
				z_temp.push_back(zetSigma[n]);
			}
			else {cout<<"Peak at aoq = "<<m<<" was not fitted properly. Sigma = "<<s<<" too big. The peak was omitted" <<endl;}
			gStyle->SetOptFit();
			afit->Update();
			index++;
		}
		aoqCenter.push_back(c_temp);
		aoqLowLim.push_back(l_temp);
		aoqHighLim.push_back(h_temp);
		aoqSigma.push_back(s_temp);
		zSigma.push_back(z_temp);
	}
	zSigma2=zSigma;
	aoqSigma2=aoqSigma;
	new ellipse(gClient->GetRoot(),200,200);
	cout<<"done"<<endl;
}
////////////////////////////////////////////////////////////////////////////////
/// Creating a window where the PID plot will be fitted with the ellipses on top of it, selecting each isotope.
/// The isotope will be selected by selecting the element and its atomic mass.
/// There are buttons for changing the ellipse radii and saving their properties. 
ellipse::ellipse(const TGWindow *p, UInt_t w, UInt_t h)
{
	// create a main frame
	fMain = new TGMainFrame (p,w,h);

	// create canvas widget
	fEcanvas4 = new TRootEmbeddedCanvas("Ecanvas4",fMain,800,600);
	fMain->AddFrame(fEcanvas4, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10,1));

	gStyle->SetOptStat(1000011);    // plotting the zet vs aoq histogram
	all = fEcanvas4->GetCanvas();
	all->SetLogz(1);
	all->Draw();  
	itot->SetTitle("Zet vs Aoq");
	itot->GetYaxis()->SetTitle("Zet");
	itot->GetYaxis()->CenterTitle();
	itot->GetXaxis()->SetTitle("Aoq");
	itot->GetXaxis()->CenterTitle();
	itot->Draw("zcol");
	TGHorizontalFrame *box = new TGHorizontalFrame(fMain,200,40);

	fN1 = new TGNumberEntry(box, 1,9,999,TGNumberFormat::kNESInteger,
						TGNumberFormat::kNEANonNegative,
						TGNumberFormat::kNELLimitMinMax,
						1,999);
	fN1->Connect("ValueSet(Long_t)","ellipse",this,"ProtonNum()");   // this is actually a number entry to enter the mass number of the isotope, NOT the proton number
	fN1->GetNumberEntry()->Connect("ReturnPressed()","ellipse",this,"ProtonNum()");
	TGLabel *fL1 = new TGLabel(box,"Enter the mass number of the isotope");
	box->AddFrame(fL1, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	box->AddFrame(fN1, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));

	fListBox = new TGListBox(box,90);    // making a listbox of elements
	for (unsigned int i=0;i<zetCenter.size();i++)
	{
		for (unsigned int j=0;j<=elements.size();j++)
		{
			if (j==round(zetCenter[i]))
			{
				fListBox->AddEntry(elements[j].c_str(),j);
			}
		}

	}
	fListBox->Resize(150,80);
	box->AddFrame(fListBox, new TGLayoutHints(kLHintsTop|kLHintsLeft, 5,5,5,5));

	TGTextButton *el = new TGTextButton(box, "&Select This Ellipse To Edit");
	el->Connect("Clicked()","ellipse",this,"ellipseHere()");
	box->AddFrame(el, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(box, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	TGHorizontalFrame *hframe4 = new TGHorizontalFrame(fMain,200,40);
	fN2 = new TGNumberEntry(hframe4, 0,9,100,TGNumberFormat::kNESRealFour,
						TGNumberFormat::kNEAAnyNumber,
						TGNumberFormat::kNELLimitMinMax,
						-0.9999,0.9999);
	fN2->Connect("ValueSet(Long_t)","ellipse",this,"fixEllipse()");
	fN2->GetNumberEntry()->Connect("ReturnPressed()","ellipse",this,"fixEllipse()");
        TGLabel *fL2 = new TGLabel(hframe4,"Increase the aoq radius by ");
	hframe4->AddFrame(fL2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	hframe4->AddFrame(fN2, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));

	fN3 = new TGNumberEntry(hframe4, 0,9,100,TGNumberFormat::kNESRealTwo,
						TGNumberFormat::kNEAAnyNumber,
						TGNumberFormat::kNELLimitMinMax,
						-0.99,0.99);
	fN3->Connect("ValueSet(Long_t)","ellipse",this,"fixEllipse()");
	fN3->GetNumberEntry()->Connect("ReturnPressed()","ellipse",this,"fixEllipse()");
        TGLabel *fL3 = new TGLabel(hframe4,"Increase the zet radius by ");
	hframe4->AddFrame(fL3, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));

	hframe4->AddFrame(fN3, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	TGTextButton *appr = new TGTextButton(hframe4, "&Delete This Ellipse");
	appr->Connect("Clicked()","ellipse",this,"DeleteEllipse()");
	hframe4->AddFrame(appr, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe4, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	TGHorizontalFrame *hframe2 = new TGHorizontalFrame(fMain,200,40);
        label = new TGLabel(hframe2,"Enter the filename. If 0 is entered, the default name will be chosen");
	hframe2->AddFrame(label, new TGLayoutHints(kLHintsTop|kLHintsLeft,5,5,5,5));
	text = new TGTextEntry(hframe2);
	text->SetText("0");;
	hframe2->AddFrame(text, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	TGTextButton *sav = new TGTextButton(hframe2, "&Save Approved Ellipses");
	sav->Connect("Clicked()","ellipse",this,"saveTXT()");
	hframe2->AddFrame(sav, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	TGTextButton *exit = new TGTextButton(hframe2, "&Exit", "gApplication->Terminate(0)");
	hframe2->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
	fMain->AddFrame(hframe2, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	// Set a name to the main frame

	fMain->SetWindowName("Ellipse Cutter - Plotting Ellipses");

	// next 3 lines make the widgets visible	
	// Map all subwindows of main frame
	fMain->MapSubwindows();
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());  // execute all layout specs for widgets before the top-level window is shown on screen
	// map main frame
	fMain->MapWindow();
	ellipseAllHere();
}
////////////////////////////////////////////////////////////////////////////////
/// Empty function because nothing needs to be done when this number is changed. 
/// This is actually the atomic mass, not proton number.
void ellipse::ProtonNum()
{
}

float aoq_pre = 0.0;		///< Theoretical AOQ value of the selected isotope 
float zet_pre = 0.0;		///< Number of protons corresponding to the element that the user selected from the listbox
float zet =0.0;			///< Corresponding Z coordinate of the ellipse center
float aoq = 0.0;		///< Corresponding AOQ coordinate of the ellipse center
int zet_id = 0;			///< The index of the Z coordinate of the ellipse center in the zetCenter vector
int aoq_id = 0;			///< The index of the AOQ coordinate of the ellipse center in the aoqCenter vector
int a_pre=0;			///< The atomic mass selected by the user
float ra_inc = 0.0;		///< Increment by which the AOQ radius of the ellipse will be increased
float rz_inc=0.0;		///< Increment by which the Z radius of the ellipse will be increased
float as = 0.0;			///< Corresponding value from the aoqSigma2 vector
float zs = 0.0;			///< Corresponding value from the zSigma2 vector
////////////////////////////////////////////////////////////////////////////////
/// This function selects the ellipse and highlights it in green. 
void ellipse::ellipseHere() 
{
	a_pre =fN1->GetNumberEntry()->GetIntNumber();   // getting the atomic mass number
cout<<"A = "<<a_pre<<endl;
	int zet_pre = fListBox->GetSelected();       // getting the z number
cout<<"Z = "<<zet_pre<<endl;
	for (unsigned int k=0;k<zetCenter.size();k++)
	{
		if ( (zetCenter[k]-zetSigma[k])<zet_pre && zet_pre<(zetCenter[k]+zetSigma[k]))
		{
			zet=zetCenter[k];
			zet_id=k;
		}
	}
	if (zet==0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
	else
	{
		aoq_pre= a_pre/zet;      // calculating the aoq number
		cout<<"Aoq = "<<aoq_pre<<endl;
		for (unsigned int k=0;k<aoqCenter[zet_id].size();k++)
		{
			if ( (aoqCenter[zet_id][k]-0.03)<aoq_pre && aoq_pre<(aoqCenter[zet_id][k]+0.03))
			{
				aoq=aoqCenter[zet_id][k];
				aoq_id=k;
			}
		}
	}
	if (zet==0 || aoq == 0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
	else
	{
		all->cd(1); // select canvas 
		float zc = zetCenter[zet_id];    // plot the ellipse
		zs = zSigma2[zet_id][aoq_id];
		float ac = aoqCenter[zet_id][aoq_id];
		as = aoqSigma2[zet_id][aoq_id];
		e1 = new TEllipse(ac,zc,as,zs,0,360,0);
		e1->SetLineColor(3);
		e1->SetLineWidth(2);
		e1->SetFillStyle(0);
		e1->Draw();
		all->Update();
		cout<<"done"<<endl;
		zet=0;
		aoq=0;
	}
	ra_inc = 0.0;
	rz_inc=0.0;
	fN2 -> SetNumber(0.0);
	fN3 -> SetNumber(0.0);
}

float zc = 0;			///< Z center of the ellipse
float ac=0;			///< AOQ center of the ellipse
float z_data=0;			///< Safestoring the zet_pre value. For some reason, without this value the function doesn't work properly.
////////////////////////////////////////////////////////////////////////////////
/// This function deleted the selected ellipse and highlights it in black. 
void ellipse::DeleteEllipse()
{
	ra_inc = fN2->GetNumberEntry()->GetNumber();  // ra and rz are by how much to increase the aoq and z rabius
	rz_inc= fN3->GetNumberEntry()->GetNumber();
	a_pre =fN1->GetNumberEntry()->GetIntNumber();
	int zet_pre = fListBox->GetSelected();
	for (unsigned int k=0;k<zetCenter.size();k++)
	{
		if ( (zetCenter[k]-zetSigma[k])<zet_pre && zet_pre<(zetCenter[k]+zetSigma[k]))
		{
			zet=zetCenter[k];
			zet_id=k;
		}
	}
	if (zet==0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
	else 
	{
		aoq_pre= a_pre/zet;
		for (unsigned int k=0;k<aoqCenter[zet_id].size();k++)
		{
			if ( (aoqCenter[zet_id][k]-0.03)<aoq_pre && aoq_pre<(aoqCenter[zet_id][k]+0.03))
			{
				aoq=aoqCenter[zet_id][k];
				aoq_id=k;
			}
		}
		if (zet==0 || aoq == 0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
		else
		{
			e1->Delete();
			all->cd(1); // select canvas
			zc = zetCenter[zet_id];
			zSigma[zet_id][aoq_id]=zSigma2[zet_id][aoq_id]+rz_inc;
			zs = zSigma2[zet_id][aoq_id];
			ac = aoqCenter[zet_id][aoq_id];
			aoqSigma[zet_id][aoq_id]=aoqSigma2[zet_id][aoq_id]+ra_inc;
			as = aoqSigma2[zet_id][aoq_id];
			TEllipse *e1 = new TEllipse(ac,zc,as,zs,0,360,0);   // plot the deleted ellipse red
			e1->SetLineWidth(2);
			e1->SetFillStyle(0);
			e1->SetLineColor(1);
			e1->Draw();
			all->Update();
			cout<<"done"<<endl;
			zet=0;
			aoq=0;
			aoqCenter[zet_id].erase(aoqCenter[zet_id].begin()+aoq_id);   // delete coordinates of a deleted ellipse
			aoqSigma[zet_id].erase(aoqSigma[zet_id].begin()+aoq_id);
			aoqSigma2[zet_id].erase(aoqSigma2[zet_id].begin()+aoq_id);
			zSigma[zet_id].erase(zSigma[zet_id].begin()+aoq_id);
			zSigma2[zet_id].erase(zSigma2[zet_id].begin()+aoq_id);
		}
		z_data=zet_pre;

	}
	ra_inc = 0.0;
	rz_inc=0.0;
}
////////////////////////////////////////////////////////////////////////////////
/// Change the radius of the ellipse and display it on the screen. 
void ellipse::fixEllipse()
{
	ra_inc = fN2->GetNumberEntry()->GetNumber();   // ra and rz are same as before
	rz_inc= fN3->GetNumberEntry()->GetNumber();
	a_pre =fN1->GetNumberEntry()->GetIntNumber();
	int zet_pre = fListBox->GetSelected();
	for (unsigned int k=0;k<zetCenter.size();k++)
	{
		if ( (zetCenter[k]-zetSigma[k])<zet_pre && zet_pre<(zetCenter[k]+zetSigma[k]))
		{
			zet=zetCenter[k];
			zet_id=k;
		}
	}
	if (zet==0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
	else
	{
		aoq_pre= a_pre/zet;
		for (unsigned int k=0;k<aoqCenter[zet_id].size();k++)
		{
			if ( (aoqCenter[zet_id][k]-0.03)<aoq_pre && aoq_pre<(aoqCenter[zet_id][k]+0.03))
			{
				aoq=aoqCenter[zet_id][k];
				aoq_id=k;
			}
		}
		if (zet==0 || aoq == 0) {cout<<"This ellipse doesn't exist. Try different Z and A values."<<endl;}
		else
		{
			e1->Delete();
			all->cd(1); // select canvas
			zc = zetCenter[zet_id];
			zSigma[zet_id][aoq_id]=zSigma2[zet_id][aoq_id]+rz_inc;  // overwrite the old radius with a new radius
			zs = zSigma2[zet_id][aoq_id]+rz_inc;
			ac = aoqCenter[zet_id][aoq_id];
			aoqSigma[zet_id][aoq_id]=aoqSigma2[zet_id][aoq_id]+ra_inc;
			as = aoqSigma2[zet_id][aoq_id]+ra_inc;
			TEllipse *e1 = new TEllipse(ac,zc,as,zs,0,360,0);  // draw the updated green ellipse
			e1->SetLineColor(3);
			e1->SetLineWidth(2);
			e1->SetFillStyle(0);
			e1->Draw();
			all->Update();
			cout<<"done"<<endl;
			zet=0;
			aoq=0;
		}
		z_data=zet_pre;
	}
	ra_inc = 0.0;
	rz_inc=0.0;
}
////////////////////////////////////////////////////////////////////////////////
/// Plot all ellipses for all Z values and AOQ values that were identified during the peak search. 
void ellipse::ellipseAllHere()
{
	all = fEcanvas4->GetCanvas();
	int size =0;
	for (unsigned int k=0;k<mean.size();k++)
	{
		size += mean[k].size();
	}
	TEllipse *ellipses[size];   // create an array of ellipses
	int index = 1;
	for (unsigned int n=0;n<aoqCenter.size();n++)
	{
		float zc = zetCenter[n];
		float zs = zetSigma[n];
		for (unsigned int p=0;p<aoqCenter[n].size();p++)
		{
			float ac = aoqCenter[n][p];
			float as = aoqSigma[n][p];
			ellipses[p] = new TEllipse(ac,zc,as,zs,0,360,0);
			ellipses[p]->SetLineColor(2);
			ellipses[p]->SetLineWidth(2);
			ellipses[p]->SetFillStyle(0);
			ellipses[p]->Draw();
			index++;
			all->Update();
		}
	}
	cout<<"done"<<endl;
}
std::vector<std::vector<float>> data;
////////////////////////////////////////////////////////////////////////////////
/// Export the information about the ellipses as a .txt file.
/// If the user inputted a custom name, that name will be used. Otherwise, current date and time will be used to generate a name of the file.
void ellipse::saveTXT()
{
	// creating a vector to store all retrieved parameters	
	// zet a z_center a_center z_radius a_radius
	for (unsigned int k=0;k<zetCenter.size();k++)
	{
		for (unsigned int p=0;p<aoqCenter[k].size();p++)
		{
			std::vector<float> temp;
			
			temp.push_back(round(zetCenter[k]));
			float n = zetCenter[k]*(aoqCenter[k][p]-1); //finding the number of neutrons
			temp.push_back(round(n)+round(zetCenter[k]));
			temp.push_back(zetCenter[k]);
			temp.push_back(aoqCenter[k][p]);
			temp.push_back(zSigma[k][p]);
			temp.push_back(aoqSigma[k][p]);
			data.push_back(temp);
			temp.clear();
		}
	}

	// saving the data as a .txt file as a table:
	// zet a z_center aoq_center z_radius aoq_radius
	std::ofstream file;
	const char *name_temp = text->GetText();
	const char *if_st = "0";
	cout<<name_temp<<endl;
	const char *t = ".txt";
	TDatime fToday;
	if (strcmp(name_temp,if_st)==0) 
	{
		Int_t date = fToday.GetDate(); 
		Int_t time = fToday.GetTime();
		cout<<"Date and time: "<<date<<" "<<time<<endl; 
		const char *name = Form("%d_%d.txt",date,time);
		file.open(name);
	}
	else 
	{
		string temp = string(name_temp)+string(t);
		const char *name = temp.c_str();
		cout<<"File name: "<<name<<endl;
		file.open(name);
	}
	file<<"Z, A, z center, aoq center, z radius, aoq radius \n";
	for (unsigned int n=0;n<data.size();n++)
	{
		for (unsigned int k=0;k<data[n].size();k++)
		{
			file << data[n][k] << " ";
		}
		file << "\n";
	}
	file.close();

	cout<<"data saved"<<endl;
}
////////////////////////////////////////////////////////////////////////////////
/// input class destructor
input::~input()
{
	fMain->Cleanup();
	delete fMain;
}
////////////////////////////////////////////////////////////////////////////////
/// zetSearch class destructor.
zetSearch::~zetSearch()
{
	fMain->Cleanup();
	delete fMain;
}
////////////////////////////////////////////////////////////////////////////////
/// Fits class destructor.
Fits::~Fits()
{
	fMain->Cleanup();
	delete fMain;
}
////////////////////////////////////////////////////////////////////////////////
/// aoqSearch class destructor.
aoqSearch::~aoqSearch()
{
	fMain->Cleanup();
	delete fMain;
}

////////////////////////////////////////////////////////////////////////////////
/// ellipse class destructor
ellipse::~ellipse()
{
	fMain->Cleanup();
	delete fMain;
}
////////////////////////////////////////////////////////////////////////////////
/// This is a mathematical expression for the gaussian curve that will be used to fit the peaks. It uses a linear background.
Double_t gausbkg(Double_t *x, Double_t *par)
{
        Double_t fun = par[0]*TMath::Exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2]));// +bkg(x,par);
        return fun;
// p0 height of the peak; p1 center of the curve; p2 sigma; p4 slope of the background; p3 offset (y-shift of the lower end of the gaussian);; before these, the first p0 is the y value before and p0 is the y value after hte gaussian curve
}
////////////////////////////////////////////////////////////////////////////////
/// Linear background used by the gausbkg.
Double_t bkg(Double_t *x, Double_t *par)
{
        Double_t bkgval;
        bkgval = par[3] + par[4] * x[0];
        return bkgval;
}
////////////////////////////////////////////////////////////////////////////////
/// Initiate the program. IMPORTANT: DO NOT EDIT THIS FUNCTION.
int main(int argc, char *argv[])
{
	TApplication theApp("App", &argc, argv);
	std::cout << "Launching... " << std::endl;
	new input(gClient->GetRoot(),200,200);
	theApp.Run();
	return 0;
}