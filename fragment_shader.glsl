#version 330 core
uniform vec2 u_resolution;
uniform vec2 u_location;
uniform float u_time;
uniform vec3 u_color;
uniform float x;
uniform float y;
out vec4 FragColor;
in vec3 vertices;

float map(float value, float istart, float istop, float ostart, float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
  }
void main() {

  float width = map(0.16666666666, 0., 2., 0, u_resolution.x);
  float height = map(.25, 0., 2., 0, u_resolution.y);


  float pct = distance(vec2(u_location.x + (width/2), u_location.y + (height/2)), gl_FragCoord.xy);
  if (pct >= abs(sin(u_time * .25) * 60))
    { discard; } 
  pct = map(pct, 0., abs(sin(u_time * .25) * 75), 1., 0.);
  FragColor = vec4(vec3(pct), 1.0);
}