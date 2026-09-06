#include "scene.h"
#include "GridRenderer.h"

class RenderSystem
{
public:
    void Init()
    {
        m_Grid.Init();

    }
    void Update(const Scene& scene, Renderer& renderer, const Camera& camera, Entity selected)
    {
        for (const auto& [entity, meshRenderer] : scene.meshRenderers)
        {
            if (!scene.transforms.count(entity)) continue;

            const auto& transform = scene.transforms.at(entity);

            Material* material = meshRenderer.material;

            // 👇 Handle selection HERE, not in Renderer
            glm::vec3 originalColor = material->color;

            if (entity == selected)
                material->color = glm::vec3(1.0f, 0.5f, 1.0f);

            material->Bind(
                transform.GetMatrix(),
                camera.GetViewMatrix(),
                camera.GetProjection()
            );

            renderer.Draw(*meshRenderer.mesh);

            material->color = originalColor;
        }
        m_Grid.Draw(camera.GetViewMatrix(),camera.GetProjection(),0.1f,1000.0f);

    }

private:
    GridRenderer m_Grid;

};