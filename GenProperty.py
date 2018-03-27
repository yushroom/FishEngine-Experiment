def GenCPPProperty(type, name):
    assert(name.startswith('m_'))
    # if name.startswith('m_'):
    #     name = name[2:]
    pretty_name = name[2:]

    # print '+++++++++++++'
    # print('')
    if type in ('int', 'float', 'bool', 'uint32_t') or type.endswith('*'):
    # print '==== v1'
        print('{0} Get{1}() const {{ return m_{1}; }}'.format(type, pretty_name))
        print('void Set{1}({0} value) {{ m_{1} = value; }}'.format(type, pretty_name))
    else:
        # print '=== v2'
        print('const {0}& Get{1}() const {{ return m_{1}; }}'.format(type, pretty_name))
        print('void Set{1}(const {0}& value) {{ m_{1} = value; }}'.format(type, pretty_name))
    print('')

def GenPythonDef(klass, name):
    pretty_name = name[2:]
    # print('.def("Get{1}", &{0}::Get{1})'.format(klass, pretty_name))
    # print('.def("Set{1}", &{0}::Set{1})'.format(klass, pretty_name))
    name2 = pretty_name[0].lower() + pretty_name[1:]
    print('.def_property("{mass}", &{Rigidbody}::Get{Mass}, &{Rigidbody}::Set{Mass})'.format(mass=name2, Mass=pretty_name, Rigidbody=klass))

template3 = '''
        @property
        def {center}(self)->{Vector3}:
            return self.cpp.Get{Center}()
        @{center}.setter
        def {center}(self, value:{Vector3}):
            self.cpp.Set{Center}(value)'''

def GenPythonProperty(type, name):
    pretty_name = name[2:]
    name2 = pretty_name[0].lower() + pretty_name[1:]
    print(template3.format(center=name2, Vector3=type, Center=pretty_name))


schema = '''
Rigidbody:
  m_Mass: 1
  m_Drag: 0
  m_AngularDrag: 0.05
  m_UseGravity: 1
  m_IsKinematic: 0
  m_Interpolate: 0
  m_Constraints: 0
  m_CollisionDetection: 0

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

klass = 'Transform'
s = '''
    Quaternion m_LocalRotation {0, 0, 0, 1};
    Vector3 m_LocalPosition {0, 0, 0};
    Vector3 m_LocalScale {1, 1, 1};
    Quaternion m_Rotation {0, 0, 0, 1};
    Vector3 m_Position {0, 0, 0};
    Vector3 m_Scale {1, 1, 1};
'''
s = s.strip().split('\n')
s = [x.strip() for x in s]
pairs = []
for line in s:
    line = line.strip()
    if line.endswith(';'):
      line = line[:-1]
    if line.startswith('//'):
      continue
    t = line.split()
    if len(t) >= 2:
      pairs.append((t[0], t[1]))
print(pairs)

for type, name in pairs:
    GenCPPProperty(type, name)

for _, name in pairs:
    GenPythonDef(klass, name)

# for type, name in pairs:
#     GenPythonProperty(type, name)