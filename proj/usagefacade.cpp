#include "usagefacade.hpp"

#include "QDebug"
#include "QPen"

#include <limits>

#include "math.h"
#include "config.hpp"

UsageFacade::UsageFacade() { drawer = new Drawer; }

void UsageFacade::setCellScene(size_t width_, size_t height_)
{
    if (scene)
        delete scene;

    scene = new CellScene(width_, height_);
}

void UsageFacade::changeCellScene(size_t newWidth, size_t newheight)
{
    scene->changeSize(newWidth, newheight);
}

bool UsageFacade::isSceneSet() { return scene; }

QGraphicsScene *UsageFacade::drawScene(QRectF rect)
{
    QGraphicsScene *retScene = nullptr;
    if (isSceneSet())
        retScene = drawer->drawScene(scene, rect);

    return retScene;
}

QGraphicsScene *UsageFacade::moveUpScene(double value, QRectF rect)
{
    scene->moveUp(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveDownScene(double value, QRectF rect)
{
    scene->moveDown(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveRightScene(double value, QRectF rect)
{
    scene->moveRight(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::moveLeftScene(double value, QRectF rect)
{
    scene->moveLeft(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::scaleScene(double value, QRectF rect)
{
    scene->scale(value);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateXScene(double angle, QRectF rect)
{
    scene->rotateX(angle);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateYScene(double angle, QRectF rect)
{
    scene->rotateY(angle);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::rotateZScene(double angle, QRectF rect)
{
    scene->rotateZ(angle);

    return drawScene(rect);
}

QGraphicsScene *UsageFacade::toCenter(QRectF rect)
{
    scene->toCenter();

    return drawScene(rect);
}

CellScene *UsageFacade::getScene() { return scene; }

void UsageFacade::addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2,
    int x3, int y3, int z3, int x4, int y4, int z4)
{
    Dot3D dot;
    std::vector<size_t> vec;

    size_t size = facets.size();

    dot = Dot3D(x1, y1, z1);
    vec = {size, size + 1};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x2, y2, z2);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x3, y3, z3);
    vec = {size, size + 1};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x4, y4, z4);
    vec = {size + 1};
    vertices.push_back(Vertex(dot, vec));

    size = vertices.size();
    vec = {size - 4, size - 3, size - 2};
    facets.push_back(vec);
    vec = {size - 4, size - 2, size - 1};
    facets.push_back(vec);
}

void UsageFacade::addTriangle(std::vector<Vertex> &vertices, std::vector<Facet> &facets,
    int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3)
{
    Dot3D dot;
    std::vector<size_t> vec;

    size_t size = facets.size();

    dot = Dot3D(x1, y1, z1);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x2, y2, z2);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    dot = Dot3D(x3, y3, z3);
    vec = {size};
    vertices.push_back(Vertex(dot, vec));

    size = vertices.size();
    vec = {size - 3, size - 2, size - 1};
    facets.push_back(vec);
}

void UsageFacade::addIlluminant(int xAngle, int yAngle)
{
    Eigen::Matrix4f transMat;
    double radianXAngle = (double) xAngle * M_PI / 180.0;
    double radianYAngle = (double) yAngle * M_PI / 180.0;

    transMat << 1,          0,          0,               0,
               0, cos(radianXAngle), -sin(radianXAngle), 0,
               0, sin(radianXAngle),  cos(radianXAngle), 0,
               0,           0,          0,               1;

    Eigen::Matrix4f prodMat;
    prodMat << cos(radianYAngle), 0, sin(radianYAngle), 0,
                0,                1,    0,              0,
              -sin(radianYAngle), 0, cos(radianYAngle), 0,
                0,                0,    0,              1;

    transMat *= prodMat;

    Illuminant illum(transMat);
    illum.setAngles(xAngle, yAngle);
    scene->addIlluminant(illum);
}

void Drawer::specBorderPut(int x, int y, double z)
{
    if (x < 0 || x >= (int) depthBuffer.size() || \
        y < 0 || y >= (int) depthBuffer.at(0).size())
        return;
    if (std::fabs(z - depthBuffer.at(x).at(y)) < 1 || z > depthBuffer.at(x).at(y))
        frameBuffer.at(x).at(y) = 3;
}

void Drawer::DDABordersForPolygon(
    int xStart, int yStart, double zStart, int xEnd, int yEnd, double zEnd)
{
    if (xStart == xEnd && yStart == yEnd)
    {
        specBorderPut(xStart, yStart, zStart);
        return;
    }

    double deltaX = xEnd - xStart;
    double deltaY = yEnd - yStart;
    double deltaZ = zEnd - zStart;

    int trX = abs(deltaX);
    int trY = abs(deltaY);

    int length;
    if (trX > trY)
        length = trX;
    else
        length = trY;

    deltaX /= length;
    deltaY /= length;
    deltaZ /= length;

    double curX = xStart;
    double curY = yStart;
    double curZ = zStart;

    for (int i = 0; i < length; i++)
    {
        specBorderPut(round(curX), round(curY), curZ);
        curX += deltaX;
        curY += deltaY;
        curZ += deltaZ;
    }
}

void Drawer::interpolateRowIntoShadowMap(
    std::vector<std::vector<double>> *map, int xA, int xB, double zA, double zB, int curY)
{
    for (int curX = xA; curX <= xB; curX++)
    {
        double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);
        if (curX >= (int) map->size() || curX < 0 || curY >= (int) map->at(0).size() ||
            curY < 0)
            continue;

        if (curZ > map->at(curX).at(curY))
            map->at(curX).at(curY) = curZ;
    }
}

void Drawer::shadowMapForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
    Eigen::Matrix4f &transMat, Illuminant *illum, size_t bufWidth, size_t bufHeight)
{
    std::array<Dot3D, 3> dotsArr;
    Eigen::Matrix4f toCenter;

    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -PLATE_Z - 5, 1;

    Eigen::Matrix4f backToStart;

    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, PLATE_Z + 5, 1;

    std::vector<std::vector<double>> *shadowMap = &illum->getShadowMap();
    Eigen::Matrix4f illumMat = illum->getTransMat();

    Eigen::Matrix4f dotTransMat = toCenter * transMat * illumMat * backToStart;

    for (size_t curFaceNum = 0; curFaceNum < facets.size(); curFaceNum++)
    {
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getUsedVertices();
        dotsArr[0] = vertices.at(curDots.at(0)).getPosition();
        dotsArr[1] = vertices.at(curDots.at(1)).getPosition();
        dotsArr[2] = vertices.at(curDots.at(2)).getPosition();

        coordinatesVec <<
            dotsArr[0].getXCoordinate(), dotsArr[0].getYCoordinate(), dotsArr[0].getZCoordinate(), 1,
            dotsArr[1].getXCoordinate(), dotsArr[1].getYCoordinate(), dotsArr[1].getZCoordinate(), 1,
            dotsArr[2].getXCoordinate(), dotsArr[2].getYCoordinate(), dotsArr[2].getZCoordinate(), 1;

        coordinatesVec *= dotTransMat;
        dotsArr[0] =
            Dot3D(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot3D(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot3D(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getYCoordinate() > dotsArr[1].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[1]);
        if (dotsArr[0].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[2]);
        if (dotsArr[1].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[1], dotsArr[2]);

        int x1 = round(dotsArr[0].getXCoordinate());
        int x2 = round(dotsArr[1].getXCoordinate());
        int x3 = round(dotsArr[2].getXCoordinate());

        double z1 = dotsArr[0].getZCoordinate();
        double z2 = dotsArr[1].getZCoordinate();
        double z3 = dotsArr[2].getZCoordinate();

        int y1 = round(dotsArr[0].getYCoordinate());
        int y2 = round(dotsArr[1].getYCoordinate());
        int y3 = round(dotsArr[2].getYCoordinate());

#pragma omp parallel for
        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            interpolateRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
        }

#pragma omp parallel for
        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            interpolateRowIntoShadowMap(shadowMap, xA, xB, zA, zB, curY);
        }
    }
}

void Drawer::zBufForModel(std::vector<Facet> &facets, std::vector<Vertex> &vertices,
    Eigen::Matrix4f &transMat, size_t color, CellScene *scene, size_t bufWidth,
    size_t bufHeight)
{
    std::array<Dot3D, 3> dotsArr;
    Eigen::Matrix4f toCenter;

    toCenter << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -X_CENTER, -Y_CENTER, -PLATE_Z - 5, 1;

    Eigen::Matrix4f backToStart;

    backToStart << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   X_CENTER, Y_CENTER, PLATE_Z + 5, 1;

    Eigen::Matrix4f dotTransMat;
    dotTransMat = toCenter * transMat * backToStart;
    std::vector<Eigen::Matrix4f> illumDotMatrices;

    for (size_t i = 0; i < scene->getIllumNum(); i++)
        illumDotMatrices.push_back(
            toCenter * scene->getIlluminant(i).getTransMat() * backToStart);

    for (size_t curFaceNum = 0; curFaceNum < facets.size(); curFaceNum++)
    {
        Eigen::MatrixXf coordinatesVec(3, 4);

        std::vector<size_t> curDots = facets.at(curFaceNum).getUsedVertices();
        dotsArr[0] = vertices.at(curDots.at(0)).getPosition();
        dotsArr[1] = vertices.at(curDots.at(1)).getPosition();
        dotsArr[2] = vertices.at(curDots.at(2)).getPosition();

        coordinatesVec <<
            dotsArr[0].getXCoordinate(), dotsArr[0].getYCoordinate(), dotsArr[0].getZCoordinate(), 1,
            dotsArr[1].getXCoordinate(), dotsArr[1].getYCoordinate(), dotsArr[1].getZCoordinate(), 1,
            dotsArr[2].getXCoordinate(), dotsArr[2].getYCoordinate(), dotsArr[2].getZCoordinate(), 1;

        coordinatesVec *= dotTransMat;

        dotsArr[0] =
            Dot3D(coordinatesVec(0, 0), coordinatesVec(0, 1), coordinatesVec(0, 2));
        dotsArr[1] =
            Dot3D(coordinatesVec(1, 0), coordinatesVec(1, 1), coordinatesVec(1, 2));
        dotsArr[2] =
            Dot3D(coordinatesVec(2, 0), coordinatesVec(2, 1), coordinatesVec(2, 2));

        if (dotsArr[0].getYCoordinate() > dotsArr[1].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[1]);
        if (dotsArr[0].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[0], dotsArr[2]);
        if (dotsArr[1].getYCoordinate() > dotsArr[2].getYCoordinate())
            std::swap(dotsArr[1], dotsArr[2]);

        int x1 = round(dotsArr[0].getXCoordinate());
        int x2 = round(dotsArr[1].getXCoordinate());
        int x3 = round(dotsArr[2].getXCoordinate());

        double z1 = dotsArr[0].getZCoordinate();
        double z2 = dotsArr[1].getZCoordinate();
        double z3 = dotsArr[2].getZCoordinate();

        int y1 = round(dotsArr[0].getYCoordinate());
        int y2 = round(dotsArr[1].getYCoordinate());
        int y3 = round(dotsArr[2].getYCoordinate());

#pragma omp parallel for
        for (int curY = (y1 < 0) ? 0 : y1;
             curY < ((y2 >= (int) bufHeight) ? (int) bufHeight - 1 : y2); curY++)
        {
            double aInc = 0;
            if (y1 != y2)
                aInc = (double) (curY - y1) / (y2 - y1);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x1 + (x2 - x1) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z1 + (z2 - z1) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);

                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);

                    for (size_t i = 0; i < scene->getIllumNum() && !visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getIlluminant(i).getShadowMap();

                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));

                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getIllumNum())
                    {
                        frameBuffer.at(curX).at(curY) = color + visible;
                    }
                    else
                        frameBuffer.at(curX).at(curY) = color + 1;
                }
            }
        }
