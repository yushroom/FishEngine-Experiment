#include <FishEngine/Physics/Physics.hpp>
#include <FishEngine/Physics/PhysxAPI.hpp>

using namespace physx;

namespace FishEngine
{
	void PxVec3ToVector3(const PxVec3& v1, Vector3& v2)
	{
		v2.x = v1.x;
		v2.y = v1.y;
		v2.z = v1.z;
	}


	bool Physics::Raycast(const Vector3 & origin, const Vector3 & direction, RaycastHit & hitInfo, float maxDistance, int layerMask)
	{
		//PxRaycastHit px_hitInfo;
		//PxU32 maxHits = 1;
		PxHitFlags px_hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE | PxHitFlag::eUV;
		PxVec3 px_origin = { origin.x, origin.y, origin.z };
		PxVec3 px_unitDir = { direction.x, direction.y, direction.z };
		px_unitDir.normalize();
		PxRaycastBuffer px_hit;	// out

		//PxU32 hitCount = PxGeometryQuery::raycast()
		auto& p = PhysxWrap::GetInstance();
		bool status = p.scene->raycast(px_origin, px_unitDir, maxDistance, px_hit, px_hitFlags);

		if (status)
		{
			hitInfo.m_Collider = (Collider*)px_hit.block.shape->userData;
			PxVec3ToVector3(px_hit.block.position, hitInfo.m_Point);
			PxVec3ToVector3(px_hit.block.normal, hitInfo.m_Normal);
			hitInfo.m_UV.x = px_hit.block.u;
			hitInfo.m_UV.y = px_hit.block.v;
		}
		return status;
	}
}