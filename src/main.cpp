#include "threepp/threepp.hpp"
#include "Mailbox.hpp"
#include <cmath>

using namespace threepp;

struct VehicleState { double x = 0, z = 0, heading = 0, speed = 0; };
struct Command      { double throttle = 0, steering = 0; };

// Simple kinematic model. Forward is the car's local -z, so a camera
// mounted on it (cameras look down -z) faces forward.
void stepPhysics(VehicleState& s, const Command& c, double dt) {
    s.speed   += c.throttle * dt;
    s.heading += c.steering * s.speed * dt;
    s.x -= s.speed * std::sin(s.heading) * dt;
    s.z -= s.speed * std::cos(s.heading) * dt;
}

int main() {
    Canvas canvas{"Simulator"};
    GLRenderer renderer{canvas};

    auto scene  = Scene::create();
    scene->background = Color::aliceblue;

    auto camera = PerspectiveCamera::create(75, canvas.aspect(), 0.1f, 1000.f);
    camera->position.set(0, 8, 12);
    camera->lookAt({0, 0, 0});

    auto groundMat = MeshBasicMaterial::create();
    groundMat->color = Color::gray;
    auto ground = Mesh::create(PlaneGeometry::create(100, 100), groundMat);
    ground->rotation.x = -math::PI / 2;
    scene->add(ground);

    auto carMat = MeshBasicMaterial::create();
    carMat->color = Color::red;
    auto car = Mesh::create(BoxGeometry::create(1, 0.5, 2), carMat);
    car->position.y = 0.25;
    scene->add(car);

    VehicleState state;
    Command cmd{0.5, 0.2};               // hard-coded for now; later from the network
    Mailbox<VehicleState> stateOut;      // the network thread will read this

    canvas.onWindowResize([&](const WindowSize& size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    const double dt = 1.0 / 240.0;
    double accumulator = 0.0;
    Clock clock;

    canvas.animate([&] {
        accumulator += clock.getDelta();
        while (accumulator >= dt) {
            stepPhysics(state, cmd, dt);
            accumulator -= dt;
        }
        stateOut.set(state);             // publish newest state for other threads

        // state to mesh, once per frame
        car->position.x = static_cast<float>(state.x);
        car->position.z = static_cast<float>(state.z);
        car->rotation.y = static_cast<float>(state.heading);

        renderer.render(*scene, *camera);
    });
}