#pragma omp parallel for
        for (int curY = (y2 < 0) ? 0 : y2;
             curY <= ((y3 >= (int) bufHeight) ? (int) bufHeight - 1 : y3); curY++)
        {
            double aInc = 0;
            if (y2 != y3)
                aInc = (double) (curY - y2) / (y3 - y2);

            double bInc = 0;
            if (y1 != y3)
                bInc = (double) (curY - y1) / (y3 - y1);

            int xA = round(x2 + (x3 - x2) * aInc);
            int xB = round(x1 + (x3 - x1) * bInc);
            double zA = z2 + (z3 - z2) * aInc;
            double zB = z1 + (z3 - z1) * bInc;

            if (xA > xB)
            {
                std::swap(xA, xB);
                std::swap(zA, zB);
            }

            if (xA < 0)
                xA = 0;
            if (xB >= (int) bufWidth)
                xB = (int) bufWidth - 1;

            for (int curX = xA; curX <= xB; curX++)
            {
                double curZ = zA + (zB - zA) * (curX - xA) / (xB - xA);

                if (curZ >= depthBuffer.at(curX).at(curY))
                {
                    short visible = 0;
                    Eigen::MatrixXf newCoordinates(1, 4);
                    for (size_t i = 0; i < scene->getIllumNum() && !visible; i++)
                    {
                        newCoordinates << curX, curY, curZ, 1;

                        newCoordinates *= illumDotMatrices.at(i);
                        std::vector<std::vector<double>> *shadowMap =
                            &scene->getIlluminant(i).getShadowMap();
                        int x = round(newCoordinates(0, 0));
                        int y = round(newCoordinates(0, 1));

                        if (x < (int) shadowMap->size() && x >= 0 &&
                            y < (int) shadowMap->at(0).size() && y >= 0 &&
                            std::fabs(shadowMap->at(x).at(y) - newCoordinates(0, 2)) < 2)
                            visible = 1;
                    }
                    depthBuffer.at(curX).at(curY) = curZ;
                    if (scene->getIllumNum())
                    {
                        frameBuffer.at(curX).at(curY) = color + visible;
                    }
                    else
                        frameBuffer.at(curX).at(curY) = color + 1;
                }
            }
        }
        DDABordersForPolygon(x1, y1, z1, x2, y2, z2);
        DDABordersForPolygon(x1, y1, z1, x3, y3, z3);
        DDABordersForPolygon(x2, y2, z2, x3, y3, z3);
    }
}

