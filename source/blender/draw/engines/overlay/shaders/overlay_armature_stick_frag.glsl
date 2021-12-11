#pragma BLENDER_REQUIRE(overlay_armature_alpha_lib.glsl)

void main()
{
  float fac = smoothstep(1.0, 0.2, colorFac);
  float z_alpha = wire_depth_alpha(gl_FragCoord.z, wireFadeDepth);
  fragColor.rgb = mix(finalInnerColor.rgb, finalWireColor.rgb, fac);
  fragColor.a = alpha * z_alpha;
  lineOutput = vec4(0.0);
}
