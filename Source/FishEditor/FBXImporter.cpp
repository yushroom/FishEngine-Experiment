#include <FishEditor/FBXImporter.hpp>

#include <FishEngine/Debug.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Color.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/Prefab.hpp>

#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>

#include <fbxsdk.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include <boost/filesystem/path.hpp>

#include <deque>
#include <cassert>
#include <iostream>

#include "RawMesh.hpp"

using namespace FishEngine;
using namespace FishEditor;


FBXImporter::~FBXImporter()
{
//	puts("FBXImporter::~FBXImporter()");
	for (auto mesh : m_model.m_meshes)
	{
		delete mesh;
	}
	if (m_model.m_prefab != nullptr)
		delete m_model.m_prefab;
}


Matrix4x4 FBXToNativeType(fbxsdk::FbxAMatrix const & fmatrix)
{
	float f44[4][4];
	auto d44 = fmatrix.Double44();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			f44[i][j] = static_cast<float>(d44[j][i]);
		}
	}
	return Matrix4x4(f44);
}

Vector3 FBXToNativeType(const FbxVector4& value)
{
	Vector3 native;
	native.x = (float)value[0];
	native.y = (float)value[1];
	native.z = (float)value[2];

	return native;
}

Vector3 FBXToNativeType(const FbxDouble3& value)
{
	return Vector3(static_cast<float>(value[0]),
				   static_cast<float>(value[1]),
				   static_cast<float>(value[2]));
}

Vector2 FBXToNativeType(const FbxVector2& value)
{
	Vector2 native;
	native.x = (float)value[0];
	native.y = (float)value[1];

	return native;
}

Color FBXToNativeType(const FbxColor& value)
{
	Color native;
	native.r = (float)value[0];
	native.g = (float)value[1];
	native.b = (float)value[2];
	native.a = (float)value[3];

	return native;
}

FbxSurfaceMaterial* FBXToNativeType(FbxSurfaceMaterial* const& value)
{
	return value;
}


RotationOrder FBXToNativeType(EFbxRotationOrder order)
{
	RotationOrder rotationOrder;
	//EFbxRotationOrder order;
	//node->GetRotationOrder(FbxNode::eDestinationPivot, order);
	switch (order)
	{
		case eEulerXYZ:
			rotationOrder = RotationOrder::XYZ; break;
		case eEulerXZY:
			rotationOrder = RotationOrder::XZY; break;
		case eEulerYXZ:
			rotationOrder = RotationOrder::YXZ; break;
		case eEulerYZX:
			rotationOrder = RotationOrder::YZX; break;
		case eEulerZXY:
			rotationOrder = RotationOrder::ZXY; break;
		case eEulerZYX:
			rotationOrder = RotationOrder::ZYX; break;
		default: // do not support eSphericXYZ
			abort();
	}
	return rotationOrder;
}

int FBXToNativeType(const int & value)
{
	return value;
}


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
		float scale = GetScale();
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
	mesh->SetName(fbxMesh->GetName());
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


std::string UpdateFileIDMap(std::map<std::string, FishEngine::GameObject*>& fileID2GO, const char* name, GameObject* go)
{
	std::string ret;
	auto it = fileID2GO.find(name);
	if (it == fileID2GO.end())
	{
		fileID2GO[name] = go;
		ret = name;
	}
	else
	{
		int postfix = 1;
		while (true)
		{
			std::string new_name = name;
			new_name += " " + std::to_string(postfix);
			auto it = fileID2GO.find(new_name);
			if (it == fileID2GO.end())
			{
				fileID2GO[new_name] = go;
				ret = new_name;
				go->SetName(new_name);
				break;
			}
			postfix++;
		}
	}
	return ret;
}


