#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D al_tex;
uniform bool al_use_tex;
varying vec4 varying_color;
varying vec2 varying_texcoord;

uniform float u_rot;

const vec4 shadow_color = vec4(0.0,0.0,0.0,0.5);
const vec2 shadow_offset = vec2(-0.03,0.03);

mat2 rot_m(float a){
   float sina = sin(a);
   float cosa = cos(a);
   return mat2(
      cosa, sina,
      -sina, cosa
   );
}

void main()
{
   vec4 color;
   if(al_use_tex){
      vec4 col = varying_color * texture2D(al_tex, varying_texcoord);
      vec2 off = rot_m(u_rot) * shadow_offset;
      vec4 shadow = vec4(shadow_color.rgb, texture2D(al_tex, varying_texcoord + off).a * shadow_color.a);
      color = mix(shadow, col, col.a);
   }
   else
      color = varying_color;
   
   gl_FragColor = color;
}