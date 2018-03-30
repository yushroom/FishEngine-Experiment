objects = '''
Prefab
GameObject
Transform
Camera
Light
RectTransform
MeshFilter
MeshRenderer
BoxCollider
SphereCollider
Rigidbody
'''

from mako.template import Template

template = '''
inline Object* CreateEmptyObjectByClassID(int classID)
{
	if (classID == ${Objects[0]}::ClassID)
		return CreateEmptyObject<${Objects[0]}>();
% for T in Objects[1:]:
	else if (classID == ${T}::ClassID)
		return CreateEmptyObject<${T}>();
% endfor
}
'''
template = Template(template)

objects = objects.split()
print(template.render(Objects=objects))
