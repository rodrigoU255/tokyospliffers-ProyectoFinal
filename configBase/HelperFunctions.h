
#pragma once


enum class CollitionType : int {
	CT_Static,
	CT_Dynamic,
};

class BlenderToBullet {
public:

	//void AssimCustomColliderToBP(class btTriangleMesh* TriangularMesh, struct aiMesh* mesh);

	void  PreLoopCollitionSettings(class bCollitionShape* CollitionShape, class btVector3, CollitionType CollitionType );


};

