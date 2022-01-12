////////////////////////////////////////////////
//Author: Sophia Devinyak (s.devinyak@gmail.com , sdevinya@uwaterloo.ca)
//Date created: November 2021 at TRIUMF under the supervision of Dr. Roger Caballero-Folch

READ ME


BEFORE USAGE
This program can only be run through the terminal. It cannot be launched by double clicking. root6 must be started before launching the program.

Whenever EllipseCutter is downloaded for the first time, or moved to a different directory, the user must first run the CompileEllipseCutter.sh file to make sure that there are no errors when using the program. Whenever the user sees the following error, please run the CompileEllipseCutter.sh script before using the EllipseCutter:

Error in cling::AutoLoadingVisitor::InsertIntoAutoLoadingState:
   Missing FileEntry for input.h
   requested to autoload type input

This script can be run by typing out ./CompileEllipseCutter.sh in the terminal.


PREPARING THE FILE
This program analyzes a 2D histogram, where y-axis is the Z value, and x-axis is aoq value. Save your TCanvas containing the TH2D* histogram as a .root file before using EllipseCutter. 


SEARCHING FOR ZET/AOQ PEAKS
Very likely, the program will identify extra "peaks", that are not actually peaks that the scientist is looking for. The user has the ability to remove such peaks. Simply use the number entry field or arrows beside the field to select the peak, and then delete it. Selected peaks turn black, and deleted peaks turn red. The red peaks will NOT be fitted in the next step of fitting zet/aoq peaks.
	NOTE: if faulty peaks are not removed, the user may run into problems when analyzing their data later.
Do NOT delete all peaks on a subcanvas in AoQ search window. Instead, delete the Z peak corresponding to that subcanvas when searching for Z peaks. 

If NOT ALL PEAKS were identified, the user would need to change the sensitivity of the peak search. For that, change the value of the lastPar variable in EllipseCutter.C . This parameter should be approximately (smallest peak height)/(tallest peak height), in other words, the smallest the value of lastPar, the higher the sensitivity. Compile the code using the CompileEllipseCutter.sh script. This parameter is used in both zetSearch and aoqSearch class. Depending on which search was not successful, the user should decide which one to change. Alternatively, both parameters could be changed. 

PLOTTING ELLIPSES
1. Plot All Ellipses on This Canvas (this is done automatically).
2. Edit ellipses:
	i. enter the # of neutrons and select the element that you want to edit;
	ii. IMPORTANT: click "Select This Ellipse to Edit". Selected ellipse will turn green;
	iii. a) increase or decrease the zet and aoq radius;
	iii. b) to delete an ellipse, click "Delete This Ellipse". Deleted ellipse will turn black.
3. When the user is happy with the results, click "Save Approved Ellipses" to save z, a, center z, center aoq, radius z and radius aoq values in a .txt file.
	Note: the red ellipses that were plotted in step 1 will stay on the canvas, even as there will be green ellipses added to edit. However, only the latest change to the radius will be remembered.

SAVING DATA
If the path entered is wrong, the data will not be saved.
