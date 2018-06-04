## 欧拉角与四元数

### 1.欧拉角转四元数

```python
from __future__ import division
from sympy import *
x, y, z = symbols('x y z')
sx, cx = sin(x/2), cos(x/2)
sy, cy = sin(y/2), cos(y/2)
sz, cz = sin(z/2), cos(z/2)
# sx, cx, sy, cy, sz, cz = symbols('sx cx sy cy sz cz')
qx = Matrix([sx, 0, 0, cx])
qy = Matrix([0, sy, 0, cy])
qz = Matrix([0, 0, sz, cz])

def quat_mul(p, q):
    x, y, z, w = p.col(0)
    x2, y2, z2, w2 = q.col(0)
    a = w*x2 + x*w2 + y*z2 - z*y2
    b = w*y2 + y*w2 + z*x2 - x*z2
    c = w*z2 + z*w2 + x*y2 - y*x2
    d = w*w2 - x*x2 - y*y2 - z*z2
    return Matrix([a, b, c, d])
q_yxz = quat_mul(qy, quat_mul(qx, qz))
print q_yxz
```

结果为

```python
Matrix([
[ sin(x/2)*cos(y/2)*cos(z/2) + sin(y/2)*sin(z/2)*cos(x/2)],
[-sin(x/2)*sin(z/2)*cos(y/2) + sin(y/2)*cos(x/2)*cos(z/2)],
[-sin(x/2)*sin(y/2)*cos(z/2) + sin(z/2)*cos(x/2)*cos(y/2)],
[ sin(x/2)*sin(y/2)*sin(z/2) + cos(x/2)*cos(y/2)*cos(z/2)]])
即
Matrix([
[cx*sy*sz + cy*cz*sx],
[cx*cz*sy - cy*sx*sz],
[cx*cy*sz - cz*sx*sy],
[cx*cy*cz + sx*sy*sz]])
```



### 2.四元数转欧拉角

```python
from __future__ import division
from sympy import *
x, y, z = symbols('x y z')
Rx = Matrix([[1, 0, 0], [0, cos(x), -sin(x)], [0, sin(x), cos(x)]])
Ry = Matrix([[cos(y), 0, sin(y)], [0, 1, 0], [-sin(y), 0, cos(y)]])
Rz = Matrix([[cos(z), -sin(z), 0], [sin(z), cos(z), 0], [0, 0, 1]])

R = Rz*Ry*Rx
```

结果为：

```python
Matrix([
[sin(x)*sin(y)*sin(z) + cos(y)*cos(z), sin(x)*sin(y)*cos(z) - sin(z)*cos(y), sin(y)*cos(x)],
[                       sin(z)*cos(x),                        cos(x)*cos(z),       -sin(x)],
[sin(x)*sin(z)*cos(y) - sin(y)*cos(z), sin(x)*cos(y)*cos(z) + sin(y)*sin(z), cos(x)*cos(y)]])
```

所以

```
R[1, 2] = -sin(x)
R[0, 2] / R[2, 2] = sin(y)/cos(y)
R[1, 0] / R[1, 1] = sin(z)/cos(z)
```

将四元数转为旋转矩阵，再按上面的式子计算就可以得到欧拉角

