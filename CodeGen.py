from mako.template import Template
schema = '''
@Vector2
	x
	y
@Vector3
	x
	y
	z
@Vector4
	x
	y
	z
	w
@Quaternion
	x
	y
	z
	w
@Color
	r
	g
	b
	a

@Modification
	target: nullptr;
	propertyPath
	value
	objectReference: nullptr;
	
@PrefabModification
	m_TransformParent: nullptr;
	m_Modifications
	m_RemovedComponents
'''

object_schema = '''
@Object
	m_ObjectHideFlags: ObjectHideFlags

@GameObject: Object
	m_PrefabParentObject: Prefab*
	m_PrefabInternal: Prefab*
	m_Component : std::vector<Component*>
	# m_Layer
	m_Name : std::string
	# m_Icon
	# m_NavMeshLayer
	# m_StaticEditorFlags
	m_IsActive : bool

@Prefab: Object
	m_Modification
	m_ParentPrefab: Prefab*
	m_RootGameObject
	m_IsPrefabParent: bool

@Component: Object
	m_PrefabParentObject: Prefab*
	m_PrefabInternal: Prefab*
	m_GameObject: GameObject*

@Transform: Component
	m_LocalRotation: Quaternion
	m_LocalPosition: Vector3
	m_LocalScale: Vector3
	m_Children: std::vector<Transform*>
	m_Father: Transform*
	m_RootOrder: int
@Camera: Component
	m_NearClipPlane
	m_FarClipPlane
	m_FieldOfView
	m_Orthographic
	m_OrthographicSize

@Behaviour: Component
	m_Enabled

@Light: Behaviour

@RectTransform: Component
	m_AnchorMin: {0.5f, 0.5f};
	m_AnchorMax: {0.5f, 0.5f};
	m_AnchoredPosition: {0, 0};
	m_SizeDelta: {100, 100};
	m_Pivot: {0.5f, 0.5f};

@MeshFilter: Component
	m_Mesh: Mesh*

@Renderer: Component
	m_Enabled: 1
	m_CastShadows: 1
	m_ReceiveShadows: 1
	m_Materials:

@MeshRenderer: Renderer

@Collider: Component
	# m_Material: {fileID: 0}
	m_IsTrigger: bool
	m_Enabled: bool

@BoxCollider: Collider
	m_Size: {x: 1, y: 1, z: 1}
	m_Center: {x: 0, y: 0, z: 0}

@SphereCollider: Collider
	m_Radius: float
	m_Center: Vector3

@Rigidbody: Component
	m_Mass: 1
	m_Drag: 0
	m_AngularDrag: 0.05
	m_UseGravity: 1
	m_IsKinematic: 0
	# m_Interpolate: 0
	# m_Constraints: 0
	# m_CollisionDetection: 0

@RenderSettings: Object
  m_ObjectHideFlags: 0
  # serializedVersion: 8
  m_Fog: 0
  m_FogColor: {r: 0.5, g: 0.5, b: 0.5, a: 1}
  m_FogMode: 3
  m_FogDensity: 0.01
  m_LinearFogStart: 0
  m_LinearFogEnd: 300
  m_AmbientSkyColor: {r: 0.212, g: 0.227, b: 0.259, a: 1}
  m_AmbientEquatorColor: {r: 0.114, g: 0.125, b: 0.133, a: 1}
  m_AmbientGroundColor: {r: 0.047, g: 0.043, b: 0.035, a: 1}
  m_AmbientIntensity: 1
  m_AmbientMode: 0
  m_SubtractiveShadowColor: {r: 0.42, g: 0.478, b: 0.627, a: 1}
  m_SkyboxMaterial: {fileID: 10304, guid: 0000000000000000f000000000000000, type: 0}
  m_HaloStrength: 0.5
  m_FlareStrength: 1
  m_FlareFadeSpeed: 3
  # m_HaloTexture: {fileID: 0}
  # m_SpotCookie: {fileID: 10001, guid: 0000000000000000e000000000000000, type: 0}
  # m_DefaultReflectionMode: 0
  # m_DefaultReflectionResolution: 128
  # m_ReflectionBounces: 1
  # m_ReflectionIntensity: 1
  # m_CustomReflection: {fileID: 0}
  m_Sun: {fileID: 0}
  m_IndirectSpecularColor: {r: 0.44657826, g: 0.49641263, b: 0.57481676, a: 1}
'''