void Drawer::zBufferAlg(CellScene *scene, size_t bufHeight, size_t bufWidth)
{
    depthBuffer.erase(depthBuffer.begin(), depthBuffer.end());
    frameBuffer.erase(frameBuffer.begin(), frameBuffer.end());

    for (size_t i = 0; i < bufWidth; i++)
    {
        depthBuffer.push_back(std::vector<double>(bufHeight, 0));
        frameBuffer.push_back(std::vector<size_t>(bufHeight, 0));
    }

    PolModel model;
    std::vector<Facet> facets;
    std::vector<Vertex> vertices;
    PolModel::model_t typeModel;

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        facets = model.getFacets();
        vertices = model.getVertices();
        for (size_t j = 0; j < scene->getIllumNum(); j++)
            shadowMapForModel(facets, vertices, scene->getTransMatrix(),
                &scene->getIlluminant(j), bufWidth, bufHeight);
    }

    model = scene->getPlateModel();
    facets = model.getFacets();
    vertices = model.getVertices();
    for (size_t j = 0; j < scene->getIllumNum(); j++)
        shadowMapForModel(facets, vertices, scene->getTransMatrix(),
            &scene->getIlluminant(j), bufWidth, bufHeight);

    for (size_t i = 0; i < scene->getModelsNum(); i++)
    {
        model = scene->getModel(i);
        facets = model.getFacets();
        vertices = model.getVertices();
        typeModel = model.getModelType();
        zBufForModel(
            facets, vertices, scene->getTransMatrix(), 4 + typeModel * 2, scene, bufWidth, bufHeight);
    }
    model = scene->getPlateModel();
    facets = model.getFacets();
    vertices = model.getVertices();
    zBufForModel(
        facets, vertices, scene->getTransMatrix(), 1, scene, bufWidth, bufHeight);

    for (size_t i = 0; i < scene->getIllumNum(); i++)
        scene->getIlluminant(i).clearShadowMap();
}

