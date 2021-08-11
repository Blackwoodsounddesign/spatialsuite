/*
Written by Stewart Blackwood 
www.stewartblackwood.com

this is an 2D implementation of Trond Lossius's Distance Based Amplitude Panning algorithm (DBAP).
The original paper can be found here: https://www.researchgate.net/publication/241169129_DBAP_-_Distance-Based_Amplitude_Panning#pf4

====== STEPS FOR USE ======
Step 1) Add the XY value for each speaker after the DBA32D.js 
(the number of outlets are based on the number of XYZ coordinates you include)

Step 2) connect the outlets of DBAP3D.js  to inlets on various *~ objects 

Step 3) add your source to the other inlets of the *~ objects 

Step 4) connect a float based control scheme to the inlet of the DBAP object. 
DBAP.js will assume the control signal is in XYZ cooridnates and assign speakerweights around your space based on the values you provide.

Step 5) Enjoy! 

===== QUICK NOTES ===== 
Make sure to add some smoothing to the values of DBAP3D.js

The spatial blur value will keep all the speakers playing (at least at some level) all of the time

dB rollof is generally 6dB to 3dB (assuming the inverse square law as a computational basis) 
Use smaller value for near-field use cases 
*/


inlets = 5;
setinletassist(0, "X Position");
setinletassist(1, "Y Position");
setinletassist(2, "Z Position");
setinletassist(3, "Spatial Blur");
setinletassist(4, "dB Rolloff");


Xs = 0; 
Ys = 0; 
Zs = 0; 

var SpeakersXY = [];
var Distances = [];
var d2a = [];
var vis = [];
var blurryness = 0;
var a = .9965784284661464;  

//Set up the outlets
if (jsarguments.length > 0)
	{
		SpeakersXY = [];
		
		for(i = 0; i < jsarguments.length; i++)
		{	
			if(beginsWithFloat(jsarguments[i]))
			{
				SpeakersXY.push(jsarguments[i]);
			}
		}
		
		//make sure that there is a XY for each speaker
		if(SpeakersXY.length % 3 == 0)
		{
			outlets = (SpeakersXY.length / 3);
		}
		else
		error("Please Enter an X, Y, and Z value for each speaker.")
	}

//takes in float values to calculate the various speaker gains
function msg_float(value)
{
	if(inlet == 0)
		Xs = value;
	
	if(inlet == 1)
		Ys = value;
	
	if(inlet == 2)
		Zs = value;
	
	if(inlet == 3)
		blurryness = value;
	
	if(inlet == 4)
	{
		CalculateACoeff(value);	
	}
	
	if(SpeakersXY.length % 3 == 0)
		CalculateSpeakerGains();
		else
		error("Please Enter an X and Y value for each speaker.")
}

function CalculateACoeff(value)
{
	a = value/6.02059991328;
}

//this calculates the speaker weights based on the DBAP formula
function CalculateSpeakerGains()
{
	//clears the arrays
	Distances = []; 
	d2a = [];
	vis = [];
	
	//loops over the array for each pair of coordinates
	for(i = 0; i < SpeakersXY.length; i = i + 3)
	{
		X1 = SpeakersXY[i];
		Y1 = SpeakersXY[i+1];
		Z1 = SpeakersXY[i+2];
		d = (X1 - Xs) * (X1 - Xs) + (Y1 - Ys) * (Y1 - Ys) + (Z1 - Zs) * (Z1 - Zs) + blurryness * blurryness;
		Distances.push(d);
	}
	
	//local interim variable that works through the Sigma notation in the DBAP formula 
	kint = 0;
	
	for(j = 0; j < Distances.length; j++)
	{
		kint += 1 / Math.pow(Distances[j],(2*a));
		d2a.push(Math.pow(Distances[j], a));
	}
	
	//calculate the k value 
	k = 1 / Math.sqrt(kint);
	
	//finally we apply the various speakers weights to the our system
	for(l = 0; l < d2a.length; l++)
	{	
		vi = k/d2a[l];
		//if we divide by zero this will make the speaker that is undifined be the only one playing
		//this will cause a noticable timbral shift which is solved by including a spatial blur value
		if(isNaN(vi))
		vis.push(1);
		else
		vis.push(vi);
		
		outlet(l,vis[l]);
	}
}


//helper function
function beginsWithFloat(val) 
{
  val = parseFloat(val);
  return ! isNaN(val);
}