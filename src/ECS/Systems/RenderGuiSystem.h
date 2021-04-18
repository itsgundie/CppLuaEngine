#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H


#include "ECS.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "glm/glm.hpp"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "HealthComponent.h"
#include "ProjectileEmitterComponent.h"

class RenderGuiSystem: public System
{
    public:
        RenderGuiSystem() = default;

    void Update(const std::unique_ptr<Registry>& registry, const SDL_Rect& camera)
    {
		ImGui::NewFrame();

        if (ImGui::Begin("Spawn Enemies"))
        {
            ImGui::Text("There you can spawn enemies!");
            static int32_t enemyPosX = 0;
            static int32_t enemyPosY = 0;
            static int32_t scaleX = 1;
            static int32_t scaleY = 1;
            static int32_t velocityX = 0;
            static int32_t velocityY = 0;
            static int32_t health = 100;
            static int32_t damage = 10;
            static float_t rotation = 0.0f;
            static float_t projectileAngle = 0.0f;
            static float_t projectileSpeed = 100.0f;
            static int32_t projectileRepeat = 1;
            static int32_t projectileDuration = 10;
            const char* sprites[] = {
                "tank_panther_up",
                "tank_panther_right",
                "tank_panther_down",
                "tank_panther_left"
                };
            static int32_t selectedSpriteIndex = 0;
            static int32_t zIndex = 1;

            if (ImGui::CollapsingHeader("Select Sprite", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Combo("texture ID", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
                ImGui::SliderInt("Render Layer", &zIndex, -1, 99);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Set Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("X-position", &enemyPosX);
                ImGui::InputInt("Y-position", &enemyPosY);
                ImGui::SliderInt("Scale by X-axis", &scaleX, 1, 10);
                ImGui::SliderInt("Scale by Y-axis", &scaleY, 1, 10);
                ImGui::SliderAngle("Rotate by Degrees", &rotation, 0, 360);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("RigidBody Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::InputInt("Velocity at X-axis", &velocityX);
                ImGui::InputInt("Velocity at Y-axis", &velocityY);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Projectile Emitter Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderAngle("Angle in Degrees", &projectileAngle, 0, 360);
                ImGui::SliderFloat("Speed (px/sec)", &projectileSpeed, 0, 5000);
                ImGui::InputInt("Repeat Fire Cooldown (sec)", &projectileRepeat);
                ImGui::InputInt("Lifetime(sec)", &projectileDuration);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderInt("Health, %", &health, 0, 100);
            }
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Damage", ImGuiTreeNodeFlags_DefaultOpen))
                ImGui::SliderInt("Damage, Value", &damage, 0, 100);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Spawn Enemy!"))
            {
                Entity enemy = registry->CreateEntity();
                enemy.AddComponent<TransformComponent>(glm::vec2(enemyPosX, enemyPosY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(velocityX, velocityY));
                enemy.AddComponent<SpriteComponent>(sprites[selectedSpriteIndex], 32, 32, zIndex);
                enemy.AddComponent<BoxColliderComponent>(32 * enemy.GetComponent<TransformComponent>().scale.x,
                                                        32 * enemy.GetComponent<TransformComponent>().scale.y);
                double_t projectileVelocityX = cos(projectileAngle) * projectileSpeed;
                double_t projectileVelocityY = sin(projectileAngle) * projectileSpeed;

                enemy.AddComponent<ProjectileEmitterComponent>(
                    glm::vec2(projectileVelocityX, projectileVelocityY),
                    projectileRepeat * 1000,
                    projectileDuration * 1000,
                    damage,
                    false
                    );
                enemy.AddComponent<HealthComponent>(health);
                enemy.Group("enemy");

            enemyPosX = enemyPosY = velocityX = velocityY = 0;
            scaleX = scaleY = 1;
            health = 100;
            damage = 10;
            rotation = 0.0f;
            projectileAngle = 0.0f;
            projectileSpeed = 100.0f;
            projectileRepeat = 1;
            projectileDuration = 10;
            selectedSpriteIndex = 0;
            zIndex = 1;
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.88f);

        if (ImGui::Begin("Map Space Coordinates", NULL, (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav)))
        {
            ImGui::Text(
                "This coordinates are [ X = %.1f, Y = %.1f]",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y
            );
        }

        ImGui::End();
		ImGui::Render();

		ImGuiSDL::Render(ImGui::GetDrawData());

    }  
    
};		

#endif