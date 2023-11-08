//____________________________________Calcular Capsa Model ______________________________________
void MyGLWidget::info_model(const Model &m) {
  double min_x, min_y, min_z, max_x, max_y, max_z; 
  min_x = m.vertices()[0];
  min_y = m.vertices()[1];
  min_z = m.vertices()[2];
  max_x = m.vertices()[0];
  max_y = m.vertices()[1];
  max_z = m.vertices()[2];
  for(unsigned int i = 3;i<m.vertices().size();i+=3) {
    double x = m.vertices()[i];
    double y = m.vertices()[i+1];
    double z = m.vertices()[i+2];
    if(x<min_x) min_x = x;
    else if (x>max_x) max_x = x;
    if(y<min_y) min_y = y;
    else if (y>max_y) max_y = y;
    if(z<min_z) min_z = z;
    else if (z>max_z) max_z = z;
  }
  punt_Min = glm::vec3(min_x, min_y, min_z);
  punt_Max = glm::vec3(max_x, max_y, max_z);
  punt_Centre = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
  punt_Centre_Base = glm::vec3((min_x+max_x)/2, min_y, (min_z+max_z)/2);
  radi = sqrt(pow(punt_Max.x-punt_Centre.x, 2) + pow(punt_Max.y-punt_Centre.y, 2) + pow(punt_Max.z-punt_Centre.z, 2));
}
//_______________________________________________________________________________________________


//____________________________________Moviment Mouse_____________________________________________
void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  DoingInteractive = true;
  click_x = e->x();
  click_y = e->y();
}
void MyGLWidget::mouseReleaseEvent( QMouseEvent *) {
  DoingInteractive = false;
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();
  if (DoingInteractive) {
    sy += (e->x() - click_x) * M_PI / 180.0;
    sx += (e->y() - click_y) * M_PI / 180.0;
    viewTransform ();
  }
  click_x = e->x();
  click_y = e->y();
  update();
}
//_______________________________________________________________________________________________


//_____________________________Crear buffers sense il·luminacio__________________________________
void MyGLWidget::createBuffers () {
  m.load("/Users/Miquelge/Desktop/IDI/uni/models/Patricio.obj");
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_Pos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_vertices (), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Col);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_matdiff (), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glm::vec3 posterra[4] = {
        glm::vec3(-2.0, 0, 2.0),
        glm::vec3(2.0, 0, 2.0),
        glm::vec3(-2.0, 0, -2.0),
        glm::vec3(2.0, 0, -2.0)
  };
  glm::vec3 colterra[4] = {
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
        glm::vec3(1.,1.,0.),
  };

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_Pos_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Col_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Col_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}
//_______________________________________________________________________________________________


//______________________________Crear buffers amb il·luminacio___________________________________
void MyGLWidget::createBuffers () {
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("/Users/Miquelge/Desktop/IDI/uni/models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  // Buffer de posicions
  glGenBuffers(1, &VBO_PatrPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  // Buffer de normals
  glGenBuffers(1, &VBO_PatrNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_normals(), GL_STATIC_DRAW);
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);
  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_PatrMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matamb(), GL_STATIC_DRAW);
  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);
  // Buffer de component difusa
  glGenBuffers(1, &VBO_PatrMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);
  // Buffer de component especular
  glGenBuffers(1, &VBO_PatrMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matspec(), GL_STATIC_DRAW);
  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);
  // Buffer de component shininness
  glGenBuffers(1, &VBO_PatrMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3, patr.VBO_matshin(), GL_STATIC_DRAW);
  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  // Dades del terra
  // VBO amb la posició dels vèrtexs
  glm::vec3 posterra[12] = {
	glm::vec3(-1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(-1.0, 1.0, -1.0),
	glm::vec3(-1.0, 1.0, -1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, 1.0, -1.0)
  }; 
  // VBO amb la normal de cada vèrtex
  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1, // la normal (0,1,0) per als primers dos triangles
	norm2, norm2, norm2, norm2, norm2, norm2  // la normal (0,0,1) per als dos últims triangles
  };
  // Definim el material del terra
  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.1,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;
  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };
  // Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  // Buffer de posicions
  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  // Buffer de normals
  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);
  // En lloc del color, ara passem tots els paràmetres dels materials
  // Buffer de component ambient
  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);
  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);
  // Buffer de component difusa
  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);
  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);
  // Buffer de component especular
  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);
  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);
  // Buffer de component shininness
  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);
  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

  glBindVertexArray(0);
}
//_______________________________________________________________________________________________


//____________________________________________Pintar_____________________________________________
void MyGLWidget::paintGL () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  info_model(m);
  glBindVertexArray (VAO);
  modelTransform();
  glDrawArrays(GL_TRIANGLES, 0, m.faces ().size () * 3);

  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
//_______________________________________________________________________________________________


//___________________________Resize sense deformació ortogonal___________________________________
void MyGLWidget::resizeGL (int w, int h) {
  if (w/h >= 1) {
  	l = -radiEscena - (2*radiEscena*w/h -2*radiEscena)/2;
  	r = radiEscena + (2*radiEscena*w/h -2*radiEscena)/2;
  	b = -radiEscena;
  	t = radiEscena;
  }
  else if (w/h < 1) {
  	l = -radiEscena;
  	r = radiEscena;
  	b = -radiEscena - (2*radiEscena*h/w -2*radiEscena)/2;
  	t = radiEscena + (2*radiEscena*h/w -2*radiEscena)/2;
  }
  projectTransform();
  glViewport(0, 0, w, h);
}
//_______________________________________________________________________________________________


