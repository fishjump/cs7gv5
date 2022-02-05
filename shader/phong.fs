#version 330 core

struct material_t {
    float shininess;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct light_t {
    vec3 position;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 texture_coordinate;

out vec4 frag_color;

uniform vec3 view_pos;
uniform material_t material;
uniform light_t light;
uniform sampler2D texture_diffuse1;

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(view_pos - frag_pos);
    vec3 light_direction = normalize(frag_pos - light.position);
    vec3 reflect_direction = reflect(light_direction, norm);

    // ambient
    vec3 ambient = light.ambient_color * material.ambient_color;

    // diffuse 
    float diff = max(dot(norm, -light_direction), 0.0);
    vec3 diffuse = diff * light.diffuse_color * material.diffuse_color;

    // specular
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = spec * light.specular_color * material.specular_color;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}