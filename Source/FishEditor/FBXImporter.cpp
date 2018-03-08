#include <FishEditor/FBXImporter.hpp>

#include <FishEngine/Debug.hpp>

#include <fbxsdk.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include <deque>
#include <cassert>

#include "RawMesh.hpp"

using namespace FishEngine;
using namespace FishEditor;

inline Vector3 FbxVector4ToVector3WithXFlipped(FbxVector4 const & v)
{
	return Vector3(static_cast<float>(-v[0]),
				   static_cast<float>(v[1]),
				   static_cast<float>(v[2]) );
}


Mesh* FishEditor::FBXImporter::ParseMesh(FbxMesh* fbxMesh)
{
	assert(fbxMesh->IsTriangleMesh());
	
	fbxMesh->RemoveBadPolygons();
	fbxMesh->GenerateNormals(false, true, false);
	fbxMesh->GenerateTangentsDataForAllUVSets();
	
	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__cpp_ref_class_fbx_mesh_html
	// A control point is an XYZ coordinate, it is synonym of vertex.
	// A polygon vertex is an index to a control point(the same control point can be referenced by multiple polygon vertices).
	// A polygon is a group of polygon vertices.The minimum valid number of polygon vertices to define a polygon is 3.
	
	int polygonCount = fbxMesh->GetPolygonCount();
	int vertexCount = fbxMesh->GetControlPointsCount();
	FbxVector4* controlPoints = fbxMesh->GetControlPoints();
	
	// at least has 1 set of uv
	assert(fbxMesh->GetElementUVCount() != 0);
	
	// get material info
	int lMaterialCount = fbxMesh->GetNode()->GetMaterialCount();
	assert(lMaterialCount != 0);
	
	// use RawMesh to construct Mesh
	RawMesh rawMesh;
	rawMesh.SetFaceCount(polygonCount);
	rawMesh.SetVertexCount(vertexCount);
	
	int subMeshCount = lMaterialCount;
	int hasSubMesh = subMeshCount > 1;
	
	// TODO:
	// if this mesh only has one material, we assume this material applies to all polygons.
	// Better choice is to apply default material to polygons without materials.

	// use material info to split submeshes
	if (hasSubMesh)
	{
		rawMesh.m_subMeshCount = subMeshCount;
		rawMesh.m_submeshMap.resize(polygonCount);
		for (int l = 0; l < fbxMesh->GetElementMaterialCount(); l++)
		{
			auto lMaterialElement = fbxMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxLayerElement::eByPolygon)
			{
				if (lMaterialElement->GetReferenceMode() == FbxLayerElement::eIndex ||
					lMaterialElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					int lIndexArrayCount = lMaterialElement->GetIndexArray().GetCount();
					if (lIndexArrayCount != polygonCount)
					{
						abort();
					}
					for (int i = 0; i < polygonCount; i++)
					{
						int lMatId = lMaterialElement->GetIndexArray().GetAt(i);
						rawMesh.m_submeshMap[i] = lMatId;
					}
				}
				else
				{
					abort();
				}
			}
			else
			{
				abort();
			}
		}
	}
	
	// positions
	for (int controlPointIndex = 0; controlPointIndex < vertexCount; ++controlPointIndex)
	{
		auto & p = controlPoints[controlPointIndex];
		float scale = m_fileScale * m_globalScale;
		auto pp = FbxVector4ToVector3WithXFlipped(p);
		rawMesh.m_vertexPositions.emplace_back(pp * scale);
	}
	
	// indices(triangles)
	for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 0) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 1) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 2) );
	}
	
	int vertexId = 0;
	for (int i = 0; i < polygonCount; i++)	// for each triangle
	{
		int lPolygonSize = fbxMesh->GetPolygonSize(i); // should be 3
		assert(lPolygonSize == 3);
		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);
			
			// UV
			//			for (int l = 0; l < fbxMesh->GetElementUVCount(); ++l)
			//			{
			FbxGeometryElementUV* leUV = fbxMesh->GetElementUV(0);
			int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
			if (leUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				auto mode = leUV->GetReferenceMode();
				if (mode == FbxGeometryElement::eDirect || mode == FbxGeometryElement::eIndexToDirect)
				{
					auto uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
					rawMesh.m_wedgeTexCoords.emplace_back((float)uv[0], (float)uv[1]);
				}
				else
				{
					abort();
				}
			}
			else
			{
				abort();
			}
			//			}
			
			// Normal
			//for (int l = 0; l < fbxMesh->GetElementNormalCount(); ++l)
			//{
			FbxGeometryElementNormal* leNormal = fbxMesh->GetElementNormal(0);
			if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				auto mode = leNormal->GetReferenceMode();
				if (mode == FbxGeometryElement::eDirect)
				{
					auto n = leNormal->GetDirectArray().GetAt(vertexId);
					rawMesh.m_wedgeNormals.emplace_back(FbxVector4ToVector3WithXFlipped(n));
				}
				else if (mode == FbxGeometryElement::eIndexToDirect)
				{
					int id = leNormal->GetIndexArray().GetAt(vertexId);
					auto n = leNormal->GetDirectArray().GetAt(id);
					rawMesh.m_wedgeNormals.emplace_back(FbxVector4ToVector3WithXFlipped(n));
				}
				else
				{
					abort();
				}
			}
			else if (leNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				auto mode = leNormal->GetReferenceMode();
				if (mode == FbxGeometryElement::eDirect)
				{
					auto n = leNormal->GetDirectArray().GetAt(lControlPointIndex);
					rawMesh.m_wedgeNormals.emplace_back(FbxVector4ToVector3WithXFlipped(n));
				}
				else if (mode == FbxGeometryElement::eIndexToDirect)
				{
					int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
					auto n = leNormal->GetDirectArray().GetAt(id);
					rawMesh.m_wedgeNormals.emplace_back(FbxVector4ToVector3WithXFlipped(n));
				}
				else
				{
					abort();
				}
			}
			else
			{
				abort();
			}
			//}
			
			// Tangent
			//for (int l = 0; l < fbxMesh->GetElementTangentCount(); ++l)
			//{
			FbxGeometryElementTangent* leTangent = fbxMesh->GetElementTangent(0);
			if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				auto mode = leTangent->GetReferenceMode();
				if (mode == FbxGeometryElement::eDirect)
				{
					auto t = leTangent->GetDirectArray().GetAt(vertexId);
					rawMesh.m_wedgeTangents.emplace_back(FbxVector4ToVector3WithXFlipped(t));
				}
				else if (mode == FbxGeometryElement::eIndexToDirect)
				{
					int id = leTangent->GetIndexArray().GetAt(vertexId);
					auto t = leTangent->GetDirectArray().GetAt(id);
					rawMesh.m_wedgeTangents.emplace_back(FbxVector4ToVector3WithXFlipped(t));
				}
				else
				{
					abort();
				}
			}
			else if (leTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				auto mode = leTangent->GetReferenceMode();
				if (mode == FbxGeometryElement::eDirect)
				{
					auto t = leTangent->GetDirectArray().GetAt(lControlPointIndex);
					rawMesh.m_wedgeTangents.emplace_back(FbxVector4ToVector3WithXFlipped(t));
				}
				else if (mode == FbxGeometryElement::eIndexToDirect)
				{
					int id = leTangent->GetIndexArray().GetAt(lControlPointIndex);
					auto t = leTangent->GetDirectArray().GetAt(id);
					rawMesh.m_wedgeTangents.emplace_back(FbxVector4ToVector3WithXFlipped(t));
				}
				else
				{
					abort();
				}
			}
			else
			{
				abort();
			}
			//}
			vertexId++;
		} // for polygonSize
	} // for polygonCount
	
	auto mesh = rawMesh.ToMesh();
