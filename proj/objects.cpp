#include "objects.hpp"
#include <QDebug>

#include "config.hpp"


const Dot3D &Vertex::getPosition() const { return position; }

void Vertex::setPosition(Dot3D &position_) { position = position_; }


const std::vector<size_t> Vertex::getUsedFacets() { return usedFacets; }

void Vertex::setUsedFacets(std::vector<size_t> usedFacets_) { usedFacets = usedFacets_; }


const std::vector<size_t> Facet::getUsedVertices() { return usedVertices; }

void Facet::setUsedVertices(std::vector<size_t> usedVertices_) { usedVertices = usedVertices_; }


const std::vector<Vertex> PolModel::getVertices() { return vertices; }

void PolModel::setVertices(std::vector<Vertex> &vertices_) { vertices = vertices_; }


const std::vector<Facet> PolModel::getFacets() { return facets; }

void PolModel::setFacets(std::vector<Facet> facets_) { facets = facets_; }


QString PolModel::getName() { return modelName; }

void PolModel::setName(QString modelName_) { modelName = modelName_; }


void PolModel::setWidthModel(int widthModel_) { widthModel = widthModel_; }

int  PolModel::getWidthModel() { return widthModel; }

void PolModel::setHeightModel(int heightModel_) { heightModel = heightModel_; }

int  PolModel::getHeightModel() { return heightModel; }


void PolModel::setHouseHeight(int houseHeight_) { houseHeight = houseHeight_; }

int PolModel::getHouseHeight() { return houseHeight; }


PlaceRoadChooser::checkBox PolModel::getDirectionRoad() { return directionRoad; }

void PolModel::setDirectionRoad(PlaceRoadChooser::checkBox directionRoad_) { directionRoad = directionRoad_; }


PlaceCarChooser::checkBox PolModel::getDirectionCar() { return directionCar; }

void PolModel::setDirectionCar(PlaceCarChooser::checkBox directionCar_) { directionCar = directionCar_; }


void PolModel::setModelNum(size_t modelNum_) { modelNum = modelNum_; }

size_t PolModel::getModelNum() { return modelNum; }


void PolModel::setUsedCell(int xCell_, int yCell_)
{
    xCell = xCell_;
    yCell = yCell_;
}

int PolModel::getUsedXCell() { return xCell; }

int PolModel::getUsedYCell() { return yCell; }


void PolModel::setModelType(model_t modelType_) { modelType = modelType_; }

PolModel::model_t PolModel::getModelType() { return modelType; }


void PolModel::rotateZ(int angle)
{
    double radianAngle = (double) angle * M_PI / 180.0;

    Dot3D start(PLATE_START);
    double xCenter = start.getXCoordinate() + xCell * SCALE_FACTOR + SCALE_FACTOR / 2;
    double yCenter = start.getYCoordinate() + yCell * SCALE_FACTOR + SCALE_FACTOR / 2;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot3D curDot = vertices.at(i).getPosition();
        curDot.rotateZ(radianAngle, xCenter, yCenter, 0);
        vertices.at(i).setPosition(curDot);
    }
}

void PolModel::moveTo(int newXCell, int newYCell)
{
    Dot3D start(PLATE_START);

    int xInc = SCALE_FACTOR * newXCell - SCALE_FACTOR * xCell;
    int yInc = SCALE_FACTOR * newYCell - SCALE_FACTOR * yCell;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        Dot3D curDot = vertices.at(i).getPosition();
        curDot.move(xInc, yInc, 0);
        vertices.at(i).setPosition(curDot);
    }

    xCell = newXCell;
    yCell = newYCell;
}


std::vector<std::vector<double>> &Illuminant::getShadowMap() { return shadowMap; }

Illuminant::Illuminant(Eigen::Matrix4f &transMatrix_)
{
    transMatrix = transMatrix_;
    for (size_t i = 0; i < ILLUM_VIS_X; i++)
    { shadowMap.push_back(std::vector<double>(ILLUM_VIS_Y, 0)); }
}

void Illuminant::setShadowMap(std::vector<std::vector<double>> &setShadowMap)
{
    shadowMap = setShadowMap;
}

void Illuminant::clearShadowMap()
{
    for (size_t i = 0; i < shadowMap.size(); i++)
    {
        for (size_t j = 0; j < shadowMap.at(0).size(); j++)
            shadowMap.at(i).at(j) = 0;
    }
}

void Illuminant::setAngles(int xAngle_, int yAngle_)
{
    xAngle = xAngle_;
    yAngle = yAngle_;
}

int Illuminant::getXAngle() { return xAngle; }

int Illuminant::getYAngle() { return yAngle; }

Eigen::Matrix4f &Illuminant::getTransMat() { return transMatrix; }

void Illuminant::setTransMat(Eigen::Matrix4f &mat) { transMatrix = mat; }

CellScene::CellScene() {}

CellScene::CellScene(size_t width_, size_t height_)
{
    width = width_;
    height = height_;

    modelsNum = 0;
    illumNum = 0;

    initUsedCells();
    toCenter();
}

void CellScene::addQuad(std::vector<Vertex> &vertices, std::vector<Facet> &facets, int x1,
int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4)
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

size_t CellScene::getWidth() { return width; }

size_t CellScene::getHeight() { return height; }

CellScene::operator bool() const { return this->plateModel; }


void CellScene::markUsedCells(size_t num)
{
    int xCell = models[num].getUsedXCell();
    int yCell = models[num].getUsedYCell();
    int widthModel = models[num].getWidthModel();
    int heightModel = models[num].getHeightModel();
    PolModel::model_t modelType_ = models[num].getModelType();

    int key;
    // 1 - стоит модель
    // 2 - стоит дорога
    // 3 - ячейка прилегает к дому
    // 4 - стоит машина на дороге

    if (modelType_ == PolModel::model_t::roadStripe)
        key = 2;
    else if (modelType_ == PolModel::model_t::glassCar)
        key = 4;
    else
        key = 1;

    if (modelType_ == PolModel::model_t::windowsHouse)
        markCellsNearHouse(xCell, yCell, widthModel, heightModel);


    for (int i = yCell; i < yCell + heightModel; i++)
    {
        for (int j = xCell; j < xCell + widthModel; j++)
        {
            getUsedCells()[i][j] = key;
            qDebug() << "i = " << i << "j = " << j << "занята моделью";
        }
    }
}

void CellScene::markCellsNearHouse(int xCell, int yCell, int widthModel, int heightModel)
{
    for (int k = 0; k < 2; k++)
    {
        int i = yCell - 1 + k * (heightModel + 1);
        if (i < 0 || size_t(i) > getHeight() - 1)
            continue;

        int j0 = xCell - 1;
        if (j0 >= 0)
        {
            if (getUsedCells()[i][j0] == 0)
                getUsedCells()[i][j0] = 3;
            qDebug() << "i = " << i << "j = " << j0 << "отмечена как прилегающая к дому";
        }

        j0 = xCell + widthModel;
        if (size_t(j0) <= getWidth() - 1)
        {
            if (getUsedCells()[i][j0] == 0)
                getUsedCells()[i][j0] = 3;
            qDebug() << "i = " << i << "j = " << j0 << "отмечена как прилегающая к дому";
        }

        for (int j = xCell; j < xCell + widthModel; j++)
        {
            if (getUsedCells()[i][j] == 0)
                getUsedCells()[i][j] = 3;
            qDebug() << "i = " << i << "j = " << j << "отмечена как прилегающая к дому";
        }
    }

    for (int k = 0; k < 2; k++)
    {
        int j = xCell - 1 + k * (widthModel + 1);
        if (j < 0 || size_t(j) > getWidth() - 1)
            continue;

        for (int i = yCell; i < yCell + heightModel; i++)
        {
            if (getUsedCells()[i][j] == 0)
                getUsedCells()[i][j] = 3;
            qDebug() << "i = " << i << "j = " << j << "отмечена как прилегающая к дому";
        }
    }
}

void CellScene::clearUsedCells(size_t num)
{
    int xCell = models[num].getUsedXCell();
    int yCell = models[num].getUsedYCell();
    int widthModel = models[num].getWidthModel();
    int heightModel = models[num].getHeightModel();
    PolModel::model_t modelType_ = models[num].getModelType();

    int key;
    if (modelType_ == PolModel::model_t::glassCar)
        key = 2;
    else
        key = 0;

    if (modelType_ == PolModel::model_t::windowsHouse)
        clearCellsNearHouse(xCell, yCell, widthModel, heightModel);


    for (int i = yCell; i < yCell + heightModel; i++)
    {
        for (int j = xCell; j < xCell + widthModel; j++)
        {
            getUsedCells()[i][j] = key;
            qDebug() << "i = " << i << "j = " << j << "освобождена после удаления модели";
        }
    }
}