QGraphicsScene *Drawer::drawScene(CellScene *scene, QRectF rect)
{
    size_t width = scene->getWidth() * SCALE_FACTOR;
    size_t height = scene->getHeight() * SCALE_FACTOR;
    qDebug() << "Сцена" << scene->getWidth() << " x " << scene->getHeight();

    scene->buildPlateModel(Dot3D(PLATE_START), Dot3D(width, height, PLATE_Z));

    using namespace std::chrono;
    nanoseconds start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    zBufferAlg(scene, rect.size().height(), rect.size().width());
    nanoseconds end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

    qDebug() << "Время выполнения z-буфера" << size_t((end - start).count() / 1000000);




    QGraphicsScene *outScene = new QGraphicsScene;
    outScene->setSceneRect(rect);

    QImage *image =
        new QImage(rect.size().width(), rect.size().height(), QImage::Format_RGB32);
    image->fill(Qt::white);

    uint plateCol             = qRgb(GRASS_COLOR);
    uint darkPlateCol         = qRgb(DARK_GRASS_COLOR);
    uint blackCol             = qRgb(BLACK_COLOUR);

    uint houseCol             = qRgb(HOUS_COLOR);
    uint darkHouseCol         = qRgb(DARK_HOUS_COLOR);

    uint roofHouseCol         = qRgb(ROOF_HOUS_COLOR);
    uint darkRoofHouseCol     = qRgb(DARK_ROOF_HOUS_COLOR);

    uint windowsHouseCol      = qRgb(WINDOWS_HOUS_COLOR);
    uint darkWindowsHouseCol  = qRgb(DARK_WINDOWS_HOUS_COLOR);

    uint treeFoliageCol       = qRgb(TREE_FOLIAGE_COLOR);
    uint darkTreeFoliageCol   = qRgb(DARK_TREE_FOLIAGE_COLOR);

    uint treeTrunkCol         = qRgb(TREE_TRUNK_COLOR);
    uint darkTreeTrunkCol     = qRgb(DARK_TREE_TRUNK_COLOR);

    uint roadAsphaltCol       = qRgb(ROAD_ASPHALT_COLOR);
    uint darkRoadAsphaltCol   = qRgb(DARK_ROAD_ASPHALT_COLOR);

    uint roadStripeCol        = qRgb(ROAD_STRIPE_COLOR);
    uint darkRoadStripeCol    = qRgb(DARK_ROAD_STRIPE_COLOR);

    uint carCol               = qRgb(CAR_COLOR);
    uint darkCarCol           = qRgb(DARK_CAR_COLOR);

    uint wheelsCarCol         = qRgb(CAR_WHEELS_COLOR);
    uint darkWheelsCarCol     = qRgb(DARK_CAR_WHEELS_COLOR);

    uint glassCarCol          = qRgb(CAR_GLASS_COLOR);
    uint darkGlassCarCol      = qRgb(DARK_CAR_GLASS_COLOR);

    nanoseconds start2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());

    for (size_t i = 0; i < rect.size().width() - 1; i++)
        for (size_t j = 0; j < rect.size().height() - 1; j++)
        {
            if (frameBuffer.at(i).at(j) == 1)
            {
                image->setPixel(i, j, darkPlateCol);
            }
            else if (frameBuffer.at(i).at(j) == 2)
            {
                image->setPixel(i, j, plateCol);
            }
            else if (frameBuffer.at(i).at(j) == 3)
            {
                image->setPixel(i, j, blackCol);
            }
            else if (frameBuffer.at(i).at(j) == 4)
            {
                image->setPixel(i, j, darkHouseCol);
            }
            else if (frameBuffer.at(i).at(j) == 5)
            {
                image->setPixel(i, j, houseCol);
            }
            else if (frameBuffer.at(i).at(j) == 6)
            {
                image->setPixel(i, j, darkRoofHouseCol);
            }
            else if (frameBuffer.at(i).at(j) == 7)
            {
                image->setPixel(i, j, roofHouseCol);
            }
            else if (frameBuffer.at(i).at(j) == 8)
            {
                image->setPixel(i, j, darkWindowsHouseCol);
            }
            else if (frameBuffer.at(i).at(j) == 9)
            {
                image->setPixel(i, j, windowsHouseCol);
            }
            else if (frameBuffer.at(i).at(j) == 10)
            {
                image->setPixel(i, j, darkTreeFoliageCol);
            }
            else if (frameBuffer.at(i).at(j) == 11)
            {
                image->setPixel(i, j, treeFoliageCol);
            }
            else if (frameBuffer.at(i).at(j) == 12)
            {
                image->setPixel(i, j, darkTreeTrunkCol);
            }
            else if (frameBuffer.at(i).at(j) == 13)
            {
                image->setPixel(i, j, treeTrunkCol);
            }
            else if (frameBuffer.at(i).at(j) == 14)
            {
                image->setPixel(i, j, darkRoadAsphaltCol);
            }
            else if (frameBuffer.at(i).at(j) == 15)
            {
                image->setPixel(i, j, roadAsphaltCol);
            }
            else if (frameBuffer.at(i).at(j) == 16)
            {
                image->setPixel(i, j, darkRoadStripeCol);
            }
            else if (frameBuffer.at(i).at(j) == 17)
            {
                image->setPixel(i, j, roadStripeCol);
            }
            else if (frameBuffer.at(i).at(j) == 18)
            {
                image->setPixel(i, j, darkCarCol);
            }
            else if (frameBuffer.at(i).at(j) == 19)
            {
                image->setPixel(i, j, carCol);
            }
            else if (frameBuffer.at(i).at(j) == 20)
            {
                image->setPixel(i, j, darkWheelsCarCol);
            }
            else if (frameBuffer.at(i).at(j) == 21)
            {
                image->setPixel(i, j, wheelsCarCol);
            }
            else if (frameBuffer.at(i).at(j) == 22)
            {
                image->setPixel(i, j, darkGlassCarCol);
            }
            else if (frameBuffer.at(i).at(j) == 23)
            {
                image->setPixel(i, j, glassCarCol);
            }
        }

    nanoseconds end2 = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    qDebug() << "Время отрисовки" << size_t((end2 - start2).count() / 1000000);
    qDebug() << "Общее время"     << size_t((end2 - start2).count() / 1000000) +
                                     size_t((end - start).count()   / 1000000);


    outScene->addPixmap(QPixmap::fromImage(*image));
    delete image;
    for (size_t i = 0; i < scene->getIllumNum(); i++)
        scene->getIlluminant(i).clearShadowMap();

    return outScene;
}




