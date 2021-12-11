#pragma BLENDER_REQUIRE(overlay_armature_alpha_lib.glsl)

void main()
{
  /* Manual back-face culling. Not ideal for performance
   * but needed for view clarity in X-ray mode and support
   * for inverted bone matrices. */
  if ((inverted == 1) == gl_FrontFacing) {
    discard;
  }
  float z_alpha = wire_depth_alpha(gl_FragCoord.z, wireFadeDepth);
  fragColor = vec4(finalColor.rgb, alpha * z_alpha);
  lineOutput = vec4(0.0);
}
