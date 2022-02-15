#ifndef ADDITIVEMATHELEMENTS_HPP
#define ADDITIVEMATHELEMENTS_HPP

#include <iostream>
#include <vector>

#include "QDebug"
#include "/usr/local/include/eigen3/Eigen/Dense"
#include "math.h"

class Dot2D
{
private:
    double xCoordinate, yCoordinate;

public:
    double getXCoordinate();
    void setXCoordinate(double value);

    double getYCoordinate();
    void setYCoordinate(double value);
};

class Dot3D
{
private:
    double xCoordinate, yCoordinate, zCoordinate;

public:
    Dot3D(){};
    Dot3D(double xCoord_, double yCoord_, double zCoord_)
    : xCoordinate(xCoord_), yCoordinate(yCoord_), zCoordinate(zCoord_)
    {
    }

    double getXCoordinate() const;
    void setXCoordinate(double value);

    double getYCoordinate() const;
    void setYCoordinate(double value);

    double getZCoordinate() const;
    void setZCoordinate(double value);

    void scale(double coef, double xCenter, double yCenter, double zCenter);
    void move(double xDif, double yDif, double zDif);
    void rotateX(double angle, double xCenter, double yCenter, double zCenter);
    void rotateY(double angle, double xCenter, double yCenter, double zCenter);
    void rotateZ(double angle, double xCenter, double yCenter, double zCenter);

    friend std::ostream &operator<<(std::ostream &os, const Dot3D &dot);
    friend QDebug operator<<(QDebug deb, const Dot3D &dot);
};

class MathVec
{
private:
    double xInc, yInc, zInc;

public:
    double getXInc();
    void setXInc(double value);

    double getYInc();
    void setYInc(double value);

    double getZInc();
    void setZInc(double value);
};

#endif // ADDITIVEMATHELEMENTS_HPP