bool UsageFacade::searchRoadsNearby(int xCell, int yCell, int widthModel, int heightModel)
{
    for (int k = 0; k < 2; k++)
    {
        int i = yCell - 1 + k * (heightModel + 1);
        if (i < 0 || size_t(i) > scene->getHeight() - 1)
            continue;

        int j0 = xCell - 1;
        if (j0 >= 0)
        {
            if (scene->getUsedCells()[i][j0] == 2 || scene->getUsedCells()[i][j0] == 4)
            {
                qDebug() << "i = " << i << "j = " << j0 << "дорога рядом с домом";
                return true;
            }
        }

        j0 = xCell + widthModel;
        if (size_t(j0) <= scene->getWidth() - 1)
        {
            if (scene->getUsedCells()[i][j0] == 2 || scene->getUsedCells()[i][j0] == 4)
            {
                qDebug() << "i = " << i << "j = " << j0 << "дорога рядом с домом";
                return true;
            }
        }

        for (int j = xCell; j < xCell + widthModel; j++)
        {
            if (scene->getUsedCells()[i][j] == 2 || scene->getUsedCells()[i][j] == 4)
            {
                qDebug() << "i = " << i << "j = " << j << "дорога рядом с домом";
                return true;
            }
        }
    }

    for (int k = 0; k < 2; k++)
    {
        int j = xCell - 1 + k * (widthModel + 1);
        if (j < 0 || size_t(j) > scene->getWidth() - 1)
            continue;

        for (int i = yCell; i < yCell + heightModel; i++)
        {
            if (scene->getUsedCells()[i][j] == 2 || scene->getUsedCells()[i][j] == 4)
            {
                qDebug() << "i = " << i << "j = " << j << "дорога рядом с домом";
                return true;
            }
        }
    }

    return false;
}






int UsageFacade::addHouse(int xCell, int yCell, int modelLength, int modelHeight, int countFloors)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelHeight - 1 >= (int) scene->getHeight())
        return 2;

    for (int i = yCell; i < yCell + modelHeight; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCells()[i][j] != 0 && scene->getUsedCells()[i][j] != 3)
            {
                qDebug() << "i = " << i << "j = " << j << "ячейка занята (дом)";
                return 1;
            }
        }
    }

    if (searchRoadsNearby(xCell, yCell, modelLength, modelHeight))
        return 4;

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SCALE_FACTOR + 10;
    int yFactor = yCell * SCALE_FACTOR + 10;
    int zFactor = PLATE_Z;

    //стены дома
    for (int i = 0; i < countFloors; i++)
    {
        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < modelLength; j++)
            {
                addQuad(vertices, facets,
                        xFactor,                yFactor, zFactor,
                        xFactor + SCALE_FACTOR, yFactor, zFactor,
                        xFactor + SCALE_FACTOR, yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor,                yFactor, zFactor + SCALE_FACTOR / 3);
                addQuad(vertices, facets,
                        xFactor,                yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor + SCALE_FACTOR, yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor + SCALE_FACTOR, yFactor, zFactor + SCALE_FACTOR,
                        xFactor,                yFactor, zFactor + SCALE_FACTOR);
                addQuad(vertices, facets,
                        xFactor,                    yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor + SCALE_FACTOR / 3, yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor + SCALE_FACTOR / 3, yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor,                    yFactor, zFactor + SCALE_FACTOR * 2 / 3);
                addQuad(vertices, facets,
                        xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor + SCALE_FACTOR,         yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor + SCALE_FACTOR,         yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR * 2 / 3);

                xFactor += SCALE_FACTOR;
            }

            xFactor = xCell * SCALE_FACTOR + 10;
            yFactor += SCALE_FACTOR * modelHeight;
        }

        yFactor = yCell * SCALE_FACTOR + 10;


        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < modelHeight; j++)
            {
                addQuad(vertices, facets,
                        xFactor, yFactor,                zFactor,
                        xFactor, yFactor + SCALE_FACTOR, zFactor,
                        xFactor, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor,                zFactor + SCALE_FACTOR / 3);
                addQuad(vertices, facets,
                        xFactor, yFactor,                zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
                        xFactor, yFactor,                zFactor + SCALE_FACTOR);
                addQuad(vertices, facets,
                        xFactor, yFactor,                    zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor,                    zFactor + SCALE_FACTOR * 2 / 3);
                addQuad(vertices, facets,
                        xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor + SCALE_FACTOR,         zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor + SCALE_FACTOR,         zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2 / 3);

                yFactor += SCALE_FACTOR;
            }

            yFactor = yCell * SCALE_FACTOR + 10;
            xFactor += SCALE_FACTOR * modelLength;
        }

        xFactor = xCell * SCALE_FACTOR + 10;

        zFactor += SCALE_FACTOR;
    }

    PolModel houseModel(vertices, facets, "Дом");
    houseModel.setUsedCell(xCell, yCell);
    houseModel.setHeightModel(modelHeight);
    houseModel.setWidthModel(modelLength);
    houseModel.setHouseHeight(countFloors);
    houseModel.setModelType(PolModel::House);
    houseModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(houseModel);

    std::vector<Vertex> vertices2;
    std::vector<Facet> facets2;

    //крыша дома
    addTriangle(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR,         yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR,         yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addTriangle(vertices2, facets2,
                xFactor,                                  yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor,                                  yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addTriangle(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR,         yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addTriangle(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR,         yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addTriangle(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR / 3, yFactor,                                  zFactor,
                xFactor,                                  yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addTriangle(vertices2, facets2,
                xFactor,                                  yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR / 3, yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);


    addQuad(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR / 3,     yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor,                                  zFactor,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR,
                xFactor + modelLength * SCALE_FACTOR / 3,     yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR);
    addQuad(vertices2, facets2,
                xFactor + modelLength * SCALE_FACTOR / 3,     yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR / 2, zFactor + SCALE_FACTOR,
                xFactor + modelLength * SCALE_FACTOR * 2 / 3, yFactor + modelHeight * SCALE_FACTOR,     zFactor,
                xFactor + modelLength * SCALE_FACTOR / 3,     yFactor + modelHeight * SCALE_FACTOR,     zFactor);


    PolModel roofHouseModel(vertices2, facets2, "Крыша дома");
    roofHouseModel.setUsedCell(xCell, yCell);
    roofHouseModel.setHeightModel(modelHeight);
    roofHouseModel.setWidthModel(modelLength);
    roofHouseModel.setHouseHeight(countFloors);
    roofHouseModel.setModelType(PolModel::roofHouse);
    roofHouseModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(roofHouseModel);

    std::vector<Vertex> vertices3;
    std::vector<Facet> facets3;

    xFactor = xCell * SCALE_FACTOR + 10;
    yFactor = yCell * SCALE_FACTOR + 10;
    zFactor = PLATE_Z;

    //окна дома
    for (int i = 0; i < countFloors; i++)
    {
        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < modelLength; j++)
            {
                addQuad(vertices3, facets3,
                        xFactor + SCALE_FACTOR / 3,     yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR / 3,
                        xFactor + SCALE_FACTOR / 3,     yFactor, zFactor + SCALE_FACTOR / 3);

                xFactor += SCALE_FACTOR;
            }

            xFactor = xCell * SCALE_FACTOR + 10;
            yFactor += SCALE_FACTOR * modelHeight;
        }

        yFactor = yCell * SCALE_FACTOR + 10;


        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < modelHeight; j++)
            {
                addQuad(vertices3, facets3,
                        xFactor, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2 / 3,
                        xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR / 3,
                        xFactor, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR / 3);

                yFactor += SCALE_FACTOR;
            }

            yFactor = yCell * SCALE_FACTOR + 10;
            xFactor += SCALE_FACTOR * modelLength;
        }

        xFactor = xCell * SCALE_FACTOR + 10;

        zFactor += SCALE_FACTOR;
    }

    PolModel windowsHouseModel(vertices3, facets3, "Окна дома");
    windowsHouseModel.setUsedCell(xCell, yCell);
    windowsHouseModel.setHeightModel(modelHeight);
    windowsHouseModel.setWidthModel(modelLength);
    windowsHouseModel.setHouseHeight(countFloors);
    windowsHouseModel.setModelType(PolModel::windowsHouse);
    windowsHouseModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(windowsHouseModel);

    return 0;
}





