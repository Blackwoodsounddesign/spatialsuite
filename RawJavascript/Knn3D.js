/*
Written by Stewart Blackwood 
www.stewartblackwood.com

this is an 3D implementation of Knn-Distance Based Amplitude Panning algorithm (Knn).
The original paper which the general idea for DBAP can be found here: https://www.researchgate.net/publication/241169129_DBAP_-_Distance-Based_Amplitude_Panning#pf4

====== STEPS FOR USE ======
Step 1) Add the XY value for each speaker after the DBA32D.js 
(the number of outlets are based on the number of XYZ coordinates you include)

Step 2) connect the outlets of Knn3D.js  to inlets on various *~ objects 

Step 3) add your source to the other inlets of the *~ objects 

Step 4) connect a float based control scheme to the inlet of the knn object. 
knn.js will assume the control signal is in XYZ cooridnates and assign speakerweights around your space based on the values you provide.

Step 5) In the 6 outlet provide an active speaker number. The Knn object will now only activate that number of speakers at a time. 

Step 6) Enjoy! 

===== QUICK NOTES ===== 
Make sure to add some smoothing to the values of Knn3D.js

The spatial blur value will keep all the speakers playing (at least at some level) all of the time

dB rollof is generally 6dB to 3dB (assuming the inverse square law as a computational basis) 
Use smaller value for near-field use cases 
*/


inlets = 6;
setinletassist(0, "X Position");
setinletassist(1, "Y Position");
setinletassist(2, "Z Position");
setinletassist(3, "Spatial Blur");
setinletassist(4, "dB Rolloff");
setinletassist(5, "Active Speaker #");


Xs = 0; 
Ys = 0; 
Zs = 0; 

var SpeakersXY = [];
var Distances = [];
var d2a = [];
var vis = [];
var blurryness = 0;
var a = .9965784284661464;
var asnum = 2;   
var sortableDistances = [];

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
	
	if(inlet == 5)
	{
		if(value > SpeakersXY.length / 3)
		error("You have activated a number of speakers greater than you have entered. This will preform standard DBAP.")
		else if (value > 0)
		asnum = value;
		else
		error("Please enter a number greater than 0")
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
	sortableDistances = [];
	d2a = [];
	vis = [];
	
	//loops over the array for each set of coordinates
	for(i = 0; i < SpeakersXY.length; i = i + 3)
	{
		X1 = SpeakersXY[i];
		Y1 = SpeakersXY[i+1];
		Z1 = SpeakersXY[i+2];
		d = Math.sqrt((X1 - Xs) * (X1 - Xs) + (Y1 - Ys) * (Y1 - Ys) + (Z1 - Zs) * (Z1 - Zs) + blurryness * blurryness);
		Distances.push(d);
		sortableDistances.push(d);
	}
	
	// sorts the distances, and then creates an array of the smallest
	var sortedDistances = sortThis(sortableDistances);
	closestasnum = sortedDistances.slice(0,(asnum));
	
	//this creates an array of the outlet numbers
	var outletind = [];	
	for (m = 0; m < asnum; m++)
	{
		for (q = 0; q < Distances.length; q++){
			if(closestasnum[m] == Distances[q])
				{
					outletind.push(q);
				}
		}
	}
	
	//local interim variable that works through the Sigma notation in the DBAP formula 
	kint = 0;
	
	for(j = 0; j < closestasnum.length; j++)
	{
		kint += 1 / Math.pow(closestasnum[j],(2*a));
		d2a.push(Math.pow(closestasnum[j], a));
	}
	
	//calculate the k value 
	k = 1 / Math.sqrt(kint);
	
	ZeroGainValues(); 
	
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
		
		outlet(outletind[l],vis[l]);
	}
}


//helper functions
function beginsWithFloat(val) 
{
  val = parseFloat(val);
  return ! isNaN(val);
}

function sortThis(array)
{
   array2 = array.sort(function(a, b){return a - b}); 
   return array2;
}

function ZeroGainValues()
{
	for(i = 0; i < Distances.length; i++)
	{
		outlet(i, 0);
	}
}