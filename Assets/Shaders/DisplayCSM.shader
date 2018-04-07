// @cull off
// @zwrite off

// #include <Common.inc>


#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

struct V2F
{
	vec2 UV;
};

#ifdef VERTEX
// @vertex
// {
	// #include <CG.inc>

	// (x, y) bottom left corner position (in clip space, (-1, 1))
	// (z, w) width and height, in range [0, 1]
	// eg. (-1, -1, 1, 1) for full sreen quad
	uniform vec4 DrawRectParameters;
	// const vec4 DrawRectParameters = vec4(-1, -1, 1, 1);

	// Screen Aligned Quad
	vec4 DrawRect(in vec2 InPosition)
	{
		return vec4( (InPosition - DrawRectParameters.xy) * DrawRectParameters.zw + DrawRectParameters.xy ,0, 1 );
	}

	layout (location = PositionIndex)   in vec3 InputPosition;
	layout (location = UVIndex)         in vec2 InputUV;

	out V2F v2f;

	void main()
	{
		// gl_Position = DrawRect(InputPosition.xy);
		gl_Position = vec4(InputPosition.x>0?1:-1, InputPosition.y>0?1:-1, 0.0, 1.0);
		// gl_Position = vec4(InputPosition.x>0, InputPosition.y>0, 0.0, 1.0);
		// v2f.UV = InputUV * vec2(1, -1);
		v2f.UV = InputUV;
	}
#endif

#ifdef FRAGMENT
// @fragment
// {
	in V2F v2f;

	out vec4 color;

	uniform sampler2DArray _MainTex;
	// uniform sampler2D _MainTex;

	void main()
	{
		vec4 color0 = vec4(1, 0, 0, 1);
		vec4 color1 = vec4(0, 1, 0, 1);
		vec4 color2 = vec4(0, 0, 1, 1);
		vec4 color3 = vec4(1, 1, 0, 1);
		vec2 uv = v2f.UV;
		int id = int(uv.y>0.5) * 2 + int(uv.x > 0.5);
		if (uv.x < 0.5)
			uv.x *= 2;
		else
			uv.x = (uv.x - 0.5) * 2;
		if (uv.y < 0.5)
			uv.y *= 2;
		else
			uv.y = (uv.y - 0.5) * 2;
		
		float depth = texture(_MainTex, vec3(uv, id)).r;
		// float depth = texture(_MainTex, v2f.UV).r;
		// depth = (depth-0.8)/0.2;
		// if (id == 0)
		// 	color = color0;
		// else if (id == 1)
		// 	color = color1;
		// else if (id == 2)
		// 	color = color2;
		// else if (id == 3)
		// 	color = color3;
		// color = texture(_MainTex, v2f.UV);
		color = vec4(depth, depth, depth, 1.0);
	}
// }
#endif