#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position; 
layout(location = 1) in vec2 texCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 v_TexCoord;

void main() 
{
    gl_Position = u_Projection * u_View * u_Model * position;
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

uniform bool u_UseTexture;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TexCoord;

out vec4 FragColor;

void main()
{
    if (u_UseTexture)
    {
        FragColor = texture(u_Texture, v_TexCoord);
    }
    else
    {
        FragColor = u_Color;
    }
}