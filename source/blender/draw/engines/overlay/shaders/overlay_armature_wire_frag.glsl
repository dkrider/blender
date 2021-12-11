
#pragma BLENDER_REQUIRE(common_view_lib.glsl)
#pragma BLENDER_REQUIRE(overlay_armature_alpha_lib.glsl)

void main()
{
  float z_alpha = wire_depth_alpha(gl_FragCoord.z, wireFadeDepth);
  lineOutput = pack_line_data(gl_FragCoord.xy, edgeStart, edgePos);
  fragColor = vec4(finalColor.rgb, finalColor.a * alpha * z_alpha);
}
