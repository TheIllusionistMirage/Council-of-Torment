uniform vec2 center;
uniform float radius;
     
void main()
{
    float difference = distance(vec2(gl_TexCoord[0].xy), center) / radius;
    difference = max(0.0, difference);

	gl_FragColor = vec4((1.0 - difference) * gl_Color.xyz, 1.0);
}