# Theatrical Spatial Audio Suite
www.stewartblackwood.com
<br>
A suite of object based spatial tools & algorithms implemented in C++ (with the JUCE SDK), Javascript, and Max/MSP. 

# Included
Create Trajectories - XYZ plugin that sends OSC over a desired port. 
<br>
Raw Javascript - contains Knn, DBAP, VBAP & VBIP algorithms implemented in Javascript. 
<br>
SpatialSuite - a max project with all of the algorithms setup for use in quad.
<br>
2DAmbisonic - An implementation of a 2D ambisonic mic array encoder/decorder. 

# Thank You 
To Tom Erbe, Bobby McElver, and the Norman Design Fund for assistance while researching and creating this.
<br>

# How To Use The Spatial Algorthims
This is as a repository of open source spatial algorithms and tools. Generally the best way to explore this is to download the entire Github repo and open up the Max/MSP project entitled Spatial Suite. This has a number of Javascript objects that house an implementation of spatial algorithms. If you double click on them, the Max/MSP javascript texteditor will display the full implementation.   
<br> 
Inside the javascript object there are some usage notes and a link to an academic paper(s) that pertain to that specific algorithm. I truly hope that this empowers you explore the interesting world of spatial sound.   
<br>
As a quick note, to understand the papers, there is a some trigonometry, linear alegbra, and a bit of calculus knowledge assumed by the authors. A smart starting point would be to create a position based panner and then build from there.

# How To Use The Spatial Controller (Entitled "Create Trajecories")
You can go to this link and read all about it: https://blackwoodsounddesign.gumroad.com/l/xxyvSs 
<br>
This is were I'm housing the source code for this plugin. Feel free to use it for yourself were ever you would like. For more information on JUCE. You can follow this link: https://juce.com/ 
