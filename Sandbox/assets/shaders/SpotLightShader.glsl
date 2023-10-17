// Basic Texture Shader

#type vertex
#version 330 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec3 a_Normal;
    layout(location = 2) in vec2 a_TexCoord;

    out vec2 v_TexCoord;
    out vec3 FragPosition;
    out vec3 Normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
    	v_TexCoord = a_TexCoord;
        gl_Position = projection * view * model * vec4(a_Position, 1.0);
        FragPosition = vec3(model * vec4(a_Position, 1.0));
        Normal = mat3(transpose(inverse(model))) * a_Normal;
    }

#type fragment
#version 330 core

    in vec3 FragPosition;
    in vec3 Normal;

    out vec4 FragColor;
    in vec2 v_TexCoord;

    uniform vec4 u_Color; 

    uniform float u_TilingFactor;
    uniform sampler2D u_Texture;

    uniform vec3 lightPos;
    uniform vec3 lightDir;
    uniform float cosInnerCone;
    uniform float cosOuterCone;
    uniform float maxDistance;
    uniform float innerConstantAttenuation;
    uniform float innerLinearAttenuation;
    uniform float innerQuadraticAttenuation;
    uniform float outerConstantAttenuation;
    uniform float outerLinearAttenuation;
    uniform float outerQuadraticAttenuation;
    uniform float shininess;
    uniform vec3 lightColor;
    uniform vec3 specularColor;

    float smoothstep(float edge0, float edge1, float x) {
        float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return t * t * (3.0 - 2.0 * t);
    }

    void main() {
        vec3 lightToFragment = FragPosition - lightPos;
        float distance = length(lightToFragment);
        lightToFragment = normalize(lightToFragment);

        float cosTheta = dot(lightToFragment, lightDir);
        float brightness = 0.0;

        if (cosTheta >= cosOuterCone && distance <= maxDistance) {
            // Calculate attenuation based on distance
            float attenuation = 1.0 / (outerConstantAttenuation + outerLinearAttenuation * distance + outerQuadraticAttenuation * distance * distance);

            if (cosTheta >= cosInnerCone) {
                // Inside the inner cone, calculate attenuation based on distance
                float innerAttenuation = 1.0 / (innerConstantAttenuation + innerLinearAttenuation * distance + innerQuadraticAttenuation * distance * distance);

                // Interpolate brightness smoothly between the inner and outer cones
                //float t = (cosTheta - cosInnerCone) / (cosOuterCone - cosInnerCone);
                float t = (cosTheta - cosOuterCone) / (cosInnerCone - cosOuterCone);
                brightness = smoothstep(0.0, 1.0, t) * innerAttenuation;
            } else {
                // Between inner and outer cones, calculate attenuation and interpolate brightness
                float t = (cosTheta - cosOuterCone) / (cosInnerCone - cosOuterCone);
                brightness = smoothstep(0.0, 1.0, t) * attenuation;
            }

            // Specular lighting
            vec3 viewDir = normalize(-FragPosition);
            vec3 reflectDir = reflect(-lightDir, Normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = specularColor * spec;

            // Final fragment color
            FragColor = vec4(lightColor * brightness, 1.0) + vec4(specular, 1.0) * texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
        } else {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }