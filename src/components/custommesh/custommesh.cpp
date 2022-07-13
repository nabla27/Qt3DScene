#include "custommesh.h"
#include <QDebug> //DEBUG

void GridIndices::setSize(const unsigned int& rowCount, const unsigned int& colCount)
{
    this->rowCount = rowCount;
    this->colCount = colCount;
}

void GridIndices::createRectangleIndices()
{
    //頂点インデックスの数 = 2*(2*(rows*cols) - (rows+cols))
    const unsigned int indexCount = 2 * (2 * rowCount * colCount - rowCount - colCount);

    QByteArray indexArray;
    indexArray.resize(indexCount * sizeof(unsigned int));
    unsigned int *index = reinterpret_cast<unsigned int*>(indexArray.data());

    /*
     *  row-1行,col-1列までの各頂点を起点に、それぞれ行方向、列方向のとなりのインデックスと結んでいく。
     *
     *  起点 ------->
     *      |   ____ ____ ____  col
     *      |  |    |    |
     *      |  |____|____|____
     *      v  |    |    |
     *         |    |    |
     *
     *       row
     */

    const unsigned int row_1 = rowCount - 1;
    const unsigned int col_1 = colCount - 1;

    for(unsigned int i = 0; i < row_1; ++i)
        for(unsigned int j = 0; j < col_1; ++j)
        {
            const unsigned int k = colCount * i + j;
            //kを起点に右(列方向)隣のインデックスと結ぶ
            *index++ = k;
            *index++ = k + 1;
            //kを起点に下(行方向)隣のインデックスと結ぶ
            *index++ = k;
            *index++ = k + colCount;
        }

    /* 余った下端(最終行)のインデックスを結ぶ */
    for(qsizetype j = 0; j < col_1; ++j)
    {
        const unsigned int i = colCount * row_1 + j; //j=0の場合、左下角のインデックス
        *index++ = i;
        *index++ = i + 1;
    }

    /* 余った右端(最終列)のインデックスを結ぶ */
    for(qsizetype i = 0; i < row_1; ++i)
    {
        const unsigned int j = colCount * (i + 1) - 1;
        *index++ = j;
        *index++ = j + colCount;
    }

    emit indicesCreated(indexArray, indexCount);
}

void GridIndices::createSingleLineIndices()
{
    /* * * * * */
    const unsigned int indexCount = colCount * 2 - 2;

    QByteArray indexArray;
    indexArray.resize(indexCount * sizeof(unsigned int));
    unsigned int *index = reinterpret_cast<unsigned int*>(indexArray.data());

    for(unsigned int p = 0; p < colCount - 1; ++p)
    {
        *index++ = p;
        *index++ = p + 1;
    }

    emit indicesCreated(indexArray, indexCount);
}

void GridIndices::createTriangleIndices()
{
    /*
     *
     *                           col(X) ----------->
     *            1         3,4
     *             |----<----|---------|------
     *             |        /|        /|
     *             |       / |       / |
     *             |      /  |      /  |
     *             |     /   ^     /   |
     *             V    /    |    /    |    /
     *     row(Z)  |   /     |   /     |   /
     *       |     |  /      |  /      |  /
     *       |     | /       | /       | /
     *       |  2,5|---->----|---------|------
     *       |     |         6
     *       |     |
     *       V     |
     *
     *
     */

    const unsigned int rowBoxCount = rowCount - 1;
    const unsigned int colBoxCount = colCount - 1;
    const unsigned int indexCount = rowBoxCount * colBoxCount * 6;

    QByteArray indexArray;
    indexArray.resize(3 * indexCount * sizeof(unsigned int)); //(x,y,z) * indexCount * unsigned int
    unsigned int *i = reinterpret_cast<unsigned int*>(indexArray.data());

    for(unsigned int row = 0; row < rowBoxCount; ++row)
        for(unsigned int col = 0; col < colBoxCount; ++col)
        {
            const unsigned int leftTop = row * colCount + col;
            const unsigned int leftBottom = leftTop + colCount;

            *i++ = leftTop;         //1
            *i++ = leftBottom;      //2
            *i++ = leftTop + 1;     //3

            *i++ = leftTop + 1;     //4
            *i++ = leftBottom;      //5
            *i++ = leftBottom + 1;  //6
        }

    emit indicesCreated(indexArray, indexCount);
}