//___________________________Resize sense deformació perspectiva_________________________________
void MyGLWidget::resizeGL (int w, int h) {
  float rViewport = float (w) / float (h);
  ra = rViewport;
  if (rViewport < 1.0) {
      FOV = 2.0 * atan(tan(fovi/2.0) / rViewport);
    }
  projectTransform();
  glViewport(0, 0, w, h);
}
//_______________________________________________________________________________________________


//________________________________________Key events_____________________________________________
void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_X: {
    	//Coses a fer i actualitzacio matrius si cal
      	break;
    }
    default: {
      event->ignore();
      break;
  	}
  }
  update();
}
//_______________________________________________________________________________________________


//____________________________________Passar uniforms____________________________________________
//carrega shaders
viewLoc = glGetUniformLocation(program->programId(), "view");
//despres
glUniform{1|2|3|4}{f|i|ui} // nombre de paràmetres depenent de 1|2|3|4
	1 – tipus float (f), int (i), unsigned int (ui), bool (f|i|ui)
	2 – tipus vec2 (f), ivec2 (i), uvec2 (ui), bvec2 (f|i|ui)
	3 – tipus vec3 (f), ivec3 (i), uvec3 (ui), bvec3 (f|i|ui)
	4 – tipus vec4 (f), ivec4 (i), uvec4 (ui), bvec4 (f|i|ui)

glUniform{1|2|3|4}{f|i|ui}v (GLint loc, GLsizei count, const Type *value);
	{1|2|3|4} i {f|i|ui} – igual que crida anterior
	count – nombre d’elements de l’array value, 1: un sol valor; >=1 array de valors

glUniformMatrix{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3}fv (GLint loc, GLsizei count, GLboolean transpose, const GLfloat *value);
	{2|3|4|2x3|3x2|2x4|4x2|3x4|4x3} – defineix les dimensions de la matriu
	count – nombre de matrius de l’array value
	transpose – si la matriu s’ha de transposar
//_______________________________________________________________________________________________


//____________________________________Model Transform____________________________________________
void MyGLWidget::modelTransform () {
	glm::mat4 transform (1.0f);
	transform = glm::translate(transform, glm::vec3(x, y, z));
	transform = glm::scale(transform, glm::vec3(1/mida));
	transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
//_______________________________________________________________________________________________


//_____________________________________View Transform____________________________________________
void MyGLWidget::viewTransform () {
	//Funció LookAt
	glm::mat4 view = glm::lookAt (OBS, VRP, UP);
  	
  	//Angles Euler
  	glm::mat4 view (1.0f);
  	view = glm::translate(view, glm::vec3(0, 0,-10));
  	view = glm::rotate(view, sx, glm::vec3(1,0,0));
  	view = glm::rotate(view, sy, glm::vec3(0,1,0));

  	//Passem el uniform
  	glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &view[0][0]);
}
//_______________________________________________________________________________________________


//__________________________________Project Transform____________________________________________
void MyGLWidget::projectTransform () {
	//Perspective
	glm::mat4 Proj = glm::perspective (FOV, ra, znear, zfar);

	//Ortho
	glm::mat4 Proj = glm::ortho (l, r, b, t, znear, zfar);
  	
  	//Passem el uniform
	glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}
//_______________________________________________________________________________________________


//______________________________________Vertex Shader____________________________________________
#version 330 core
in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 colFocus;
uniform vec3 llumAmbient;
uniform vec3 posFocus;
uniform int b;

out vec3 fcolor;
vec3 Lambert (vec3 NormSCO, vec3 L) {
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}
vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO) {
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular
    
    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine); 
}
void main() {
    mat3 nMatrix   = inverse(transpose(mat3(view * TG)));
    vec3 NormSCO   = normalize(nMatrix * normal);
    vec4 vertexSCO = view * TG * vec4(vertex, 1.0);
    vec4 focusSCO;
    if (b == 1) focusSCO = view * vec4(posFocus, 1.0);
    else focusSCO = vec4(0, 0, 0, 1.0);
    vec4 L = focusSCO - vertexSCO;
    vec3 Lxyz = normalize(L.xyz);

    //fcolor       = Lambert (NormSCO, Lxyz);
    fcolor         = Phong (NormSCO, Lxyz, vertexSCO);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
//_______________________________________________________________________________________________


//____________________________________Fragment Shader____________________________________________
#version 330 core

in vec3 fcolor;
out vec4 FragColor;

void main() {	
	FragColor = vec4(fcolor,1);	
}
//_______________________________________________________________________________________________


//________________________________Pas info de qt a widget________________________________________
Declarem slot a fitxer .h

public slots:
    void actualitzar(int x);

Linquem tot amb qt i la part de signals
//_______________________________________________________________________________________________

  
//________________________________Pas info de widget a qt________________________________________
Declarem signals a fitxer .h

signals:
	void actualitzar(int x);

Linquem tot amb qt i la part de signals

Per a usar-les fem: emit actualitzar(sy);
//_______________________________________________________________________________________________







