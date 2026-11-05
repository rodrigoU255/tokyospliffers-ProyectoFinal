#include "HelperFunctions.h"
//#include "Model.h"

//Phys
#include <btBulletDynamicsCommon.h>
//
//void BlenderToBullet::AssimCustomColliderToBP(btTriangleMesh* TriangleMesh, aiMesh* mesh) {
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
//        aiFace face = mesh->mFaces[i];
//
//        // Solo procesar si la cara es un triángulo
//        if (face.mNumIndices == 3) {
//            // Obtener los 3 vértices de Assimp
//            aiVector3D v0 = mesh->mVertices[face.mIndices[0]];
//            aiVector3D v1 = mesh->mVertices[face.mIndices[1]];
//            aiVector3D v2 = mesh->mVertices[face.mIndices[2]];
//
//            // Convertirlos al tipo de dato matemático de Bullet
//            btVector3 btV0(v0.x, v0.y, v0.z);
//            btVector3 btV1(v1.x, v1.y, v1.z);
//            btVector3 btV2(v2.x, v2.y, v2.z);
//
//            // Agregar el triángulo al mesh de colisión
//            TriangleMesh->addTriangle(btV0, btV1, btV2, true);
//        }
//    }
//}