//	GetLinkData(fbxMesh, mesh, rawMesh.m_vertexIndexRemapping);
	
//	m_model.m_fbxMeshLookup[fbxMesh] = m_model.m_meshes.size();
//	m_model.m_meshes.push_back(mesh);
	
	return mesh;
}

void FishEditor::FBXImporter::BakeTransforms(FbxScene * scene)
{
	// FBX stores transforms in a more complex way than just translation-rotation-scale as used by Banshee.
	// Instead they also support rotations offsets and pivots, scaling pivots and more. We wish to bake all this data
	// into a standard transform so we can access it using node's local TRS properties (e.g. FbxNode::LclTranslation).
	
	// fbx sdk doc:
	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_C35D98CB_5148_4B46_82D1_51077D8970EE_htm
	
	double frameRate = FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());
	
	std::deque<FbxNode*> todo;
	todo.push_back(scene->GetRootNode());
	
	while (todo.size() > 0)
	{
		FbxNode* node = todo.front();
		todo.pop_front();
		
		FbxVector4 zero(0, 0, 0);
		FbxVector4 one(1, 1, 1);
		
		// Activate pivot converting
		node->SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive);
		node->SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive);
		
		// We want to set all these to 0 (1 for scale) and bake them into the transforms
		node->SetPostRotation(FbxNode::eDestinationPivot, zero);
		node->SetPreRotation(FbxNode::eDestinationPivot, zero);
		node->SetRotationOffset(FbxNode::eDestinationPivot, zero);
		node->SetScalingOffset(FbxNode::eDestinationPivot, zero);
		node->SetRotationPivot(FbxNode::eDestinationPivot, zero);
		node->SetScalingPivot(FbxNode::eDestinationPivot, zero);
		node->SetGeometricTranslation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricRotation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricScaling(FbxNode::eDestinationPivot, one);
		
		// FishEngine assumes euler angles are in ZXY order
		// do not use this line, since it will mess up animation curves
		//node->SetRotationOrder(FbxNode::eDestinationPivot, eEulerZXY);
		
		EFbxRotationOrder lRotationOrder;
		node->GetRotationOrder(FbxNode::eSourcePivot , lRotationOrder);
		node->SetRotationOrder(FbxNode::eDestinationPivot , lRotationOrder);
		
		// Keep interpolation as is
		node->SetQuaternionInterpolation(FbxNode::eDestinationPivot, node->GetQuaternionInterpolation(FbxNode::eSourcePivot));
		
		for (int i = 0; i < node->GetChildCount(); i++)
		{
			FbxNode* childNode = node->GetChild(i);
			todo.push_back(childNode);
		}
	}
	
	scene->GetRootNode()->ConvertPivotAnimationRecursive(nullptr, FbxNode::eDestinationPivot, frameRate);
	//scene->GetRootNode()->ResetPivotSetAndConvertAnimation();
}

