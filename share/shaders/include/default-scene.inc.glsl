scene = opU( scene, vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 ) );
scene = opU( scene, vec2( sdBox(       pos-vec3( 1.0,0.25, 0.0), vec3(0.25) ), 3.0 ) );
scene = opU( scene, vec2( udRoundBox(  pos-vec3( 1.0,0.25, 1.0), vec3(0.15), 0.1 ), 41.0 ) );
scene = opU( scene, vec2( sdTorus(     pos-vec3( 0.0,0.25, 1.0), vec2(0.20,0.05) ), 25.0 ) );
scene = opU( scene, vec2( sdCapsule(   pos,vec3(-1.3,0.10,-0.1), vec3(-0.8,0.50,0.2), 0.1  ), 31.9 ) );
scene = opU( scene, vec2( sdTriPrism(  pos-vec3(-1.0,0.25,-1.0), vec2(0.25,0.05) ),43.5 ) );
scene = opU( scene, vec2( sdCylinder(  pos-vec3( 1.0,0.30,-1.0), vec2(0.1,0.2) ), 8.0 ) );
scene = opU( scene, vec2( sdCone(      pos-vec3( 0.0,0.50,-1.0), vec3(0.8,0.6,0.3) ), 55.0 ) );
scene = opU( scene, vec2( sdTorus82(   pos-vec3( 0.0,0.25, 2.0), vec2(0.20,0.05) ),50.0 ) );
scene = opU( scene, vec2( sdTorus88(   pos-vec3(-1.0,0.25, 2.0), vec2(0.20,0.05) ),43.0 ) );
scene = opU( scene, vec2( sdCylinder6( pos-vec3( 1.0,0.30, 2.0), vec2(0.1,0.2) ), 12.0 ) );
scene = opU( scene, vec2( sdHexPrism(  pos-vec3(-1.0,0.20, 1.0), vec2(0.25,0.05) ),17.0 ) );
scene = opU( scene, vec2( sdPryamid4(  pos-vec3(-1.0,0.15,-2.0), vec3(0.8,0.6,0.25) ),37.0 ) );
scene = opU( scene, vec2( opS( udRoundBox(  pos-vec3(-2.0,0.2, 1.0), vec3(0.15),0.05),
                           sdSphere(    pos-vec3(-2.0,0.2, 1.0), 0.25)), 13.0 ) );
scene = opU( scene, vec2( opS( sdTorus82(  pos-vec3(-2.0,0.2, 0.0), vec2(0.20,0.1)),
                           sdCylinder(  opRep( vec3(atan(pos.x+2.0,pos.z)/6.2831, pos.y, 0.02+0.5*length(pos-vec3(-2.0,0.2, 0.0))), vec3(0.05,1.0,0.05)), vec2(0.02,0.6))), 51.0 ) );
scene = opU( scene, vec2( 0.5*sdSphere(    pos-vec3(-2.0,0.25,-1.0), 0.2 ) + 0.03*sin(50.0*pos.x)*sin(50.0*pos.y)*sin(50.0*pos.z), 65.0 ) );
scene = opU( scene, vec2( 0.5*sdTorus( opTwist(pos-vec3(-2.0,0.25, 2.0)),vec2(0.20,0.05)), 46.7 ) );
scene = opU( scene, vec2( sdConeSection( pos-vec3( 0.0,0.35,-2.0), 0.15, 0.2, 0.1 ), 13.67 ) );
scene = opU( scene, vec2( sdEllipsoid( pos-vec3( 1.0,0.35,-2.0), vec3(0.15, 0.2, 0.05) ), 43.17 ) );

float m = mix(0.0, 0.2, smoothstep(0.9, 1.2, pos.y));
vec2 r2 = vec2(pillar(pos-vec3(-1.0, 0.0, 0.0)), 3.0);
r2 = opU( r2, vec2(pillar(pos-vec3(1.0, 0.0, 0.0)), 3.0));
r2 = opU( r2, vec2(pillar(pos-vec3(-1.0, 0.0, 2.0)), 3.0));
r2 = opU( r2, vec2(pillar(pos-vec3(1.0, 0.0, 2.0)), 3.0));

r2 = opU( r2, vec2(pillar(pos-vec3(-1.0, 1.1, 2.0)), 3.0));

r2 = opU( r2, vec2( sdBox( pos-vec3( 0.0,1.1,1.0), vec3(1.1, 0.05, 2.1) ), 3.0 ) );

r2 = opU( r2, vec2( sdBox( pos-vec3( -1.2,1.0,1.0), vec3(0.275, 0.1, 2.1) ), 3.0 ) );
r2 = opU( r2, vec2( sdBox( pos-vec3(  1.3,0.55,1.0), vec3(0.275, 0.55, 2.1) ), 3.0 ) );

r2 = vec2( opS(r2.x, sdBox( pos-vec3( 0.0,0.0,3.0), vec3(0.9, 2.0, 0.9) )), 3.0 );

scene = opU(scene, r2);
