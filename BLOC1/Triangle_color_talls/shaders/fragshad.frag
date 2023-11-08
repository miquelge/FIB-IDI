#version 330 core

out vec4 FragColor;

void main () {
	
	
	if (int(gl_FragCoord.y)%8<4) discard; //FragColor = vec4 (0., 0., 0., 1.);
	else {
		if ( gl_FragCoord.x < 693 && gl_FragCoord.y < 693 ) FragColor = vec4 (1., 1., 0., 1.);
		else if ( gl_FragCoord.x < 693 && gl_FragCoord.y >= 693 ) FragColor = vec4 (1., 0., 0., 1.);
		else if ( gl_FragCoord.x >= 693 && gl_FragCoord.y < 693 ) FragColor = vec4 (0., 1., 0., 1.);
		else FragColor = vec4 (0., 0., 1., 1.);
	}
}

