uniform float leftPos;
uniform float width;
     
void main()
{
	float alpha = (gl_FragCoord.x - leftPos) / width;
	gl_FragColor = vec4(gl_Color.xyz, alpha);
}