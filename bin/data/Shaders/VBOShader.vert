varying vec3 tnorm, eyeVec;

void main()
{	
	tnorm = normalize(gl_NormalMatrix * gl_Normal);

	//--- Eye's position
	eyeVec = vec3(gl_ModelViewMatrix * gl_Vertex);
	eyeVec = normalize(eyeVec);

	gl_Position = ftransform();

}