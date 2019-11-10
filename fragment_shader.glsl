#version 330 core
uniform vec2 u_resolution;
uniform vec2 u_location;
uniform float u_time;
uniform vec3 u_color;
uniform vec2 u_mouse;
uniform float u_size;
uniform float x;
uniform float y;
out vec4 FragColor;
in vec3 vertices;

float map(float value, float istart, float istop, float ostart, float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
  }

mat2 scale(vec2 _scale){
  return mat2(_scale.x,0.0,
              0.0,_scale.y);
}

float box(vec2 _st, vec2 _size, float _smoothEdges){

    _size = vec2(0.5)-_size*0.5;
    vec2 aa = vec2(_smoothEdges*0.5);
    vec2 uv = smoothstep(_size,_size+aa,_st);
    uv *= smoothstep(_size,_size+aa,vec2(1.0)-_st);
    return uv.x*uv.y;
}

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(24.18, 12.238)))*
        43758.5453123);
}

void main() {

  vec2 st = gl_FragCoord.xy/u_resolution;
  st.x *= 12;
  st.y *= 8;
  st = fract(st);
  float pct = 0.0;
  pct = distance(st,vec2(0.5));
  vec3 color = vec3(pct);
  
  
  if (pct >= 0.5){ 
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st = st * 100;
    st = fract(st);
    st += abs(sin(u_time * random(vec2(45, 34)))) * .5;
    vec3 color = vec3(1.0, 1.0, 1.0) - vec3(box(st, vec2(0.4), 0.01));
   
    FragColor = vec4(vec3(0.5), 1.0);
  } else{
    FragColor = vec4(vec3(pct), 1.0);
  }
}