int UsageFacade::addTree(int xCell, int yCell)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell >= (int) scene->getWidth() || yCell >= (int) scene->getHeight())
        return 2;

    if (scene->getUsedCells()[yCell][xCell] != 0 && scene->getUsedCells()[yCell][xCell] != 3)
    {
        qDebug() << "i = " << xCell << "j = " << xCell << "ячейка занята (дерево)";
        return 1;
    }

    int xFactor = xCell * SCALE_FACTOR + 10;
    int yFactor = yCell * SCALE_FACTOR + 10;
    int zFactor = PLATE_Z;

    std::vector<Vertex> vertices2;
    std::vector<Facet> facets2;

    //нижняя часть листвы дерева
    addQuad(vertices2, facets2,
            xFactor,                yFactor,                zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor,                zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
            xFactor,                yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR);

    addQuad(vertices2, facets2,
            xFactor,                yFactor, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor,                yFactor, zFactor + SCALE_FACTOR * 3 / 2);
    addQuad(vertices2, facets2,
            xFactor, yFactor,                zFactor + SCALE_FACTOR,
            xFactor, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
            xFactor, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor, yFactor,                zFactor + SCALE_FACTOR * 3 / 2);
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR, yFactor,                zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor,                zFactor + SCALE_FACTOR * 3 / 2);
    addQuad(vertices2, facets2,
            xFactor,                yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor,                yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2);

    addQuad(vertices2, facets2,
            xFactor,                yFactor,                zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor,                zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor,                yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2);

    //верхняя часть листвы дерева (вдоль Ox)
    addQuad(vertices2, facets2,
            xFactor,                yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR * 2,
            xFactor,                yFactor + SCALE_FACTOR / 3, zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 3 / 2,
            xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2,
            xFactor, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor,                yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2,
            xFactor,                yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2);

    addQuad(vertices2, facets2,
            xFactor,                yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR / 3,     zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2,
            xFactor,                yFactor + SCALE_FACTOR * 2 / 3, zFactor + SCALE_FACTOR * 2);

    //верхняя часть листвы дерева (вдоль Oy)
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR / 3,     yFactor, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR / 3,     yFactor, zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR / 3, yFactor,                zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR / 3, yFactor,                zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor,                zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor,                zFactor + SCALE_FACTOR * 2);
    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR / 3,     yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 3 / 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR / 3,     yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2);

    addQuad(vertices2, facets2,
            xFactor + SCALE_FACTOR / 3,     yFactor,                zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor,                zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR * 2 / 3, yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2,
            xFactor + SCALE_FACTOR / 3,     yFactor + SCALE_FACTOR, zFactor + SCALE_FACTOR * 2);

    PolModel treeFoliageModel(vertices2, facets2, "Дерево");
    treeFoliageModel.setUsedCell(xCell, yCell);
    treeFoliageModel.setHeightModel(1);
    treeFoliageModel.setWidthModel(1);
    treeFoliageModel.setModelType(PolModel::treeFoliage);
    treeFoliageModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(treeFoliageModel);


    std::vector<Vertex> vertices;
    std::vector<Facet> facets;


    //ствол дерева
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor + SCALE_FACTOR);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor + SCALE_FACTOR);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 2 / 5, zFactor + SCALE_FACTOR);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor,
            xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor + SCALE_FACTOR,
            xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 3 / 5, zFactor + SCALE_FACTOR);

    PolModel treeTrunkModel(vertices, facets, "Ствол дерева");
    treeTrunkModel.setUsedCell(xCell, yCell);
    treeTrunkModel.setHeightModel(1);
    treeTrunkModel.setWidthModel(1);
    treeTrunkModel.setModelType(PolModel::treeTrunk);
    treeTrunkModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(treeTrunkModel);

    return 0;
}





