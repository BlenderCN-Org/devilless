#version 110

uniform mat4 modelView;
uniform mat4 viewProjection;
uniform vec4 materials[4];
uniform mat4 bones[37];

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
//attribute float vertexMaterialID;
attribute vec3 vertexBoneIDs;
attribute vec3 vertexBoneWeights;

varying vec3 fragmentColor;

void main() {
	vec4 pos = vec4(vertexPosition, 1.0);
	
	mat4 finalTransform = bones[int(vertexBoneIDs.x)] * vertexBoneWeights.x;
	finalTransform += bones[int(vertexBoneIDs.y)] * vertexBoneWeights.y;
	finalTransform += bones[int(vertexBoneIDs.z)] * vertexBoneWeights.z;
	
	fragmentColor = vertexNormal;
	gl_Position = pos * finalTransform * modelView * viewProjection;
}