GameObject* FishEditor::FBXImporter::ParseNode(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();
//	std::cout << "FBXImporter::ParseNode " << nodeName << std::endl;

	auto go = new GameObject(nodeName);
	go->SetPrefabInternal(m_model.m_prefab);
//	m_model.m_objects[GameObject::ClassID][nodeName] = go;
	auto actual_name = UpdateFileIDMap(m_model.m_gameObjects, nodeName, go);
//	m_model.m_m_gameObjectsobjects[Transform::ClassID][actual_name] = go->GetTransform();

	FbxDouble3 t = pNode->LclTranslation.Get();
	FbxDouble3 r = pNode->LclRotation.Get();
	FbxDouble3 s = pNode->LclScaling.Get();

	float scale = GetScale();

	// note: x is flipped
	go->GetTransform()->SetLocalPosition(-t[0] * scale, t[1] * scale, t[2] * scale);
	go->GetTransform()->SetLocalScale(s[0], s[1], s[2]);

	EFbxRotationOrder rotationOrder;
	pNode->GetRotationOrder(FbxNode::eSourcePivot, rotationOrder);
	RotationOrder order = FBXToNativeType(rotationOrder);
	Quaternion rot = Quaternion::Euler(order, r[0], -r[1], -r[2]);
	go->GetTransform()->SetLocalRotation(rot);
	m_model.m_fbxNodeLookup[pNode] = go->GetTransform();

	auto nodeAttributeCount = pNode->GetNodeAttributeCount();
	for (int i = 0; i < nodeAttributeCount; ++i)
	{
		auto nodeAttribute = pNode->GetNodeAttributeByIndex(i);
		auto type = nodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
			auto mesh = ParseMesh(lMesh);
			m_model.m_meshes.push_back(mesh);
//			if (mesh->name().empty())
//			{
//				mesh->setName(nodeName);
//			}
			mesh->SetName(nodeName);

			auto mf = new MeshFilter();
			go->AddComponent(mf);
			mf->SetMesh(mesh);

			auto mr = new MeshRenderer();
			go->AddComponent(mr);
			mr->SetMaterial(Material::GetDefaultMaterial());

//			m_meshes[nodeName] = mesh;
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

	for (auto comp : go->GetAllComponents())
	{
		comp->SetPrefabInternal(m_model.m_prefab);
	}
	
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		auto child = ParseNode(pNode->GetChild(j));
		child->GetTransform()->SetParent(go->GetTransform(), false);
	}
	return go;
}

void FishEditor::FBXImporter::Import(const std::string& path)
{
//	auto oldScene = SceneManager::GetActiveScene();

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
	//FbxAxisSystem::DirectX.ConvertScene(lScene); // wrong!
//	// FbxSystemUnit::m.ConvertScene(lScene);
//	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_CC93340E_C4A1_49EE_B048_E898F856CFBF_htm
//	// do NOT use FbxSystemUnit::ConvertScene(lScene), which just simply set transform.scale of root nodes.
	
	auto unit = lScene->GetGlobalSettings().GetSystemUnit();
	if ( unit == FbxSystemUnit::mm )
	{
		m_FileScale = 0.001f;
	}
	else if ( unit == FbxSystemUnit::dm)
	{
		m_FileScale = 0.1f;
	}
	else if (unit == FbxSystemUnit::cm)
	{
		m_FileScale = 0.01f;
	}
	else if (unit == FbxSystemUnit::m)
	{
		m_FileScale = 1.0f;
	}
	else
	{
		abort();
	}
	
	FbxGeometryConverter converter(lSdkManager);
	converter.Triangulate(lScene, true);
	BakeTransforms(lScene);

//	int geometryCount = lScene->GetGeometryCount();
//	for (int i = 0; i < geometryCount; ++i) {
//		auto geometry = lScene->GetGeometry(0);
//		if (geometry->GetAttributeType() != FbxNodeAttribute::eMesh) {
//			LogWarning("geometry type not supported");
//			continue;
//		}
//		FbxMesh *lMesh = (FbxMesh *) geometry;
//		auto name = lMesh->GetName();
//		std::cout << i << ": " << name << std::endl;
////		auto mesh = ParseMesh(lMesh);
////		m_model.m_meshes.push_back(mesh);
//	}

	m_model.m_prefab = new Prefab();
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode->GetChildCount() == 0)
	{
		abort();
	}
	else if (lRootNode->GetChildCount() == 1)
	{
		lRootNode = lRootNode->GetChild(0);
	}
	m_model.m_rootGameObject = ParseNode(lRootNode);
	auto p = boost::filesystem::path(path);
	auto fileName = p.stem().string();
	m_model.name = fileName;
	m_model.m_rootGameObject->SetName(fileName);
	m_model.m_prefab->SetRootGameObject(m_model.m_rootGameObject);
	
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();

	int count = 0;
	for (auto&& p : m_model.m_gameObjects)
	{
		auto go = p.second;
		int classID = GameObject::ClassID;
		int fileID = classID * 100000 + count;
//		std::cout << fileID << ": " << p.first << " instanceID: " << go->GetInstanceID() << std::endl;
		//go->GetTransform()->SetRootOrder(count / 2);

		auto name = go->GetName();
		m_fileIDToObject[fileID] = go;
		m_Assets[classID][name] = go;
		for (auto comp : go->GetAllComponents())
		{
			classID = comp->GetClassID();
			fileID = classID * 100000 + count;
			m_fileIDToObject[fileID] = comp;
			m_Assets[classID][name] = go;
		}
		count += 2;
	}

	count = 0;
	for (auto mesh : m_model.m_meshes)
	{
		int classID = Mesh::ClassID;
		int fileID = classID*100000 + count;
		auto name = mesh->GetName();
		m_fileIDToObject[fileID] = mesh;
		m_Assets[classID][name] = mesh;
		count += 2;
	}
}

