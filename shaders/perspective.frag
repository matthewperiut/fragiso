#version 330

out vec4 FragColor;

const vec2 resolution = vec2(256, 144);
uniform float time;
uniform mat4 viewMatrix;

const int MAX_STEPS = 100;
const float MAX_DISTANCE = 100.0;
const float EPSILON = 0.1;

// Sphere properties
const vec3 sphereCenter = vec3(0.0, 0.0, 0.0);
const float sphereRadius = 1.0;
const vec3 sphereColor = vec3(1.0, 0.0, 0.0);

// Light properties
const vec3 lightPosition = vec3(2.0, 2.0, -3.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float ambientIntensity = 0.2;
const float diffuseIntensity = 0.8;
const float specularIntensity = 0.5;
const float shininess = 32.0;

vec3 calculateNormal(vec3 point)
{
    return normalize(point - sphereCenter);
}

vec3 calculateLighting(vec3 point, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPosition - point);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diffuse = max(dot(normal, lightDir), 0.0);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0), shininess);

    vec3 ambientColor = ambientIntensity * sphereColor;
    vec3 diffuseColor = diffuseIntensity * diffuse * sphereColor * lightColor;
    vec3 specularColor = specularIntensity * specular * lightColor;

    return ambientColor + diffuseColor + specularColor;
}

void main()
{
    // Calculate the aspect ratio
    float aspectRatio = resolution.x / resolution.y;

    // Convert pixel coordinates to normalized device coordinates
    vec2 uv = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    uv.x *= aspectRatio; // Adjust the x-coordinate based on the aspect ratio

    // Extrapolate camera position and direction from view matrix
    vec3 cameraPosition = vec3(inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0));
    vec3 cameraDirection = normalize(vec3(inverse(viewMatrix) * vec4(uv, -1.0, 0.0)));

    float distance = 0.0;
    vec3 rayOrigin = cameraPosition;

    // Ray marching loop
    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 rayPoint = rayOrigin + distance * cameraDirection;

        // Sphere intersection check
        float sphereDistance = length(rayPoint - sphereCenter) - sphereRadius;

        if (sphereDistance < EPSILON)
        {
            // Ray hit the sphere, calculate shading
            vec3 normal = calculateNormal(rayPoint);
            vec3 viewDir = normalize(cameraPosition - rayPoint);
            vec3 color = calculateLighting(rayPoint, normal, viewDir);

            FragColor = vec4(color, 1.0);
            return;
        }

        distance += min(sphereDistance, MAX_DISTANCE);

        if (distance >= MAX_DISTANCE)
            break;
    }

    // No intersection, set fragment color to background
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
