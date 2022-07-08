#include "custommesh.h"

void GridIndices::createSquareIndices(const unsigned int& rows,
                                            const unsigned int& cols)
{
    //頂点インデックスの数 = 2*(2*(rows*cols) - (rows+cols))
    const unsigned int indexCount = 2 * (2 * rows * cols - rows - cols);

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

    const unsigned int row_1 = rows - 1;
    const unsigned int col_1 = cols - 1;

    for(unsigned int i = 0; i < row_1; ++i)
        for(unsigned int j = 0; j < col_1; ++j)
        {
            const unsigned int k = cols * i + j;
            //kを起点に右(列方向)隣のインデックスと結ぶ
            *index++ = k;
            *index++ = k + 1;
            //kを起点に下(行方向)隣のインデックスと結ぶ
            *index++ = k;
            *index++ = k + cols;
        }

    /* 余った下端(最終行)のインデックスを結ぶ */
    for(qsizetype j = 0; j < col_1; ++j)
    {
        const unsigned int i = cols * row_1 + j; //j=0の場合、左下角のインデックス
        *index++ = i;
        *index++ = i + 1;
    }

    /* 余った右端(最終列)のインデックスを結ぶ */
    for(qsizetype i = 0; i < row_1; ++i)
    {
        const unsigned int j = cols * (i + 1) - 1;
        *index++ = j;
        *index++ = j + cols;
    }

    emit created(indexArray, indexCount);
}
