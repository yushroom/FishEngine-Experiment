def GenCPPProperty(type, name):
    assert(name.startswith('m_'))
    # if name.startswith('m_'):
    #     name = name[2:]
    pretty_name = name[2:]

    # print '+++++++++++++'
    # print('')
    if type in ('int', 'float', 'bool', 'uint32_t', 'bool') or type.endswith('*'):
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



klass = 'Prefab'
s = '''
        PrefabModification          m_Modification;
        Prefab*                    m_ParentPrefab = nullptr;
        GameObject*                m_RootGameObject = nullptr;
        bool                        m_IsPrefabParent = true;
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