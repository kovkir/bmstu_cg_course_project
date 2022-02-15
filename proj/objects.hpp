#ifndef OBJS_HPP
#define OBJS_HPP

#include "additivemathelements.hpp"
#include <iostream>
#include <vector>

#include "placechooser.h"

class Vertex
{
public:
    Vertex() {}
    Vertex(Dot3D &position_, std::vector<size_t> &usedFacets_)
        : position(position_), usedFacets(usedFacets_) { }

    Vertex(Dot3D &position_) : position(position_) { }

    const Dot3D &getPosition() const;
    void setPosition(Dot3D &position_);
    const std::vector<size_t> getUsedFacets();
    void setUsedFacets(std::vector<size_t> usedFacets_);

private:
    Dot3D position;
    std::vector<size_t> usedFacets;
};

class Facet
{
public:
    Facet() {}
    Facet(std::vector<size_t> usedVertices_) : usedVertices(usedVertices_) {}

    const std::vector<size_t> getUsedVertices();
    void setUsedVertices(std::vector<size_t> usedVertices_);

private:
    std::vector<size_t> usedVertices;
};

class PolModel
{
public:
    PolModel() {}
    PolModel(std::vector<Vertex> vertices_, std::vector<Facet> facets_)
        : vertices(vertices_), facets(facets_) { }

    PolModel(std::vector<Vertex> vertices_, std::vector<Facet> facets_, QString name_)
        : vertices(vertices_), facets(facets_), modelName(name_) { }

    ~PolModel() {}

    const std::vector<Vertex> getVertices();
    void setVertices(std::vector<Vertex> &vertices_);
    const std::vector<Facet> getFacets();
    void setFacets(std::vector<Facet> facets_);

    void moveTo(int newXCell, int newYCell);
    void rotateZ(int angle);

    void setUsedCell(int xCell_, int yCell_);
    int getUsedXCell();
    int getUsedYCell();

    void setWidthModel(int widthModel_);
    int  getWidthModel();
    void setHeightModel(int heightModel_);
    int  getHeightModel();
    void setHouseHeight(int houseHeight_);
    int  getHouseHeight();

    void setModelNum(size_t modelNum_);
    size_t getModelNum();

    QString getName();
    void setName(QString modelName_);

    enum model_t
    {
        House,
        roofHouse,
        windowsHouse,
        treeFoliage,
        treeTrunk,
        roadAsphalt,
        roadStripe,
        Car,
        wheelsCar,
        glassCar
    };

    PlaceRoadChooser::checkBox getDirectionRoad();
    void setDirectionRoad(PlaceRoadChooser::checkBox directionRoad_);

    PlaceCarChooser::checkBox getDirectionCar();
    void setDirectionCar(PlaceCarChooser::checkBox directionCar_);

    void setModelType(model_t modelType_);
    model_t getModelType();

private:
    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    QString modelName;

    int xCell, yCell;
    int widthModel, heightModel;

    int houseHeight = -1;

    PlaceRoadChooser::checkBox directionRoad = PlaceRoadChooser::checkBox::XAXIS;
    PlaceCarChooser::checkBox directionCar = PlaceCarChooser::checkBox::XAXIS;

    size_t modelNum;
    model_t modelType;
};

class Illuminant
{
public:
    Illuminant(Eigen::Matrix4f &transMat);
    Illuminant(){};

    void setShadowMap(std::vector<std::vector<double>> &setShadowMap);
    std::vector<std::vector<double>> &getShadowMap();
    void clearShadowMap();

    void setAngles(int xAngle_, int yAngle_);
    int getXAngle();
    int getYAngle();

    void setTransMat(Eigen::Matrix4f &mat);
    Eigen::Matrix4f &getTransMat();

private:
    std::vector<std::vector<double>> shadowMap;
    Eigen::Matrix4f transMatrix;
    int xAngle, yAngle;
};


class CellScene
{
public:
    CellScene();
    CellScene(size_t width_, size_t height_);

    operator bool() const;

    size_t getWidth();
    size_t getHeight();

    void buildPlateModel(Dot3D startOfPlate_, Dot3D endOfPlate_);
    PolModel &getPlateModel();

    void changeSize(size_t newWidth, size_t newHeight);

    void moveUp(double value);
    void moveDown(double value);
    void moveLeft(double value);
    void moveRight(double value);

    void scale(double value);

    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);

    void toCenter();

    size_t getRealModelsNum();
    size_t getModelsNum();
    PolModel &getModel(size_t num);
    void setModel(size_t num, PolModel &newModel);
    void addModel(PolModel &model);
    void deleteModel(size_t num);

    size_t getIllumNum();
    Illuminant &getIlluminant(size_t num);
    void setIlluminant(Illuminant &illum, size_t i);
    void addIlluminant(Illuminant &illum);
    void deleteIlluminant(size_t num);

    Eigen::Matrix4f &getTransMatrix();
    void multToTrans(Eigen::Matrix4f &newTrans);

    std::vector<std::vector<size_t>> &getUsedCells();
    void initUsedCells();

    void markUsedCells(size_t num);
    void clearUsedCells(size_t num);
    void printUsedCells();

private:
    void markCellsNearHouse(int xCell, int yCell, int widthModel, int heightModel);
    void clearCellsNearHouse(int xCell, int yCell, int widthModel, int heightModel);

    void addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
                 int x1, int y1, int z1,
                 int x2, int y2, int z2,
                 int x3, int y3, int z3,
                 int x4, int y4, int z4);

    size_t width, height;

    PolModel *plateModel = nullptr;

    Eigen::Matrix4f transMatrix;

    size_t realModelsNum = 0;
    size_t modelsNum = 0;
    std::vector<PolModel> models;

    size_t illumNum = 0;
    std::vector<Illuminant> illuminants;

    Dot3D centerDot;

    std::vector<std::vector<size_t>> usedCells;
};

#endif // OBJS_HPP