void CellScene::clearCellsNearHouse(int xCell, int yCell, int widthModel, int heightModel)
{
    for (int k = 0; k < 2; k++)
    {
        int i = yCell - 1 + k * (heightModel + 1);
        if (i < 0 || size_t(i) > getHeight() - 1)
            continue;

        int j0 = xCell - 1;
        if (j0 >= 0)
        {
            if (getUsedCells()[i][j0] == 3)
                getUsedCells()[i][j0] = 0;
            qDebug() << "i = " << i << "j = " << j0 << "отмечена как не прилегающая к дому";
        }

        j0 = xCell + widthModel;
        if (size_t(j0) <= getWidth() - 1)
        {
            if (getUsedCells()[i][j0] == 3)
                getUsedCells()[i][j0] = 0;
            qDebug() << "i = " << i << "j = " << j0 << "отмечена как не прилегающая к дому";
        }

        for (int j = xCell; j < xCell + widthModel; j++)
        {
            if (getUsedCells()[i][j] == 3)
                getUsedCells()[i][j] = 0;
            qDebug() << "i = " << i << "j = " << j << "отмечена как не прилегающая к дому";
        }
    }

    for (int k = 0; k < 2; k++)
    {
        int j = xCell - 1 + k * (widthModel + 1);
        if (j < 0 || size_t(j) > getWidth() - 1)
            continue;

        for (int i = yCell; i < yCell + heightModel; i++)
        {
            if (getUsedCells()[i][j] == 3)
                getUsedCells()[i][j] = 0;
            qDebug() << "i = " << i << "j = " << j << "отмечена как не прилегающая к дому";
        }
    }
}


