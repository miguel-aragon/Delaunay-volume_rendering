uniform vec3 myColor;
varying vec3 tnorm, eyeVec;

void main (void)
{
	
	float lambertTerm = 1.0-abs(dot(tnorm,eyeVec));
	
	gl_FragColor = vec4(myColor, lambertTerm);

}
