#ifndef ECSTRUCT_H
#define ECSTRUCT_H

#include <QObject>




struct ECStruct : public QObject
{
    Q_OBJECT

public:
    static constexpr int enumStride = 100;
    static constexpr int enumOffset = 1000;
    enum class EntityType { Empty, Object2D, Object3D, Camera };
    enum class EntitySet
    {
        /* Empty */
        Empty = 1000,

        /* Object2D */
        Text2D = 1100, Line,

        /* Object3D */
        Cone = 1200, Cuboid, Cylinder, ExtrudedText, Plane, Sphere, Torus,

        /* Camera */
        FirstPersonCamera = 1300, OrbitCamera,
    };

    enum class ComponentType { Transform, Material, Mesh, Animation, Plot };
    enum class ComponentsSet
    {
        /* Transform */
        Transform = 1000,

        /* Material */
        DiffuseMapMaterial = 1100, DiffuseSpecularMapMaterial, DiffuseSpecularMaterial, GoochMaterial, MetalRougthMaterial, MorphPhongMaterial,
        NormalDiffuseMapMaterial, NormalDiffuseSpecularMapMaterial, PerVertexColorMaterial, PhongAlphaMaterial, PhongMaterial, TextureMaterial,

        /* Mesh */
        ConeMesh = 1200, CuboidMesh, CylinderMesh, ExtrudedTextMesh, PlaneMesh, SphereMesh, TorusMesh, LineMesh, LineAxis,

        /* Animation */
        BasicAnimation = 1300, GroupAnimation,

        /* Plot */
        GridMesh = 1400, SurfaceMesh, ExplicitFuncLine, Vector3D,
    };


    Q_ENUM(EntityType)
    Q_ENUM(EntitySet)
    Q_ENUM(ComponentType)
    Q_ENUM(ComponentsSet)



    ///* ComponentSet -> QComponent */
    //template <ComponentsSet N> struct toClass { using type = QComponent; };
    //template<> struct toClass<ComponentsSet::Transform> { using type = QTransform; };

    //template<> struct toClass<ComponentsSet::DiffuseMapMaterial> { using type = QDiffuseMapMaterial; };
    //template<> struct toClass<ComponentsSet::DiffuseSpecularMaterial> { using type = QDiffuseSpecularMapMaterial; };
    //template<> struct toClass<ComponentsSet::GoochMaterial> { using type = QDiffuseSpecularMaterial; };
    //template<> struct toClass<ComponentsSet::MetalRougthMaterial> { using type = QGoochMaterial; };
    //template<> struct toClass<ComponentsSet::MorphPhongMaterial> { using type = QMorphPhongMaterial; };
    //template<> struct toClass<ComponentsSet::NormalDiffuseMapMaterial> { using type = QNormalDiffuseMapMaterial; };
    //template<> struct toClass<ComponentsSet::NormalDiffuseSpecularMapMaterial> { using type = QNormalDiffuseSpecularMapMaterial; };
    //template<> struct toClass<ComponentsSet::PerVertexColorMaterial> { using type = QPerVertexColorMaterial; };
    //template<> struct toClass<ComponentsSet::PhongAlphaMaterial> { using type = QPhongAlphaMateiral; };
    //template<> struct toClass<ComponentsSet::PhongMaterial> { using type = QPhongMaterial; };
    //template<> struct toClass<ComponentsSet::TextureMaterial> { using type = QTextureMaterial; };

    //template<> struct toClass<ComponentsSet::ConeMesh> { using type = QConeMesh; };
    //template<> struct toClass<ComponentsSet::CuboidMesh> { using type = QCuboidMesh; };
    //template<> struct toClass<ComponentsSet::CylinderMesh> { using type = QCylinderMesh; };
    //template<> struct toClass<ComponentsSet::ExtrudedTextMesh> { using type = QExtrudedTextMesh; };
    //template<> struct toClass<ComponentsSet::PlaneMesh> { using type = QPlaneMesh; };
    //template<> struct toClass<ComponentsSet::SphereMesh> { using type = QSphereMesh; };
    //template<> struct toClass<ComponentsSet::TorusMesh> { using type = QTorusMesh; };
};












#endif // ECSTRUCT_H