#include <QVector3D>
#include <algorithm>
const QList<QVector3D> GridColorVertex::turboColorMap = QList<QVector3D>({
                                                                             QVector3D(0.0f, 0.0f, 0.5f),
                                                                             QVector3D(0.0f, 0.0f, 0.9f),
                                                                             QVector3D(0.0f, 0.3f, 1.0f),
                                                                             QVector3D(0.0f, 0.7f, 1.0f),
                                                                             QVector3D(0.2f, 1.0f, 0.8f),
                                                                             QVector3D(0.5f, 1.0f, 0.5f),
                                                                             QVector3D(0.8f, 1.0f, 0.2f),
                                                                             QVector3D(1.0f, 0.8f, 0.0f),
                                                                             QVector3D(1.0f, 0.4f, 0.0f),
                                                                             QVector3D(0.9f, 0.0f, 0.0f),
                                                                             QVector3D(0.5f, 0.0f, 0.0f)
                                                                         });
const QList<QVector3D> GridColorVertex::autumnColorMap = QList<QVector3D>({
                                                                              QVector3D(1.0f, 0.00f, 0.0f),
                                                                              QVector3D(1.0f, 0.25f, 0.0f),
                                                                              QVector3D(1.0f, 0.50f, 0.0f),
                                                                              QVector3D(1.0f, 0.75f, 0.0f),
                                                                              QVector3D(1.0f, 1.00f, 0.0f)
                                                                          });

void GridColorVertex::createColorVertex(const QByteArray& array)
{
    switch(colorMapType)
    {
    case ColorMapType::Turbo:
    {
        createColorVertexFromMap(array, turboColorMap);
        return;
    }
    case ColorMapType::Autumn:
    {
        createColorVertexFromMap(array, autumnColorMap);
    }
    default:
        return;
    }
}

void GridColorVertex::createColorVertexFromMap(const QByteArray &array, const QList<QVector3D> &map)
{
    const unsigned int vertexCount = rowCount * colCount;

    QByteArray colorVertex;
    colorVertex.resize(3 * vertexCount * sizeof(float)); //(r,g,b) * vertexCount * float
    float *c = reinterpret_cast<float*>(colorVertex.data());
    const float *p = reinterpret_cast<const float*>(array.data());

    const unsigned int maxColorIndex = map.size() - 1;

    for(unsigned int iz = 0; iz < rowCount; ++iz)
        for(unsigned int ix = 0; ix < colCount; ++ix)
        {
            const float y = p[(iz * colCount + ix) * 3 + 1];
            const float normalizedValue = (y - minValue) / (maxValue - minValue); //y値を[minValue,maxValue]の間で正規化した値0~1

            const float colorIndex = normalizedValue * maxColorIndex;
            const float complement = colorIndex - (unsigned int)colorIndex; //colorIndexの小数以下
            const unsigned int colorIndexU = std::min((unsigned int)colorIndex, maxColorIndex);
            const QVector3D color = map.at(colorIndexU) + (map.at(std::min(colorIndexU + 1, maxColorIndex)) - map.at(colorIndexU)) * complement;

            *c++ = color.x(); //R
            *c++ = color.y(); //G
            *c++ = color.z(); //B
        }

    emit colorVertexCreated(colorVertex, vertexCount);
}