void FishEditor::FBXImporter::ParseNode(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();
	auto nodeAttributeCount = pNode->GetNodeAttributeCount();
	for (int i = 0; i < nodeAttributeCount; ++i)
	{
		auto nodeAttribute = pNode->GetNodeAttributeByIndex(i);
		auto type = nodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
			auto mesh = ParseMesh(lMesh);
//			m_model.m_meshes.push_back(mesh);
//			if (mesh->name().empty())
//			{
//				mesh->setName(nodeName);
//			}
			mesh->name = nodeName;
			m_meshes[mesh->name] = mesh;
//
//			if (IsNewlyCreated())
//			{
//				m_recycleNameToFileID[mesh->name()] = m_nextMeshFileID;
//				m_fileIDToRecycleName[m_nextMeshFileID] = mesh->name();
//				m_nextMeshFileID += 2;
//			}
			
//			int lMaterialCount = pNode->GetMaterialCount();
//			MaterialPtr material;
//			if (lMaterialCount == 0)
//			{
//				material = Material::defaultMaterial();
//			}
//			else
//			{
//				auto lMaterial = pNode->GetMaterial(0);
//				material = ParseMaterial(lMaterial);
//			}
//
//			RendererPtr renderer;
//			if (mesh->m_skinned)
//			{
//				auto srenderer = go->AddComponent<SkinnedMeshRenderer>();
//				m_model.m_skinnedMeshRenderers.push_back(srenderer);
//				srenderer->SetMaterial(material);
//				srenderer->setSharedMesh(mesh);
//				srenderer->setAvatar(m_model.m_avatar);
//				srenderer->setRootBone(m_model.m_rootNode->transform());
//				renderer = srenderer;
//			}
//			else
//			{
//				go->AddComponent<MeshFilter>()->SetMesh(mesh);
//				renderer = go->AddComponent<MeshRenderer>();
//				renderer->SetMaterial(material);
//			}
//
//			for (int i = 1; i < lMaterialCount; ++i)
//			{
//				auto lMaterial = pNode->GetMaterial(i);
//				material = ParseMaterial(lMaterial);
//				renderer->AddMaterial(material);
//			}
		}
	}
	
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		ParseNode(pNode->GetChild(j));
//		child->transform()->SetParent(go->transform(), false);
	}
}

void FishEditor::FBXImporter::Import(const std::string& path)
{
	// Initialize the SDK manager. This object handles memory management.
	FbxManager * lSdkManager = FbxManager::Create();
	
	// Create the IO settings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	
	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	
	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(path.c_str(), -1, lSdkManager->GetIOSettings()))
	{
		LogError("Call to FbxImporter::Initialize() failed.");
		LogError(std::string( "Error returned: ") + lImporter->GetStatus().GetErrorString() );
		abort();
	}
	
	// Create a new scene so that it can be populated by the imported file.
	FbxScene * lScene = FbxScene::Create(lSdkManager, "FishEngineImporter");
	
	// Import the contents of the file into the scene.
	lImporter->Import(lScene);
	
	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	
//	//FbxAxisSystem::MayaYUp.ConvertScene(lScene);
	FbxAxisSystem::DirectX.ConvertScene(lScene); // wrong!
//	// FbxSystemUnit::m.ConvertScene(lScene);
//	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_CC93340E_C4A1_49EE_B048_E898F856CFBF_htm
//	// do NOT use FbxSystemUnit::ConvertScene(lScene), which just simply set transform.scale of root nodes.
	
	auto unit = lScene->GetGlobalSettings().GetSystemUnit();
	if ( unit == FbxSystemUnit::mm )
	{
		m_fileScale = 0.001f;
	}
	else if ( unit == FbxSystemUnit::dm)
	{
		m_fileScale = 0.1f;
	}
	else if (unit == FbxSystemUnit::cm)
	{
		m_fileScale = 0.01f;
	}
	else if (unit == FbxSystemUnit::m)
	{
		m_fileScale = 1.0f;
	}
	else
	{
		abort();
	}
	
	FbxGeometryConverter converter(lSdkManager);
	converter.Triangulate(lScene, true);
	BakeTransforms(lScene);
	
	FbxNode* lRootNode = lScene->GetRootNode();
	ParseNode(lRootNode);
	
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();
}

