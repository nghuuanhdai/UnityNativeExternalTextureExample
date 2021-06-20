Shader "NativeOpenGLRender/OES_External_Tex"
{
    Properties {
        _MainTex ("Texture", 2D) = "white" {}
        _UvTopLeftRight ("UV of top corners", Vector) = (0, 1, 1, 1)
        _UvBottomLeftRight ("UV of bottom corners", Vector) = (0 , 0, 1, 0) 
    }

    // For GLES3 or GLES2 on device
    SubShader
    {
        Pass
        {
            ZWrite Off
            GLSLPROGRAM
            #pragma only_renderers gles3 gles
            #ifdef SHADER_API_GLES3
            #extension GL_OES_EGL_image_external_essl3 : require
            #else
            #extension GL_OES_EGL_image_external : require
            #endif
            uniform vec4 _UvTopLeftRight;
            uniform vec4 _UvBottomLeftRight;
            #ifdef VERTEX
            varying vec2 textureCoord;
            void main()
            {
                vec2 uvTop = mix(_UvTopLeftRight.xy, _UvTopLeftRight.zw, gl_MultiTexCoord0.x);
                vec2 uvBottom = mix(_UvBottomLeftRight.xy, _UvBottomLeftRight.zw, gl_MultiTexCoord0.x);
                textureCoord = mix(uvTop, uvBottom, gl_MultiTexCoord0.y);
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif
            #ifdef FRAGMENT
            varying vec2 textureCoord;
            uniform samplerExternalOES _MainTex;

            void main()
            {
                #ifdef SHADER_API_GLES3
                gl_FragColor = texture(_MainTex, textureCoord);
                #else
                gl_FragColor = textureExternal(_MainTex, textureCoord);
                #endif
            }

            #endif
            ENDGLSL
        }
    }
}
