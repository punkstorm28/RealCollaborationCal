//
// Created by rob-ot on 13.3.19.
//

#include <ur3-livemodel/headers/RobotPart.h>

#include "ur3-livemodel/headers/Artifact.h"


Artifact::Artifact(const std::string &ojectStlFile, float partRadius, float partLength) : RobotPart() {
    setOjectStlFile(ojectStlFile);
    setPartRadius(partRadius);
    setPartLength(partLength);
}


const std::string &Artifact::getOjectStlFile() const {
    return ojectStlFile;
}

void Artifact::setOjectStlFile(const std::string &ojectStlFile) {
    Artifact::ojectStlFile = ojectStlFile;
}

const Eigen::Vector3d &Artifact::getWorldTranslation() const {
    return worldTranslation;
}

const Eigen::Vector3d &Artifact::getWorldRotationRpy() const {
    return worldRotationRpy;
}

float Artifact::getPartRadius() const {
    return partRadius;
}

void Artifact::setPartRadius(float partRadius) {
    Artifact::partRadius = partRadius;
}

float Artifact::getPartLength() const {
    return partLength;
}

void Artifact::setPartLength(float partLength) {
    Artifact::partLength = partLength;
}

const Eigen::Matrix4d &RobotPart::getWorldTransformation() const {
    return worldTransformation;
}

const Eigen::Vector3d &RobotPart::getWorldTranslation() const {
    return worldTranslation;
}

const Eigen::Vector3d &RobotPart::getWorldRotationRpy() const {
    return worldRotationRpy;
}

void Artifact::computeWorldTransform() {
    //Use the data to populate the affine transformation Matrix
    Eigen::Affine3d currentTransformation = Eigen::Affine3d::Identity();

    Eigen::AngleAxisd rollAngle(worldRotationRpy[0], Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd pitchAngle(worldRotationRpy[1], Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd yawAngle(worldRotationRpy[2], Eigen::Vector3d::UnitZ());

    Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;

    Eigen::Matrix3d rotationMatrix = q.matrix();
    currentTransformation.prerotate(rotationMatrix);
    currentTransformation.translate(getWorldTranslation());

    worldTransformation = currentTransformation.matrix();
}