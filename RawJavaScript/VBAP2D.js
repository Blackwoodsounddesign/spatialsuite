/*
Written by Stewart Blackwood 
www.stewartblackwood.com

this is an 2D implementation of Ville Pulkki's Vector Based Amplitude Panning algorithm (VBAP).
The original paper can be found here: https://www.aes.org/e-lib/browse.cfm?elib=7853

====== STEPS FOR USE ======
Step 1) Add the azimuth value for each speaker after the VBAP.js 
(the number of outlets are based on the number of azimuths you include)

Step 2) connect the outlets of VBAP.js to inlets on various *~ objects 

Step 3) add your source to the other inlets of the *~ objects 

Step 4) connect a float based control scheme to the inlet of the VBAP object. 
VBAP.js will assume the control signal is in degrees and pan around your space based on the values you provide.

Step 5) Enjoy! 

===== QUICK NOTE ===== 
Make sure to add some smoothing to the values of VBAP.js 
*/

var SpeakerDegrees = []; 
var PossibleValues = [];
var SpeakerValues = [];
var SourcePlacement; 

//Set up our outlets
if (jsarguments.length > 0)
	{
		SpeakerDegrees = [];
		
		for(i = 0; i < jsarguments.length; i++)
		{	
			if(beginsWithFloat(jsarguments[i]))
			{
				SpeakerDegrees.push(jsarguments[i]);
			}
		}
		
		outlets = SpeakerDegrees.length;
	}

//when we get a float we treat it as an azimuth value
function msg_float(theta)
{
	//wrap the degrees around
	SourcePlacement = theta % 360;
	
	if(!ExactCheck())
	{
	SourcePositionCheck(SourcePlacement);
	}
}

//this checks what values the source position is in-between
function SourcePositionCheck(theta)
{
	PossibleValues = [];
	
	for(i = 0; i < SpeakerDegrees.length; i++)
	{
		speakerPos = SpeakerDegrees[i];
			
			for(j = 0; j < SpeakerDegrees.length; j++)
			{
				speakerPos2 = SpeakerDegrees[j];
				
				if(speakerPos < theta && theta < speakerPos2)
					{
						PossibleValues.push(speakerPos, speakerPos2);
					}
			}
	} 
	
	if(PossibleValues.length > 1)
		{
			PossibleValueCheck();
		}
		else
		{
			ZeroGainValues();
			PossibleValues.push(Math.max.apply(Math, SpeakerDegrees), Math.min.apply(Math, SpeakerDegrees));
			PossibleValueCheck();
		}	
}	

//this checks if the source is exactly at one of the speakers. 
//If so it only plays it out of that speaker
function ExactCheck()
{
	for(i = 0; i < SpeakerDegrees.length; i++)
	{
		if(SourcePlacement == SpeakerDegrees[i])
		{
			output1 = SpeakerDegrees.indexOf(SourcePlacement);
			g1 = 1;
			
			ZeroGainValues();
	
			outlet(output1, g1);
			return true; 
		}
	}
	
	return false;
}

//this checks to see which two speakers the source is between
function PossibleValueCheck()
{
	activeNum = 1000;
	Speaker1 = 0;
	Speaker2 = 0;
	
	for(i = 0; i < PossibleValues.length; i = i +2)
	{	
		curr = PossibleValues[i+1] - PossibleValues[i];
		
		if(curr <= activeNum)
		{
			activeNum = curr;
			Speaker1 = PossibleValues[i];
			Speaker2 = PossibleValues[i+1];
		}
	}
	
	SpeakerValues = [];
	
	SpeakerValues.push(Speaker1);
	SpeakerValues.push(Speaker2);
	
	if(SpeakerValues.length > 0)
	{
		UpdateGainCoeff();
	}
}

//this is where the VBAP forumla lives
function UpdateGainCoeff()
{
	//position of the source
	p1 = Math.cos(degrees_to_radians(SourcePlacement));
	p2 = Math.sin(degrees_to_radians(SourcePlacement));
	
	//first speaker location
	x1 = Math.cos(degrees_to_radians(SpeakerValues[0]));
	y1 = Math.sin(degrees_to_radians(SpeakerValues[0]));
	
	//second speaker location
	x2 = Math.cos(degrees_to_radians(SpeakerValues[1]));
	y2 = Math.sin(degrees_to_radians(SpeakerValues[1]));
	
	//determinate for the inverse of the 2x2 speaker matrix
	det = 1/(x1 * y2 - x2 * y1);
	
	//speaker gain weights
	g1 = p1*det*y2 - p2*det*x2;
	g2 = p2*det*x1 - p1*det*y1;
	
	//these are just in case anything crazy happens
	if(g1 < 0)
	g1=0
	
	if(g2 < 0)
	g2=0 
	
	if(g1 > 1)
	g1=1
	
	if(g2 > 1)
	g2=1 
	
	//finds the outlet connected to the speakers we are panning between
	output1 = SpeakerDegrees.indexOf(SpeakerValues[0]);
	output2 = SpeakerDegrees.indexOf(SpeakerValues[1]);
	
	//zero out all the other gain values
	ZeroGainValues();
	
	//set our two two favorite ones to our calculate gain weights
	outlet(output1, g1);
	outlet(output2, g2); 
}

//these are various quick helper functions
function degrees_to_radians(degrees)
{
  var pi = Math.PI;
  return degrees * (pi/180);
}

function beginsWithFloat(val) 
{
  val = parseFloat(val);
  return ! isNaN(val);
}

function ZeroGainValues()
{
	for(i = 0; i < SpeakerDegrees.length; i++)
	{
		outlet(i, 0);
	}
}