#include <QVector3D>
void GridNormalVertex::createNormalVertex(const QByteArray& array)
{
    /*
     *
     *
     *
     *              |--------1|--------6|---------|---------|
     *              |       / |       / |       / |       /
     *              |      /  |      /  |      /  |      /
     *              |     /   |     /   |     /   |     /
     *              |    /    |    /    |    /    |    /
     *              |   /     |   /     |   /     |   /
     *              |  /      |  /      |  /      |  /
     *              | /       | /       | /       | /
     *              |2--------|--------5|---------|---------
     *              |       /0|       / |       / |       /
     *              |      /  |      /  |      /  |      /
     *              |     /   |     /   |     /   |     /
     *              |    /    |    /    |    /    |    /
     *              |   /     |   /     |   /     |   /
     *              |  /      |  /      |  /      |  /
     *              | /       | /       | /       | /
     *              |3-------4|---------|---------|---------
     *              |       / |       / |       / |       /
     *              |      /  |      /  |      /  |      /
     *              |     /   |     /   |     /   |     /
     *              |    /    |    /    |    /    |    /
     *              |   /     |   /     |   /     |   /
     *              |  /      |  /      |  /      |  /
     *              | /       | /       | /       | /
     *              |---------|---------|---------|--------
     *
     *
     */

    const unsigned int vertexCount = rowCount * colCount;

    QByteArray normalVertex;
    normalVertex.resize(3 * vertexCount * sizeof(float)); //(nx,ny,nz) * vertexCount * float
    float *n = reinterpret_cast<float*>(normalVertex.data());
    const float *p = reinterpret_cast<const float*>(array.data());

    //頂点が共有している面法線ベクトルを格納．最大でも一つの頂点で共有されている面は6つ
    QVector<QVector3D> facesNormals;
    facesNormals.reserve(6);
    //対象頂点の周囲の頂点
    QVector3D v0; //
    QVector3D v1; //iz>0
    QVector3D v2; //ix>0
    QVector3D v3; //iz<rowCount-1, ix>0
    QVector3D v4; //iz<rowCount-1
    QVector3D v5; //ix<colCount-1
    QVector3D v6; //ix<colCount-1, iz>0

    for(unsigned int iz = 0; iz < rowCount; ++iz)
    {
        for(unsigned int ix = 0; ix < colCount; ++ix)
        {
            const bool isNotLeft = ix > 0;
            const bool isNotRight = ix < colCount - 1;
            const unsigned int index = 3 * (iz * colCount + ix); //点(iz,ix)のベクトルのx座標

            /* 周囲の頂点座標を取得 */
            v0 = QVector3D(p[index], p[index + 1], p[index + 2]);
            if(iz > 0)
            {
                const unsigned int upIndex = index - colCount * 3;
                v1 = QVector3D(p[upIndex], p[upIndex + 1], p[upIndex + 2]);
                if(isNotRight)
                    v6 = QVector3D(p[upIndex + 3], p[upIndex + 4], p[upIndex + 5]);
            }
            if(iz < rowCount - 1)
            {
                const unsigned int downIndex = index + colCount * 3;
                v4 = QVector3D(p[downIndex], p[downIndex + 1], p[downIndex + 2]);
                if(isNotLeft)
                    v3 = QVector3D(p[downIndex - 3], p[downIndex - 2], p[downIndex - 1]);
            }
            if(isNotLeft)
                v2 = QVector3D(p[index - 3], p[index - 2], p[index - 1]);
            if(isNotRight)
                v5 = QVector3D(p[index + 3], p[index + 4], p[index + 5]);


            /* 頂点を共有する面の面法線ベクトルを求め，リストに収納 */
            if(iz > 0)
            {
                if(isNotLeft)
                {
                    facesNormals << QVector3D::normal(v0, v1, v2);
                }
                if(isNotRight)
                {
                    facesNormals << QVector3D::normal(v0, v5, v6);
                    facesNormals << QVector3D::normal(v0, v6, v1);
                }
            }
            if(iz < rowCount - 1)
            {
                if(isNotLeft)
                {
                    facesNormals << QVector3D::normal(v0, v2, v3);
                    facesNormals << QVector3D::normal(v0, v3, v4);
                }
                if(isNotRight)
                {
                    facesNormals << QVector3D::normal(v0, v4, v5);
                }
            }

            /* 頂点で共有されている面の面法線ベクトルの和 */
            QVector3D vertexNormals;
            for(const QVector3D& faceNormal : facesNormals)
            {
                vertexNormals += faceNormal;
            }

            /* 頂点法線ベクトル = 面法線ベクトルの和を正規化 */
            vertexNormals.normalize();

            *n++ = vertexNormals.x();
            *n++ = vertexNormals.y();
            *n++ = vertexNormals.z();

            facesNormals.clear();
        }
    }

    emit normalVertexCreated(normalVertex, vertexCount);
}






