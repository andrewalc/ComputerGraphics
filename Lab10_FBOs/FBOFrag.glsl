#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;

// We always define a fragment color that we output.
out vec4 fragColor;

// Maintain our uniforms.
uniform sampler2D FBOTex;              // our primary texture
const float offset = 1.0 / 300.0;  

void main() {
  // Set our output fragment color to whatever we pull from our input texture (Note, change 'tex' to whatever the sampler is named)
  // The variable 'color' will now contain whatever would have been on screen if we were rendering directly.
  vec3 color = texture(FBOTex, texCoords).rgb;
  // We can now modify things a bit to provide some nice post-processing effects.
  // TODO - Implement some type of post-processing effect here.
  vec2 offsets[9] = vec2[](
      vec2(-offset,  offset), // top-left
      vec2( 0.0f,    offset), // top-center
      vec2( offset,  offset), // top-right
      vec2(-offset,  0.0f),   // center-left
      vec2( 0.0f,    0.0f),   // center-center
      vec2( offset,  0.0f),   // center-right
      vec2(-offset, -offset), // bottom-left
      vec2( 0.0f,   -offset), // bottom-center
      vec2( offset, -offset)  // bottom-right    
  );

  float kernel[9] = float[](
      -1, -1, -1,
      -1,  9, -1,
      -1, -1, -1
  );
  
  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
      sampleTex[i] = vec3(texture(FBOTex, texCoords.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
      col += sampleTex[i] * kernel[i];
  
  fragColor = vec4(col, 1.0);

}