int UsageFacade::addRoad(int xCell, int yCell, PlaceRoadChooser::checkBox direction)
{
    xCell -= 1;
    yCell -= 1;

    if (xCell >= (int) scene->getWidth() || yCell >= (int) scene->getHeight())
        return 2;

    if (scene->getUsedCells()[yCell][xCell] == 3)
    {
        qDebug() << "i = " << xCell << "j = " << xCell << "ячейка рядом с домом (дорога)";
        return 4;
    }
    if (scene->getUsedCells()[yCell][xCell] != 0)
    {
        qDebug() << "i = " << xCell << "j = " << xCell << "ячейка занята (дорога)";
        return 1;
    }



    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SCALE_FACTOR + 10;
    int yFactor = yCell * SCALE_FACTOR + 10;
    int zFactor = PLATE_Z;

    //асфальт
    addQuad(vertices, facets,
            xFactor,                yFactor,                zFactor + 1,
            xFactor + SCALE_FACTOR, yFactor,                zFactor + 1,
            xFactor + SCALE_FACTOR, yFactor + SCALE_FACTOR, zFactor + 1,
            xFactor,                yFactor + SCALE_FACTOR, zFactor + 1);

    PolModel roadAsphaltModel(vertices, facets, "Дорога");
    roadAsphaltModel.setUsedCell(xCell, yCell);
    roadAsphaltModel.setHeightModel(1);
    roadAsphaltModel.setWidthModel(1);
    roadAsphaltModel.setDirectionRoad(direction);
    roadAsphaltModel.setModelType(PolModel::roadAsphalt);
    roadAsphaltModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(roadAsphaltModel);

    std::vector<Vertex> vertices2;
    std::vector<Facet> facets2;

    //полоса дороги
    if (direction == PlaceRoadChooser::checkBox::XAXIS)
    {
        addQuad(vertices2, facets2,
                xFactor + SCALE_FACTOR / 6,     yFactor + SCALE_FACTOR * 2 / 5, zFactor + 2,
                xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 2 / 5, zFactor + 2,
                xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 3 / 5, zFactor + 2,
                xFactor + SCALE_FACTOR / 6,     yFactor + SCALE_FACTOR * 3 / 5, zFactor + 2);
    }
    else
    {
        addQuad(vertices2, facets2,
                xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR / 6,     zFactor + 2,
                xFactor + SCALE_FACTOR * 2 / 5, yFactor + SCALE_FACTOR * 5 / 6, zFactor + 2,
                xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR * 5 / 6, zFactor + 2,
                xFactor + SCALE_FACTOR * 3 / 5, yFactor + SCALE_FACTOR / 6,     zFactor + 2);
    }

    PolModel roadStripeModel(vertices2, facets2, "Полоса дороги");
    roadStripeModel.setUsedCell(xCell, yCell);
    roadStripeModel.setHeightModel(1);
    roadStripeModel.setWidthModel(1);
    roadStripeModel.setDirectionRoad(direction);
    roadStripeModel.setModelType(PolModel::roadStripe);
    roadStripeModel.setModelNum(scene->getRealModelsNum());
    scene->addModel(roadStripeModel);

    return 0;
}