'''
Camera:
  m_ObjectHideFlags: 0
  m_PrefabParentObject: {fileID: 0}
  m_PrefabInternal: {fileID: 0}
  m_GameObject: {fileID: 822216902}
  m_Enabled: 1
  serializedVersion: 2
  m_ClearFlags: 1
  m_BackGroundColor: {r: 0.19215687, g: 0.3019608, b: 0.4745098, a: 0}
  m_NormalizedViewPortRect:
	serializedVersion: 2
	x: 0
	y: 0
	width: 1
	height: 1
  near clip plane: 0.3
  far clip plane: 1000
  field of view: 60
  orthographic: 0
  orthographic size: 5
  m_Depth: -1
  m_CullingMask:
	serializedVersion: 2
	m_Bits: 4294967295
  m_RenderingPath: -1
  m_TargetTexture: {fileID: 0}
  m_TargetDisplay: 0
  m_TargetEye: 3
  m_HDR: 1
  m_AllowMSAA: 1
  m_AllowDynamicResolution: 0
  m_ForceIntoRT: 0
  m_OcclusionCulling: 1
  m_StereoConvergence: 10
  m_StereoSeparation: 0.022
'''

template1 = '''#pragma once

#include <FishEngine/Math/Vector2.hpp>
#include <FishEngine/Math/Vector3.hpp>
#include <FishEngine/Math/Vector4.hpp>
#include <FishEngine/Math/Quaternion.hpp>
#include <FishEngine/Prefab.hpp>

#include <FishEngine/Serialization/Archive.hpp>

namespace FishEngine
{
% for c in ClassInfo:
	// ${c['className']}
	inline InputArchive& operator>>(InputArchive& archive, ${c['className']}& t)
	{
	% for member in c['members']:
		archive.AddNVP("${member}", t.${member});
	% endfor
		return archive;
	}
	inline OutputArchive& operator<<(OutputArchive& archive, const ${c['className']}& t)
	{
	% for member in c['members']:
		archive.AddNVP("${member}", t.${member});
	% endfor
		return archive;
	}

% endfor
}

'''

template2 = '''
namespace FishEngine
{
% for c in ClassInfo:
	// ${c['className']}
	void ${c['className']}::Deserialize(InputArchive& archive)
	{
	% if 'parent' in c:
		${c['parent']}::Deserialize(archive);
	% endif
	% for member in c['members']:
		archive.AddNVP("${member}", this->${member});
	% endfor
	}

	void ${c['className']}::Serialize(OutputArchive& archive) const
	{
	% if 'parent' in c:
		${c['parent']}::Serialize(archive);
	% endif
	% for member in c['members']:
		archive.AddNVP("${member}", this->${member});
	% endfor
	}


% endfor
}
'''

def Func(schema, template):
	template1 = Template(template)

	def GenSerialize(className, parentClassName, members):
		classInfo = {'className': className, 'parent': parentClassName, 'members':members}
		return template1.render(c = classInfo)
	classInfo = []

	for s in schema.split("@"):
		s = s.strip()
		if len(s) == 0:
			continue
		lines = s.split('\n')
		l0 = lines[0].strip()
		klass = {}
		if ':' in l0:
			className, parentClassName = l0.split(':')
			className = className.strip()
			parentClassName = parentClassName.strip()
			klass['parent'] = parentClassName
		else:
			className = l0
		klass['className'] = className
		members = list(map(lambda x: x.split(':')[0].strip(), lines[1:]))
		members = [m for m in members if not m.startswith('#')]
		klass['members'] = members

		classInfo.append(klass)
	# print(classInfo)
	print(template1.render(ClassInfo=classInfo))

Func(schema, template1)
# Func(object_schema, template2)