# Real Collaboration Cal
## Live obstacle tracking and collision prediction in collaborative robot workspaces.


### Required Libraries:
1. Realsense 2 (Intel Realsense library for the realsense cameras).
2. PCL 1.9.1 (Built with visualisation support)
3. OpenCV 4
4. Qt 5
5. VTK 8.1

## Introduction:
Collaborative robots are supposed to work right alongside their human counterparts, although safety remains
a concern while considering the high speeds (high momentum) the robots may be working at and any collision with the
human worker therefore has the potential to hurt. The current safetly methods employed are usually boundaries imposed
on the workspace and whenever somebody crosses the boundary, the robot responds by either slowing down or stopping, other
methods include force sensors to detect collision and stop the robot.
This project attempts to find a more precise way of predicting such a collision event by live scanning the workspace
of the robot using multiple Intel Realsense D435 cameras (Multiple to get a better scan), combining the data to one scan
in pointcloud form with reference at the robot's origin, segementing the point-cloud to recognise stray objects from the
robot and using this information along with robot's current position and trajectory to predict the collision event in future.



## Steps:

### 1. Calibration:
      * Aruco Based Calibration
      * GUI Based Manual Calibration

#### Aruco Based Calibration:

This method employs aruco markers placed alongside every camera and using another camera (Calibration camera), mounted
at the robot's TCP, the first step involves calibrating the calibration camera to get it's intrinsics, then we use the
intrinsics with open CV to calculate the pose of the aruco tag (also the pose of the corresponding camera) with respect
to the calibration camera. We know the transformation from the calibration camera frame to the robot's origin by using
the TCP pose data provided to us by the robot. We further combine the transformation to get the transformation from the
camera's perspective to robot's base coordinates.

##### Code Explanation:
[RealsensePoseEstimation.h] (https://github.com/vyomkeshj/RealCollaborationCal/blob/qt_ui/headers/RealsensePoseEstimation.h)

```
/**
* Gets the transformation from aruco to calibration camera given the calibration camera id
* @Param calibcamId
*/
tuple<Vec3d, Vec3d> visualizeDetectedAxesAndReturnTransformation(String calibcamId) {
Vec3d rvec, tvec;
while(true) {
Mat imageCopy;
realsenseManager.getConnectedDeviceIds();
auto[image, depth_information, video_frame] = this->realsenseManager.getCVAlignedMatrix(calibcamId); //Returns aligned video frame and depth frame

double tick = (double)getTickCount();

vector< int > markerIds;
vector< vector< Point2f > > markerCorners, rejectedMarkers; //To store the detected marker corners and rejected markers

// detect markers in the image
aruco::detectMarkers(image, dictionary, markerCorners, markerIds, detectorParams,
rejectedMarkers);

double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
totalTime += currentTime;
totalIterations++;
if(totalIterations % 30 == 0) {
cout << "Detection Time = " << currentTime * 1000 << " ms "
<< "(Mean = " << 1000 * totalTime / double(totalIterations) << " ms)" << endl;
}

// draw results
image.copyTo(imageCopy);
if(markerIds.size() > 0) {
aruco::drawDetectedMarkers(imageCopy, markerCorners);
}
if(showRejected && rejectedMarkers.size() > 0)
aruco::drawDetectedMarkers(imageCopy, rejectedMarkers, noArray(), Scalar(100, 0, 255));

for (auto& currentMarker: markerCorners) {
calculateArucoPose(0.09, currentMarker, rvec, tvec, depth_information);   //estimate pose for the aruco marker
}

aruco::drawAxis(imageCopy, camMatrix, distCoeffs, rvec, tvec, 0.1);
//getEstimatedPose();
//printMatrix(rvec);
//applyColorMap(imageCopy, imageCopy, COLORMAP_JET);
imshow("Pose Estimator", imageCopy);    //visualize result
char key = (char)waitKey(waitTime);
if(key == 27) break;
}
return std::make_tuple(rvec, tvec);
}```


##### Learnings:
1. [Using a single aruco marker to estimate the pose of the camera is inappropriate] (https://stackoverflow.com/questions/51709522/unstable-values-in-aruco-pose-estimation?noredirect=1&lq=1)
2. Aruco marker detection requires a brightly lit environment.

##### Next:

1. Use multiple markers to improve detection accuracy.