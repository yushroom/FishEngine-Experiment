from mako.template import Template
schema_struct_FishEngine = '''
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
@AnimatorControllerLayer
	m_Name: Base Layer
	m_StateMachine: {fileID: 1107723725710441410}
	# m_Mask: {fileID: 0}
	# m_Motions: []
	# m_Behaviours: []
	# m_BlendingMode: 0
	# m_SyncedLayerIndex: -1
	# m_DefaultWeight: 0
	# m_IKPass: 0
	# m_SyncedLayerAffectsTiming: 0
	m_Controller: {fileID: 9100000}
@ChildAnimatorStateMachine
	m_State
@HumanLimit:
	min: {x: 0, y: 0, z: 0}
	max: {x: 0, y: 0, z: 0}
	value: {x: 0, y: 0, z: 0}
	length: 0
@HumanBone:
	boneName: RightHandPinky3
	humanName: Right Little Distal
	limit:
@SkeletonBone:
	name: Run(Clone)
	position: {x: 0, y: 0, z: 0}
	rotation: {x: 0, y: 0, z: 0, w: 1}
	scale: {x: 1, y: 1, z: 1}
@HumanDescription:
	human
	skeleton
'''

schema_struct_FishEditor = '''
@ModelImporterClipAnimation
	name: PlayerIdle
	takeName: Take 001
	firstFrame: 1
	lastFrame: 400
@ModelImporterAnimation
	clipAnimations
	isReadable: 1
@ModelImporterMesh
	# lODScreenPercentages: []
	globalScale: 1
	# meshCompression: 0
	addColliders: 0
	importBlendShapes: 1
	swapUVChannels: 0
	# generateSecondaryUV: 0
	useFileUnits: 1
	# optimizeMeshForGPU: 1
	# keepQuads: 0
	# weldVertices: 1
	# secondaryUVAngleDistortion: 8
	# secondaryUVAreaDistortion: 15.000001
	# secondaryUVHardAngle: 88
	# secondaryUVPackMargin: 4
	useFileScale: 1
'''

schema_Object_FishEngine = '''
@Object
	m_ObjectHideFlags
	m_PrefabParentObject
	m_PrefabInternal
	m_Name

@GameObject: Object
	m_PrefabParentObject: Prefab*
	m_PrefabInternal: Prefab*
	m_Component : std::vector<Component*>
	m_Layer: int
	m_Name : std::string
	m_TagString: std::string
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
@SkinnedMeshRenderer: Renderer
	m_Mesh
	m_Avatar
	m_RootBone
	m_Bones

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
@Avatar: Object
@Motion: Object
@AnimationClip: Motion
	m_avatar: Avatar*
@Animation: Behaviour
	m_clip
	m_wrapMode
@Animator: Behaviour
	m_Avatar
	m_Controller
	m_ApplyRootMotion
	m_LinearVelocityBlending
	m_WarningMessage
	m_HasTransformHierarchy
	m_AllowConstantClipSamplingOptimization
@RuntimeAnimatorController : Object
@AnimatorController: RuntimeAnimatorController
	m_AnimatorLayers
@AnimatorState: Object
	m_Motion
@AnimatorStateMachine: Object
	m_ChildStates:
	# - serializedVersion: 1
	# m_State: {fileID: 1102003656108312482}
	# m_Position: {x: 360, y: 72, z: 0}
	# m_ChildStateMachines: []
	# m_AnyStateTransitions: []
	# m_EntryTransitions: []
	# m_StateMachineTransitions: {}
	# m_StateMachineBehaviours: []
	# m_AnyStatePosition: {x: 50, y: 20, z: 0}
	# m_EntryPosition: {x: 50, y: 120, z: 0}
	# m_ExitPosition: {x: 800, y: 120, z: 0}
	# m_ParentStateMachinePosition: {x: 800, y: 20, z: 0}
	m_DefaultState: {fileID: 1102003656108312482}
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

schema_Object_FishEditor = '''
@AssetImporter: Object
@ModelImporter: AssetImporter
	fileIDToRecycleName
	# materials
	animations
	meshes
	importAnimation: 1
	humanDescription
	animationType: 3
@DefaultImporter: AssetImporter
  # externalObjects: {}
  # userData: 
  # assetBundleName: 
  # assetBundleVariant: 
@NativeFormatImporter: AssetImporter
  # externalObjects: {}
  mainObjectFileID: 9100000
  # userData: 
  # assetBundleName: 
  # assetBundleVariant: 
'''


template_serialize_header = '''
#pragma once
#include <FishEngine/Serialization/Archive.hpp>

namespace FishEngine
{
% for c in ClassInfo:
	struct ${c['className']};
	FishEngine::InputArchive&  operator>>(FishEngine::InputArchive& archive, ${c['className']}& t);
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const ${c['className']}& t);

% endfor
}
'''

template_serialize_source = '''
namespace FishEngine
{
% for c in ClassInfo:
	// ${c['className']}
	FishEngine::InputArchive& operator>>(FishEngine::InputArchive& archive, ${c['className']}& t)
	{
	% for member in c['members']:
		archive.AddNVP("${member}", t.${member});
	% endfor
		return archive;
	}
	FishEngine::OutputArchive& operator<<(FishEngine::OutputArchive& archive, const ${c['className']}& t)
	{
	% for member in c['members']:
		archive.AddNVP("${member}", t.${member});
	% endfor
		return archive;
	}

% endfor
}
'''

template2 = '''#include <FishEngine/Serialization/Serialize.hpp>
#include <FishEngine/Serialization/Archive.hpp>
#include <FishEngine/FishEngine2.hpp>

using namespace FishEngine;
using namespace FishEditor;
using namespace FishEditor::Animation;

//namespace FishEngine
//{
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
//}
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

# Func(schema_struct_FishEngine, template_serialize_header)
Func(schema_struct_FishEngine, template_serialize_source)
# Func(object_schema, template2)