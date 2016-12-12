To install in Celestia-put the 3ds model file in your 'models' folder,and Jpg textures in your 'medres' folder.
Then you will have to edit 'solarsys.ssc' to look like the following-


"Hubble" "Sol/Earth"
{
	Class "spacecraft"
	Mesh "bigHubble.3ds"
	Radius 0.007                  # 14 meters across
	Beginning         2448007     # Deployed 25 April 1990
	# Ending 2010?
        Color [ 0 0 0 ]
	EllipticalOrbit {
		Period          0.06903
		SemiMajorAxis   6767
		Eccentricity    0.0012082
		Inclination      28.4675
		AscendingNode    51.0100
		ArgOfPericenter  27.0378
		MeanAnomaly     333.0836
		Epoch           2452028.18381755
	}

	Albedo       0.10
}

The model came from  http://hubblesource.stsci.edu/sources/modelbuilders/ originally though it had a lot of holes
that needed to be fixed.I'm sure there are still a few left but not so noticeable.The textures I made myself
except for hubsolar.jpg,which was made by Terrier (Celestia forum) I think.
Any problems Email me jestr@ntlworld.com