#version 330

out vec4 FragColor;

uniform vec3 lightDir = vec3(0.5, 0.5, 0.0);  // Light direction

const vec2 resolution = vec2(256, 144);
uniform mat4 viewMatrix;

uniform float zoom;

const int MAX_STEPS = 100;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.01;

// Sphere properties
const vec3 sphereCenter = vec3(0.0, 0.0, 0.0);
const float sphereRadius = 1.0;
const vec3 sphereColor = vec3(1.0, 0.0, 0.0);

float calculateSDF(vec3 point)
{
    return length(point - sphereCenter) - sphereRadius;
}

vec3 calculateNormal(vec3 point)
{
    return normalize(point - sphereCenter);
}

void main()
{
    float aspectRatio = resolution.x / resolution.y;
    vec2 uv = ((gl_FragCoord.xy / resolution) * 2.0 - 1.0) / zoom;
    uv.x *= aspectRatio;

    // Transform ray origin and direction with the inverse view matrix
    vec4 rayOrigin = inverse(viewMatrix) * vec4(vec3(uv, 0.0), 1.0);
    vec4 rayDir = inverse(viewMatrix) * vec4(vec3(0.0, 0.0, -1.0), 0.0);

    float t = 0.0;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = vec3(rayOrigin) + t * vec3(rayDir);
        float d = calculateSDF(p);
        if (abs(d) < EPSILON)
        {
            vec3 normal = calculateNormal(p);
            vec3 normalizedLightDir = normalize(lightDir);

            float diffuse = max(dot(normal, normalizedLightDir), 0.0);
            vec3 ambient = 0.1 * sphereColor;
            vec3 color = (ambient + diffuse * sphereColor);

            FragColor = vec4(color, 1.0);
            return;
        }

        t += d;

        if (t > MAX_DISTANCE)
        {
            break;
        }
    }

    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
