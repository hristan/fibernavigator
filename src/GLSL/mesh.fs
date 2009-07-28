#include lighting.fs

uniform int sector;
uniform float cutX, cutY, cutZ;
uniform int dimX, dimY, dimZ;
uniform float voxX, voxY, voxZ;
uniform float alpha_;
varying vec4 myColor;

uniform bool showFS;
uniform bool useTex;
uniform bool blendTex;
uniform bool cutAtSurface;
uniform bool useCMAP;

uniform sampler2D cutTex;
uniform int countTextures;
uniform bool useLic;
uniform bool lightOn;

#include functions.fs

void cutFrontSector() 
{
    float cx = cutX * voxX;
    float cy = cutY * voxY;
    float cz = cutZ * voxZ;

	if (sector == 1 && vertex.x > cx && vertex.y > cy && vertex.z > cz)
		discard;
	if (sector == 2 && vertex.x > cx && vertex.y > cy && vertex.z < cz)
		discard;
	if (sector == 3 && vertex.x > cx && vertex.y < cy && vertex.z < cz)
		discard;
	if (sector == 4 && vertex.x > cx && vertex.y < cy && vertex.z > cz)
		discard;
	if (sector == 5 && vertex.x < cx && vertex.y < cy && vertex.z > cz)
		discard;
	if (sector == 6 && vertex.x < cx && vertex.y < cy && vertex.z < cz)
		discard;
	if (sector == 7 && vertex.x < cx && vertex.y > cy && vertex.z < cz)
		discard;
	if (sector == 8 && vertex.x < cx && vertex.y > cy && vertex.z > cz)
		discard;
}

void cutAtSplineSurface() 
{
	vec3 u = gl_TexCoord[0].xyz;
	u.y = u.y / float(dimY);
	u.z = u.z / float(dimZ);

	if ( type9 == 5 ) 
	{
		if ( vertex.x < ( texture2D(cutTex, u.yz).r * float(dimX) ) )
			discard;
	}
	
}

void main() 
{
	if (gl_FrontFacing)
		;//discard;

	if (!showFS)
		cutFrontSector();

	if (cutAtSurface)
		cutAtSplineSurface();

	/* Normalize the normal. A varying variable CANNOT
	 // be modified by a fragment shader. So a new variable
	 // needs to be created. */
	vec3 n = normal.xyz;

	vec4 ambient = vec4(0.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	/* In this case the built in uniform gl_MaxLights is used
	 // to denote the number of lights. A better option may be passing
	 // in the number of lights as a uniform or replacing the current
	 // value with a smaller value. */
	 
	if ( lightOn )
	   calculateLighting(gl_MaxLights, -n, vertex.xyz, gl_FrontMaterial.shininess, ambient, diffuse, specular);

	vec4 color = vec4(0.0);

	if ( useTex ) 
	{
		vec3 v = gl_TexCoord[0].xyz;
		v.x = v.x / (float(dimX) * voxX);
		v.y = v.y / (float(dimY) * voxY);
		v.z = v.z / (float(dimZ) * voxZ);

		if (blendTex)
		{
		     if (show9) lookupTexMesh( color, type9, tex9, 0.0, v, alpha9 );
             if (show8) lookupTexMesh( color, type8, tex8, 0.0, v, alpha8 );
             if (show7) lookupTexMesh( color, type7, tex7, 0.0, v, alpha7 );
             if (show6) lookupTexMesh( color, type6, tex6, 0.0, v, alpha6 );
             if (show5) lookupTexMesh( color, type5, tex5, 0.0, v, alpha5 );
             if (show4) lookupTexMesh( color, type4, tex4, 0.0, v, alpha4 );
             if (show3) lookupTexMesh( color, type3, tex3, 0.0, v, alpha3 );
             if (show2) lookupTexMesh( color, type2, tex2, 0.0, v, alpha2 );
             if (show1) lookupTexMesh( color, type1, tex1, 0.0, v, alpha1 );
             if (show0) lookupTexMesh( color, type0, tex0, 0.0, v, alpha0 );
		}
		else
		{ 
		     if (show9) lookupTexMesh( color, type9, tex9, threshold9, v, alpha9 );
		     if (show8) lookupTexMesh( color, type8, tex8, threshold8, v, alpha8 );
		     if (show7) lookupTexMesh( color, type7, tex7, threshold7, v, alpha7 );
		     if (show6) lookupTexMesh( color, type6, tex6, threshold6, v, alpha6 );
		     if (show5) lookupTexMesh( color, type5, tex5, threshold5, v, alpha5 );
		     if (show4) lookupTexMesh( color, type4, tex4, threshold4, v, alpha4 );
		     if (show3) lookupTexMesh( color, type3, tex3, threshold3, v, alpha3 );
		     if (show2) lookupTexMesh( color, type2, tex2, threshold2, v, alpha2 );
		     if (show1) lookupTexMesh( color, type1, tex1, threshold1, v, alpha1 );
		     if (show0) lookupTexMesh( color, type0, tex0, threshold0, v, alpha0 );
		}

		color = color + (ambient * color / 2.0) + (diffuse * color) + (specular * color / 2.0);	
	}
	
	else if ( useCMAP && !useTex )
	{
		colorMap( color.rgb, myColor.r);
		color = color + (ambient * color / 2.0) + (diffuse * color / 2.0) + (specular * color / 2.0);			
	} 

	if ( color.rgb == vec3(0.0) ) 
	{
		color = gl_FrontLightModelProduct.sceneColor + (ambient
				* gl_FrontMaterial.ambient) + (diffuse
				* gl_FrontMaterial.diffuse) + (specular
				* gl_FrontMaterial.specular);
	}

	color.a = alpha_;
	color = clamp(color, 0.0, 1.0);

	if (useLic)
    {
        // gl_Color.r = LIC texture gray value
        // gl_Color.g = scalar product of triangle normal and input vector
        // gl_Color.b = FA value
        // gl_Color.a = noise texture gray value
       float fa = clamp( (gl_Color.b - 0.1) * 3., 0., 1. ); 
        
       float licBlend = (1.0 - gl_Color.g) * fa;
       vec4 tempColor = vec4(gl_Color.r, gl_Color.r, gl_Color.r, (1.0 - gl_Color.g) );
       vec4 licColor = clamp( tempColor * 1.8 - vec4(0.4), 0., 1.);
       
       
       float noiseBlend = clamp((gl_Color.g - 0.6),0., 1.) * clamp((fa - 0.2),0., 1.) * 3.;
       vec4 noiseColor = vec4( gl_Color.a );
       
       gl_FragColor = ((noiseColor - vec4(0.5)) * color * noiseBlend) + ((licColor - vec4(0.5)) * color * licBlend) + color;
    }
    else
        gl_FragColor = color;
}
