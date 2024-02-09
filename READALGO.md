<h1 align="center">
	ALGORITHM
</h1>

This is just a brief explanation of the algorithm which may have already been implemented or have to implement this into the project. This file is for the developers behind this project. But if you wish to take a peek into the background, feel free...

### Phrases or words to Note :
 * coords = co-ordinates
 * interval_time = the interval at which a keyframe has to be animated
 * tracked_coords = tracked co-ordinates which are transferred from .py to .cpp
 * MP = meadiapipe

<br><br>
After getting coords from .py to .cpp, the steps are as follows :

## * Getting the interval_time

First we have to know the total_time of the video. We are getting the total_frames and fps from openCV. So the
total_time is.

	total_time = total_frames / fps

Then we can easily find the interval_time by dividing 1 by fps.

	interval_time = 1 / fps

## * Convert the tracked_coords to 3D scale :

So now, when we look into the tracked_coords, they values are actually clamped to the range of (-2, 2). It means that the values from MP must be on a different unit/scale. We have to first find out the scale and convert all of the tracked points translate through that unit/scale. (Assumption: The tracked_coords are of the scale/unit = 10)

	tracked_coords = tracked_coords * scale/unit

These coords which later can be transferred to calculate the origin.

## * Adjust the tracked_coords according to origin :

The tracked_coords from MP all have the midpoint of bone index [23] and [24] which is [left_hip] and [right_hip] respectively. But in the 3D world, the origin will be from the world origin. So we can convert the tracked_coords to the 3D world coords by taking the WorldCordinates of the [hip_bone] (the bone from imported 3D model which is assigned as the index of hip_bone position which is mapped by the user manually) and adding it with the tracked_coords.

	tracked_coords = tracked_coords + WorldCordinate( [hip_bone] )

Thus we will get the newly tracked_coords which are actually the 3D world coords which has 3D world origin.

## * Finding the relation between tracked_coords :

tracked_coords only contain the translation values. So we have to find the rotation of each bone in 3D world by only using these translation values by MP. Moreover the MP Pose Estimation uses different structure to track co-ordinates as shown below.

<img src="UI/Work/68747470733a2f2f6d65646961706970652e6465762f696d616765732f6d6f62696c652f706f73655f747261636b696e675f66756c6c5f626f64795f6c616e646d61726b732e706e67.png" alt="Icon">

##  * Calculating the Rotation :
	pass