int UsageFacade::addCar(int xCell, int yCell, PlaceCarChooser::checkBox direction)
{
    int modelLength;
    int modelHeight;

    if(direction == PlaceCarChooser::checkBox::XAXIS)
    {
        modelHeight = 1;
        modelLength  = 2;
    }
    else
    {
        modelHeight = 2;
        modelLength  = 1;
    }

    xCell -= 1;
    yCell -= 1;

    if (xCell + modelLength - 1 >= (int) scene->getWidth() || \
        yCell + modelHeight - 1 >= (int) scene->getHeight())
        return 2;

    for (int i = yCell; i < yCell + modelHeight; i++)
    {
        for (int j = xCell; j < xCell + modelLength; j++)
        {
            if (scene->getUsedCells()[i][j] != 2)
            {
                qDebug() << "i = " << i << "j = " << j << "нет дороги (машина)";
                return 3;
            }
        }
    }


    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    int xFactor = xCell * SCALE_FACTOR + 10;
    int yFactor = yCell * SCALE_FACTOR + 10;
    int zFactor = PLATE_Z + SCALE_FACTOR / 16;

    //рама машины
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR * 5 / 6, zFactor);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR / 4, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 3);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 3,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 3);

    zFactor += SCALE_FACTOR / 3;

    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 4, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR / 4,     yFactor + SCALE_FACTOR * 5 / 6, zFactor);

    //кузов машины
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 5);
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5);

    //лобовое стекло
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR / 6,     zFactor);
    //заднее стекло
    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR / 6,     zFactor);

    // стёкла водителя
    addTriangle(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR / 6, zFactor);
    addTriangle(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor);

    // стёкла пассажира
    addTriangle(vertices, facets,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6, zFactor,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR / 6, zFactor);
    addTriangle(vertices, facets,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor);

    //крыша машины
    zFactor += SCALE_FACTOR / 5;

    addQuad(vertices, facets,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6,     zFactor,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6, zFactor);


    PolModel carModel(vertices, facets, "Машина");
    carModel.setUsedCell(xCell, yCell);
    carModel.setHeightModel(modelHeight);
    carModel.setWidthModel(modelLength);
    carModel.setDirectionCar(direction);
    carModel.setModelNum(scene->getRealModelsNum());
    carModel.setModelType(PolModel::Car);

    if(modelLength == 1)
        carModel.rotateZ(-90);

    scene->addModel(carModel);

    std::vector<Vertex> vertices2;
    std::vector<Facet> facets2;

    xFactor = xCell * SCALE_FACTOR + 10;
    yFactor = yCell * SCALE_FACTOR + 10;
    zFactor = PLATE_Z;

    //колёса машины
    for (int i = 0; i < 2; i++)
    {
        yFactor -= 2;

        for (int j = 0; j < 2; j++)
        {
            addQuad(vertices2, facets2,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR / 6,     zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR * 2 / 6, zFactor,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR * 2 / 6, zFactor);
            addQuad(vertices2, facets2,
                    xFactor + SCALE_FACTOR / 2, yFactor + SCALE_FACTOR / 6,     zFactor,
                    xFactor + SCALE_FACTOR / 2, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR / 2, yFactor + SCALE_FACTOR * 2 / 6, zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR / 2, yFactor + SCALE_FACTOR * 2 / 6, zFactor);
            addQuad(vertices2, facets2,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR / 6,     zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR / 6,     zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR * 2 / 6, zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR * 2 / 6, zFactor);
            addQuad(vertices2, facets2,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR / 6, zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR / 6, zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR / 6, zFactor + SCALE_FACTOR / 4);
            addQuad(vertices2, facets2,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR * 2 / 6, zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR * 2 / 6, zFactor,
                    xFactor + SCALE_FACTOR * 3 / 4, yFactor + SCALE_FACTOR * 2 / 6, zFactor + SCALE_FACTOR / 4,
                    xFactor + SCALE_FACTOR / 2,     yFactor + SCALE_FACTOR * 2 / 6, zFactor + SCALE_FACTOR / 4);

            xFactor += SCALE_FACTOR * 3 / 4;
        }

        xFactor = xCell * SCALE_FACTOR + 10;
        yFactor = yCell * SCALE_FACTOR + 10;

        yFactor += SCALE_FACTOR / 2 + 4;
    }

    PolModel wheelsCarModel(vertices2, facets2, "Колёса машина");
    wheelsCarModel.setUsedCell(xCell, yCell);
    wheelsCarModel.setHeightModel(modelHeight);
    wheelsCarModel.setWidthModel(modelLength);
    wheelsCarModel.setDirectionCar(direction);
    wheelsCarModel.setModelType(PolModel::wheelsCar);
    wheelsCarModel.setModelNum(scene->getRealModelsNum());

    if(modelLength == 1)
        wheelsCarModel.rotateZ(-90);

    scene->addModel(wheelsCarModel);

    std::vector<Vertex> vertices3;
    std::vector<Facet> facets3;

    xFactor = xCell * SCALE_FACTOR + 10;
    yFactor = yCell * SCALE_FACTOR + 10;
    zFactor = PLATE_Z + SCALE_FACTOR * 19 / 48;

    //лобовое стекло
    addQuad(vertices3, facets3,
            xFactor + SCALE_FACTOR * 5 / 6 - 3, yFactor + SCALE_FACTOR / 6 + 5,     zFactor + SCALE_FACTOR / 5 - 7 / 5,
            xFactor + SCALE_FACTOR * 5 / 6 - 3, yFactor + SCALE_FACTOR * 5 / 6 - 5, zFactor + SCALE_FACTOR / 5 - 7 / 5,
            xFactor + SCALE_FACTOR * 3 / 6 + 3, yFactor + SCALE_FACTOR * 5 / 6 - 5, zFactor + 17 / 5,
            xFactor + SCALE_FACTOR * 3 / 6 + 3, yFactor + SCALE_FACTOR / 6 + 5,     zFactor + 17 / 5);
    //заднее стекло
    addQuad(vertices3, facets3,
            xFactor + SCALE_FACTOR * 7 / 6 + 3, yFactor + SCALE_FACTOR / 6 + 5,     zFactor + SCALE_FACTOR / 5 - 7 / 5,
            xFactor + SCALE_FACTOR * 7 / 6 + 3, yFactor + SCALE_FACTOR * 5 / 6 - 5, zFactor + SCALE_FACTOR / 5 - 7 / 5,
            xFactor + SCALE_FACTOR * 9 / 6 - 3, yFactor + SCALE_FACTOR * 5 / 6 - 5, zFactor + 17 / 5,
            xFactor + SCALE_FACTOR * 9 / 6 - 3, yFactor + SCALE_FACTOR / 6 + 5,     zFactor + 17 / 5);

    zFactor -= SCALE_FACTOR / 15;
    xFactor += SCALE_FACTOR / 15;

    // стёкла водителя
    addTriangle(vertices3, facets3,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor);
    addTriangle(vertices3, facets3,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 5 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor,
            xFactor + SCALE_FACTOR * 3 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor);

    xFactor -= SCALE_FACTOR * 2 / 15;

    // стёкла пассажира
    addTriangle(vertices3, facets3,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR / 6 - 1, zFactor);
    addTriangle(vertices3, facets3,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor + SCALE_FACTOR / 5,
            xFactor + SCALE_FACTOR * 7 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor,
            xFactor + SCALE_FACTOR * 9 / 6, yFactor + SCALE_FACTOR * 5 / 6 + 1, zFactor);

    PolModel glassCarModel(vertices3, facets3, "Стёкла машины");
    glassCarModel.setUsedCell(xCell, yCell);
    glassCarModel.setHeightModel(modelHeight);
    glassCarModel.setWidthModel(modelLength);
    glassCarModel.setDirectionCar(direction);
    glassCarModel.setModelType(PolModel::glassCar);
    glassCarModel.setModelNum(scene->getRealModelsNum());

    if(modelLength == 1)
        glassCarModel.rotateZ(-90);

    scene->addModel(glassCarModel);

    return 0;
}

