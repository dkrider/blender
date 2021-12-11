#pragma BLENDER_REQUIRE(overlay_armature_alpha_lib.glsl)

void main()
{
  float n = normalize(normalView).z;
  float z_alpha = wire_depth_alpha(gl_FragCoord.z, wireFadeDepth);
  if (isDistance) {
    n = 1.0 - clamp(-n, 0.0, 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 0.33 * alpha * z_alpha) * n;
  }
  else {
    /* Smooth lighting factor. */
    const float s = 0.2; /* [0.0-0.5] range */
    float fac = clamp((n * (1.0 - s)) + s, 0.0, 1.0);
    fragColor.rgb = mix(finalStateColor, finalBoneColor, fac * fac);
    fragColor.a = alpha * z_alpha;
  }
  lineOutput = vec4(0.0);
}