void CellScene::moveUp(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, -value, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveDown(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, value, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveLeft(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
          -value, 0, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::moveRight(double value)
{
    Eigen::Matrix4f moveMat;
    moveMat << 1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
           value, 0, 0, 1;

    transMatrix *= moveMat;
}

void CellScene::scale(double value)
{
    Eigen::Matrix4f scaleMatrix;
    scaleMatrix << value,     0,     0, 0,
                    0,    value,     0, 0,
                    0,        0, value, 0,
                    0,        0,     0, 1;

    transMatrix *= scaleMatrix;
}

void CellScene::rotateX(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    rotateMatrix << 1,           0,          0, 0,
                    0, cos(angle), -sin(angle), 0,
                    0, sin(angle),  cos(angle), 0,
                    0,           0,          0, 1;

    transMatrix *= rotateMatrix;
}

void CellScene::rotateY(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    rotateMatrix << cos(angle),  0, sin(angle), 0,
                    0,           1,          0, 0,
                    -sin(angle), 0, cos(angle), 0,
                    0,           0,          0, 1;

    transMatrix *= rotateMatrix;
}

void CellScene::rotateZ(double angle)
{
    Eigen::Matrix4f rotateMatrix;
    rotateMatrix << cos(angle),  -sin(angle), 0, 0,
                    sin(angle),   cos(angle), 0, 0,
                    0,            0,          1, 0,
                    0,            0,          0, 1;

    transMatrix *= rotateMatrix;
}

void CellScene::toCenter()
{
    Dot3D start(PLATE_START);

    Eigen::Matrix4f newMat;

    newMat << 1,  0, 0, 0,
              0,  1, 0, 0,
              0,  0, 1, 0,
              0,  0, 0, 1;

    newMat(3, 0) = X_CENTER - start.getXCoordinate() - getWidth() * SCALE_FACTOR / 2;
    newMat(3, 1) = Y_CENTER - start.getYCoordinate() - getHeight() * SCALE_FACTOR / 2;
    newMat(3, 2) = 0;

    transMatrix = newMat;
}


size_t CellScene::getRealModelsNum() { return realModelsNum; }

size_t CellScene::getModelsNum() { return modelsNum; }

PolModel &CellScene::getModel(size_t num) { return models.at(num); }

void CellScene::setModel(size_t num, PolModel &newModel) { models.at(num) = newModel; }


void CellScene::addModel(PolModel &model)
{
    modelsNum++;
    models.push_back(model);

    PolModel::model_t modelType_ = models[modelsNum - 1].getModelType();

    if (modelType_ == PolModel::model_t::windowsHouse || \
        modelType_ == PolModel::model_t::treeTrunk || \
        modelType_ == PolModel::model_t::roadStripe || \
        modelType_ == PolModel::model_t::glassCar)
    {
        markUsedCells(modelsNum - 1);
        realModelsNum++;
        printUsedCells();
    }
}

size_t CellScene::getIllumNum() { return illumNum; }

Illuminant &CellScene::getIlluminant(size_t num) { return illuminants.at(num); }

void CellScene::setIlluminant(Illuminant &illum, size_t i) { illuminants.at(i) = illum; }

void CellScene::addIlluminant(Illuminant &illum)
{
    illumNum++;
    illuminants.push_back(illum);
}

void CellScene::deleteIlluminant(size_t num)
{
    if (num < illuminants.size())
    {
        illumNum--;
        illuminants.erase(illuminants.begin() + num);
    }
}

void CellScene::deleteModel(size_t num)
{
    if (num < models.size())
    {
        PolModel::model_t modelType_ = models[num].getModelType();

        if (modelType_ == PolModel::model_t::windowsHouse || \
            modelType_ == PolModel::model_t::treeTrunk || \
            modelType_ == PolModel::model_t::roadStripe || \
            modelType_ == PolModel::model_t::glassCar)
        {
            clearUsedCells(num);
            realModelsNum--;
        }

        modelsNum--;
        models.erase(models.begin() + num);
        printUsedCells();
    }
}


void CellScene::printUsedCells()
{
    for (size_t i = 0; i < height; i++)
    {
        qDebug() << getUsedCells()[i];
    }
}


PolModel &CellScene::getPlateModel() { return *plateModel; }

void CellScene::buildPlateModel(Dot3D startOfPlate_, Dot3D endOfPlate_)
{
//    if (plateModel)
//        return;

    std::vector<Vertex> vertices;
    std::vector<Facet> facets;

    for (size_t y = startOfPlate_.getYCoordinate(); y < endOfPlate_.getYCoordinate();
         y += SCALE_FACTOR)
        for (size_t x = startOfPlate_.getXCoordinate(); x < endOfPlate_.getXCoordinate();
             x += SCALE_FACTOR)
            addQuad(vertices, facets, x, y, PLATE_Z, x + SCALE_FACTOR, y, PLATE_Z,
            x + SCALE_FACTOR, y + SCALE_FACTOR, PLATE_Z, x, y + SCALE_FACTOR, PLATE_Z);

    addQuad(vertices, facets, startOfPlate_.getXCoordinate(),
    startOfPlate_.getYCoordinate(), PLATE_Z - 10, startOfPlate_.getXCoordinate(),
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    startOfPlate_.getYCoordinate(), PLATE_Z - 10);

    addQuad(vertices, facets, startOfPlate_.getXCoordinate(),
    startOfPlate_.getYCoordinate(), PLATE_Z - 10, startOfPlate_.getXCoordinate(),
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, startOfPlate_.getXCoordinate(),
    endOfPlate_.getYCoordinate() + 10, PLATE_Z, startOfPlate_.getXCoordinate(),
    startOfPlate_.getYCoordinate(), PLATE_Z);

    addQuad(vertices, facets, startOfPlate_.getXCoordinate(),
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    endOfPlate_.getYCoordinate() + 10, PLATE_Z, startOfPlate_.getXCoordinate(),
    endOfPlate_.getYCoordinate() + 10, PLATE_Z);

    addQuad(vertices, facets, endOfPlate_.getXCoordinate() + 10,
    endOfPlate_.getYCoordinate() + 10, PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    startOfPlate_.getYCoordinate(), PLATE_Z - 10, endOfPlate_.getXCoordinate() + 10,
    startOfPlate_.getYCoordinate(), PLATE_Z, endOfPlate_.getXCoordinate() + 10,
    endOfPlate_.getYCoordinate() + 10, PLATE_Z);

    addQuad(vertices, facets, endOfPlate_.getXCoordinate() + 10,
    startOfPlate_.getYCoordinate(), PLATE_Z - 10, startOfPlate_.getXCoordinate(),
    startOfPlate_.getYCoordinate(), PLATE_Z - 10, startOfPlate_.getXCoordinate(),
    startOfPlate_.getYCoordinate(), PLATE_Z, endOfPlate_.getXCoordinate() + 10,
    startOfPlate_.getYCoordinate(), PLATE_Z);
    if (plateModel)
        delete plateModel;
    plateModel = new PolModel(vertices, facets);
}

void CellScene::changeSize(size_t newWidth, size_t newHeight)
{
    if (newWidth)
    {
        if (newWidth < width)
        {
            for (size_t i = 0; i < modelsNum; i++)
            {
                if (getModel(i).getUsedXCell() >= (int)newWidth)
                {
                    deleteModel(i);
                    i--;
                }
            }
        }

        width = newWidth;
    }


    if (newHeight)
    {
        if (newHeight < height)
        {
            for (size_t i = 0; i < modelsNum; i++)
            {
                if (getModel(i).getUsedYCell() >= (int)newHeight)
                {
                    deleteModel(i);
                    i--;
                }
            }
        }

        height = newHeight;
    }
}

Eigen::Matrix4f &CellScene::getTransMatrix() { return transMatrix; }


void CellScene::initUsedCells()
{
    usedCells.resize(getHeight());

    for (size_t i = 0; i < getHeight(); i++)
    {
        usedCells[i].resize(getWidth(), 0);
    }
}


std::vector<std::vector<size_t>> &CellScene::getUsedCells() { return usedCells; }
