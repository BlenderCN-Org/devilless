#version 110

uniform mat4 modelView;
uniform mat4 viewProjection;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

varying vec3 fragmentColor;

void main() {
	fragmentColor = vertexNormal;
	gl_Position = vec4(vertexPosition, 1.0) * modelView * viewProjection;
}