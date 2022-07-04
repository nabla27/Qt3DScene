#include "utility.h"

QIcon IconSet::mesh3d = QIcon();
QIcon IconSet::object3d = QIcon();
QIcon IconSet::surface3d = QIcon();
QIcon IconSet::transform3d = QIcon();
QIcon IconSet::animationgroup = QIcon();
QIcon IconSet::clone = QIcon();
QIcon IconSet::gear = QIcon();
QIcon IconSet::import = QIcon();
QIcon IconSet::play = QIcon();
QIcon IconSet::plus = QIcon();
QIcon IconSet::sort = QIcon();
QIcon IconSet::trashcan = QIcon();

void IconSet::load()
{
    mesh3d = QIcon(":/icon/3dmesh");
    object3d = QIcon(":/icon/3dobject");
    surface3d = QIcon(":/icon/3dsurface");
    transform3d = QIcon(":/icon/3dtransform");
    animationgroup = QIcon(":/icon/animation-group");

    clone = QIcon(":/icon/clone");
    gear = QIcon(":/icon/gear");
    import = QIcon(":/icon/import");
    play = QIcon(":/icon/play");
    plus = QIcon(":/icon/plus");
    sort = QIcon(":/icon/sort");
    trashcan = QIcon(":/icon/trash-can");
}

