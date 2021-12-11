#pragma BLENDER_REQUIRE(overlay_armature_alpha_lib.glsl)

void main()
{
  float z_alpha = wire_depth_alpha(gl_FragCoord.z, wireFadeDepth);
  fragColor = vec4(finalColor.rgb, finalColor.a * alpha * z_alpha);
  lineOutput = vec4(0.0);
}
