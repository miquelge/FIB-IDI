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

void main()
{
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
