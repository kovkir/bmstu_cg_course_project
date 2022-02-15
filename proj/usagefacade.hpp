#ifndef USAGEFACADE_HPP
#define USAGEFACADE_HPP

#include "objects.hpp"
#include <QGraphicsScene>

#include "placechooser.h"

//#include <omp.h>

class Drawer
{
public:
    void zBufferAlg(CellScene *scene, size_t bufheight, size_t bufWidth);
    void zBufForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
        Eigen::Matrix4f &transMat, size_t color, CellScene *scene, size_t bufWidth, size_t bufHeight);
    void shadowMapForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
        Eigen::Matrix4f &transMat, Illuminant *illum, size_t bufWidth, size_t bufHeight);


    QGraphicsScene *drawScene(CellScene *scene, QRectF rect);

private:
    void interpolateColIntoShadowMap();
    void interpolateRowIntoShadowMap(std::vector<std::vector<double>> *map, int xA, int xB, double zA, double zB, int curY);
    void specBorderPut(int x, int y, double z);
    void DDABordersForPolygon(
        int xStart, int yStart, double zStart, int xEnd, int yEnd, double zEnd);

    std::vector<std::vector<double>> depthBuffer;
    std::vector<std::vector<size_t>> frameBuffer;
};

class UsageFacade
{
public:
    UsageFacade();

    void setCellScene(size_t width_, size_t height_);
    void changeCellScene(size_t newWidth, size_t newheight);
    bool isSceneSet();

    QGraphicsScene *drawScene(QRectF rect);

    int addHouse(int xCell, int yCell, int modelLength, int modelHeight, int countFloors);
    int addTree(int xCell, int yCell);
    int addRoad(int xCell, int yCell, PlaceRoadChooser::checkBox direction);
    int addCar(int xCell, int yCell, PlaceCarChooser::checkBox direction);

    void addIlluminant(int xAngle, int yAngle);

    QGraphicsScene *moveUpScene(double value, QRectF rect);
    QGraphicsScene *moveDownScene(double value, QRectF rect);
    QGraphicsScene *moveLeftScene(double value, QRectF rect);
    QGraphicsScene *moveRightScene(double value, QRectF rect);

    QGraphicsScene *scaleScene(double value, QRectF rect);

    QGraphicsScene *rotateXScene(double angle, QRectF rect);
    QGraphicsScene *rotateYScene(double angle, QRectF rect);
    QGraphicsScene *rotateZScene(double angle, QRectF rect);

    QGraphicsScene *toCenter(QRectF rect);

    CellScene *getScene();

private:     
    CellScene *scene = nullptr;
    Drawer *drawer;

    void addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
                 int x1, int y1, int z1,
                 int x2, int y2, int z2,
                 int x3, int y3, int z3,
                 int x4, int y4, int z4);
    void addTriangle(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
                     int x1, int y1, int z1,
                     int x2, int y2, int z2,
                     int x3, int y3, int z3);

    bool searchRoadsNearby(int xCell, int yCell, int widthModel, int heightModel);
};

#endif // USAGEFACADE_HPP
