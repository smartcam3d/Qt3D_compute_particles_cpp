#version 430

uniform float particleStep;
uniform float finalCollisionFactor;


layout (local_size_x = 1024) in;

struct ParticleData
{
    vec4 position;
    vec4 direction;
    vec4 color;
};

// Particles from previouse frame
layout (std430, binding = 0) coherent buffer Particles
{
    ParticleData particles[];
};

void main(void)
{
    uint globalId = gl_GlobalInvocationID.x;

    // Retrieve current particle from previous frame
    ParticleData currentParticle = particles[globalId];

    // New position = old position + distance traveled over step duration
    currentParticle.position = currentParticle.position + currentParticle.direction * particleStep;

    // Make acceleration more or less point toward the center of the scene
    vec4 acceleration =  normalize(vec4(0.0) - currentParticle.position) * finalCollisionFactor;

    // New velocity = old velocity + acceleration over step duration
    currentParticle.direction = currentParticle.direction + acceleration * particleStep;


    // Save updated particle
    particles[globalId] = currentParticle;
}
