//
// Created by rob-ot on 18.3.19.
//

#include <ur3-livemodel/headers/RobotModelImpl.h>
#include <ur3-livemodel/headers/Artifact.h>
#include <Eigen/Dense>
#include <ur3-livemodel/headers/Joint.h>
#include "ur3-livemodel/headers/RobotModelImpl.h"
#include <pcl/kdtree/kdtree_flann.h>

using namespace Eigen;

RobotModelImpl::RobotModelImpl() {
    initializeRobot();
}
void RobotModelImpl::initializeRobot() {
    std::vector<RobotPart*>* partPointer = currentRobotState.getPartsInSequence();

    int jointIndex = 1;
    Artifact* baseArtifact =
            new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_base.stl",
                    partPointer, 0.075, 0.038, 0);
    baseArtifact->setPartName("baseArtifact");
    baseArtifact->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    baseArtifact->setWorldTranslation(Vector3d(0.0, 0.0, 0.0));

    Artifact* shoulderLink =  new Artifact("//Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_shoulder.stl"
            , partPointer, 0.075, 0.178, 2);
    shoulderLink->setPartName("shoulderLink");
    shoulderLink->setWorldRotationRpy(Vector3d(0, 0, 0));
    shoulderLink->setWorldTranslation(Eigen::Vector3d(0, 0, 0));

    Joint* shoulderPanJoint = new Joint(baseArtifact, shoulderLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 1);
    shoulderPanJoint->setWorldRotationRpy(Eigen::Vector3d(0, 0, 0));
    shoulderPanJoint->setRotationAxis(Eigen::Vector3d(0, 0, 1));
    shoulderPanJoint->setWorldTranslation(Eigen::Vector3d(0, 0, 0.085));

    currentRobotState.addPart(baseArtifact);
    currentRobotState.addPart(shoulderPanJoint);
    currentRobotState.addPart(shoulderLink);

    Artifact* upperArmLink = new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_upperarm.stl",
            partPointer, 0.075, 0.24365, 4);
    upperArmLink->setPartName("upperArmLink");

    upperArmLink->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    upperArmLink->setWorldTranslation(Vector3d(0.0, 0.0, 0.0));

    Joint* shoulderLiftJoint = new Joint(shoulderLink, upperArmLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 3);
    shoulderLiftJoint->setWorldRotationRpy(Eigen::Vector3d(0, 0, 0));
    shoulderLiftJoint->setRotationAxis(Eigen::Vector3d(0, 1, 0));
    shoulderLiftJoint->setWorldTranslation(Eigen::Vector3d(0, 0.054, 0.152));


    currentRobotState.addPart(shoulderLiftJoint);
    currentRobotState.addPart(upperArmLink);

    Artifact *forearmLink = new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_forearm.stl",
                                         partPointer, 0.075, 0.21325, 6);
    forearmLink->setPartName("forearmLink");
    forearmLink->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    forearmLink->setWorldTranslation(Vector3d(0.0, 0.0, 0));


    Joint* elbowJoint = new Joint(upperArmLink, forearmLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 5);
    elbowJoint->setWorldRotationRpy(Eigen::Vector3d(0.0, 0.0, 0.0));
    elbowJoint->setRotationAxis(Eigen::Vector3d(0, 1, 0));
    elbowJoint->setWorldTranslation(Eigen::Vector3d(0, 0.06, 0.395));


    currentRobotState.addPart(elbowJoint);
    currentRobotState.addPart(forearmLink);

    Artifact* wrist1Link = new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_wrist1.stl",
                                        partPointer, 0.075, 0.12, 8);    //TODO: fix the numbers
    wrist1Link->setPartName("wrist1Link");
    wrist1Link->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    wrist1Link->setWorldTranslation(Vector3d(0.0, 0.0,0));

    Joint* wrist1Joint = new Joint(upperArmLink, forearmLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 7);
    wrist1Joint->setWorldRotationRpy(Eigen::Vector3d(0, 0, 0));
    wrist1Joint->setRotationAxis(Eigen::Vector3d(0, 1, 0));
    wrist1Joint->setWorldTranslation(Eigen::Vector3d(0, 0.06, 0.608));



    currentRobotState.addPart(wrist1Joint);
    currentRobotState.addPart(wrist1Link);
    jointIndex = jointIndex + 2;
    jointIndexKeeper.insert(std::make_pair(jointIndex, wrist1Joint));

    Artifact* wrist2Link = new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_wrist2.stl",
                                        partPointer, 0.075, 0.12, 10);
    wrist2Link->setPartName("wrist2Link");
    wrist2Link->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    wrist2Link->setWorldTranslation(Vector3d(0.0, 0.0,0));

    Joint* wrist2Joint = new Joint(upperArmLink, forearmLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 9);
    wrist2Joint->setWorldRotationRpy(Eigen::Vector3d(0, 0, 0));
    wrist2Joint->setRotationAxis(Eigen::Vector3d(0, 0, 1));
    wrist2Joint->setWorldTranslation(Eigen::Vector3d(0, 0.106, 0.648));


    currentRobotState.addPart(wrist2Joint);
    currentRobotState.addPart(wrist2Link);
    jointIndex = jointIndex + 2;
    jointIndexKeeper.insert(std::make_pair(jointIndex, wrist2Joint));

    Artifact* wrist3Link = new Artifact("/Users/vyomkesh/XcodeProjects/RealCollaborationCal/ur3-livemodel/ur3stl/z_wrist3.stl", partPointer,
            0.075, 0.12, 12);
    wrist3Link->setPartName("wrist3Link");
    wrist3Link->setWorldRotationRpy(Vector3d(0.0, 0.0, 0.0));
    wrist3Link->setWorldTranslation(Vector3d(0.0, 0.0,0));

    Joint* wrist3Joint = new Joint(upperArmLink, forearmLink, partPointer, -2.0 * PI, +2.0 * PI, 0, 11);
    wrist3Joint->setWorldRotationRpy(Eigen::Vector3d(0, 0, 0));
    wrist3Joint->setRotationAxis(Eigen::Vector3d(0, 1, 0));
    wrist3Joint->setWorldTranslation(Eigen::Vector3d(0, 0.147, 0.693));


    currentRobotState.addPart(wrist3Joint);
    currentRobotState.addPart(wrist3Link);
    jointIndex = jointIndex + 2;
    jointIndexKeeper.insert(std::make_pair(jointIndex, wrist3Joint));



    baseArtifact->computeWorldTransformation();
    shoulderPanJoint->computeWorldTransformation();

    shoulderLink->computeWorldTransformation();
    shoulderLiftJoint->computeWorldTransformation();
    upperArmLink->computeWorldTransformation();
    elbowJoint->computeWorldTransformation();
    forearmLink->computeWorldTransformation();
    wrist1Joint->computeWorldTransformation();
    wrist1Link->computeWorldTransformation();
    wrist2Joint->computeWorldTransformation();
    wrist2Link->computeWorldTransformation();
    wrist3Joint->computeWorldTransformation();
    wrist3Link->computeWorldTransformation();

    currentRobotState.rotateAtJoint(1, PI/3);

    //currentRobotState.rotateAtJoint(1, -PI/3);
}


 void RobotModelImpl::rotateAtJoint(int jointIndex, double angle) {
     currentRobotState.rotateAtJoint(jointIndex, angle);
 }

std::vector<RobotPart*>* RobotModelImpl::getPartsInSpace() {
    return currentRobotState.getPartsInSequence();
}

/*
void RobotModelImpl::filterRobotFromPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr inputPc) {
    float filterDist = 0.07;
    pcl::KdTreeFLANN<pcl::PointXYZRGB> kdtree;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr completeRobot(new pcl::PointCloud<pcl::PointXYZRGB>());

    for (int i = 0; i < getPartsInSpace()->size(); i++) {
        RobotPart* part = getPartsInSpace()->at(i);
        auto artifact = dynamic_cast<Artifact *>(part);
        if(artifact != nullptr) {

            PolygonMesh inMesh = part-
            pcl::PointCloud<pcl::PointXYZ> cloud;
            pcl::fromPCLPointCloud2(inMesh->cloud, cloud);
            pcl::transformPointCloud(cloud, cloud, transform);
            pcl::toPCLPointCloud2(cloud, inMesh->cloud);



        }
    }
}
*/


