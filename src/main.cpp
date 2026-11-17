#include "threepp/threepp.hpp"
using namespace threepp;

int main() {
    Canvas canvas("sim", {{"aa", 4}});
    GLRenderer renderer(canvas);

    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.set(0, 10, 15);

    OrbitControls controls{*camera, canvas};

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    auto groundMat = MeshBasicMaterial::create();
    groundMat->color = Color::gray;
    auto ground = Mesh::create(PlaneGeometry::create(50, 50), groundMat);
    ground->rotation.x = -math::PI / 2; // Make the plane as the ground
    scene->add(ground);

    auto boxMat = MeshBasicMaterial::create();
    boxMat->color = Color::red;
    auto box = Mesh::create(BoxGeometry::create(1, 0.5, 2), boxMat);
    box->position.y = 0.25;
    scene->add(box);

    canvas.onWindowResize([&](const WindowSize& size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    const float physicsDt = 1.0f / 240.0f;
    float accumulator = 0.0f;
    Clock clock;

    canvas.animate([&] {
        accumulator += clock.getDelta();
        while (accumulator >= physicsDt) {
            box->position.z -= 2.0f * physicsDt;
            accumulator -= physicsDt;
        }
        renderer.render(*scene, *camera);
    });
}