    #include"Deimgui.h"
       
    // ── DOCKSPACE ────────────────────────────────────────────────────────────
    void Deimgui::ShowDockspace() {
        static bool open = true;
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_MenuBar        | ImGuiWindowFlags_NoDocking      |
            ImGuiWindowFlags_NoTitleBar     | ImGuiWindowFlags_NoCollapse     |
            ImGuiWindowFlags_NoResize       | ImGuiWindowFlags_NoMove         |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::SetNextWindowViewport(vp->ID);

        ImGui::Begin("DockSpace Master", &open, flags);
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0,0),
                         ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }

    // ── TOOLBAR ──────────────────────────────────────────────────────────────
    void Deimgui::ShowToolbar() {
        ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x, vp->WorkPos.y));
        ImGui::SetNextWindowSize(ImVec2(vp->WorkSize.x, 50));
        ImGui::SetNextWindowBgAlpha(0.95f);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoMove    | ImGuiWindowFlags_NoResize  |
            ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("##Toolbar", nullptr, flags);
        ImGui::SetCursorPosY(7);

        if (ImGui::Button("Save", ImVec2(70,35))) gameState.SaveScene();
        ImGui::SameLine(0,10);
        if (ImGui::Button("Load", ImVec2(70,35))) gameState.LoadScene();
        ImGui::SameLine(0,10);
        if (ImGui::Button("New",  ImVec2(70,35))) gameState.NewScene();

        ImGui::SameLine(0,20); ImGui::Separator(); ImGui::SameLine(0,20);

        if (ImGui::Button(gameState.isPlaying ? "Playing" : "Play", ImVec2(70,35))) {
            gameState.isPlaying = !gameState.isPlaying;
            if (gameState.isPlaying) gameState.isPaused = false;
        }
        ImGui::SameLine(0,10);
        if (ImGui::Button(gameState.isPaused ? "Resume" : "Pause", ImVec2(70,35)))
            gameState.isPaused = !gameState.isPaused;
        ImGui::SameLine(0,10);
        if (ImGui::Button("Stop", ImVec2(70,35))) {
            gameState.isPlaying = false;
            gameState.isPaused  = false;
        }

        ImGui::End();
    }

    // ── HIERARCHY ────────────────────────────────────────────────────────────
    void Deimgui::ShowHierarchy() {
        ImGui::Begin("Hierarchy", &showHierarchy, ImGuiWindowFlags_NoCollapse);

        ImGui::Text("Scene: %s", gameState.currentScenePath.c_str());
        ImGui::Separator();

        if (!gameState.scene) {
            ImGui::TextDisabled("No scene loaded");
            ImGui::End();
            return;
        }

        ImGui::Text("Entities: %d", (int)gameState.scene->entities.size());

        // ── Add entity input + button ─────────────────────────────────────
        static char newName[64] = "Entity";
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 80.0f);
        ImGui::InputText("##newname", newName, sizeof(newName));
        ImGui::SameLine();
        if (ImGui::Button("+ Add", ImVec2(-1, 0))) {
            // Scene decides everything — ImGui just passes the name
            Entity e = gameState.scene->CreateEntity(newName);
            gameState.selectedObjectID = (int)e;
            strncpy(newName, "Entity", sizeof(newName));
        }

        ImGui::Separator();

        // ── Entity list ───────────────────────────────────────────────────
        if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
            Entity toDelete = (Entity)-1;

            for (Entity e : gameState.scene->entities) {
                const std::string& label = gameState.scene->names.count(e)
                    ? gameState.scene->names[e]
                    : "Entity " + std::to_string(e);

                bool selected = (gameState.selectedObjectID == (int)e);
                if (ImGui::Selectable(label.c_str(), selected))
                    gameState.selectedObjectID = (int)e;

                // Right-click menu — only scene operations, no component logic
                if (ImGui::BeginPopupContextItem()) {
                    ImGui::TextDisabled("%s", label.c_str());
                    ImGui::Separator();
                    if (ImGui::MenuItem("Delete"))
                        toDelete = e;
                    ImGui::EndPopup();
                }
            }

            // Defer deletion to avoid invalidating the loop iterator
            if (toDelete != (Entity)-1) {
                if (gameState.selectedObjectID == (int)toDelete)
                    gameState.selectedObjectID = -1;
                gameState.scene->DestroyEntity(toDelete);  // Scene owns this
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }

    // ── INSPECTOR ────────────────────────────────────────────────────────────
    // Pure display — reads whatever components the Scene has on this entity.
    // ImGui never adds or removes components; Scene does that externally.
    void Deimgui::ShowInspector() {
        ImGui::Begin("Inspector", &showInspector, ImGuiWindowFlags_NoCollapse);

        if (!gameState.scene || gameState.selectedObjectID == -1) {
            ImGui::TextDisabled("Select an entity to view properties");
            ImGui::End();
            return;
        }

        Entity e = (Entity)gameState.selectedObjectID;

        // Editable name
        if (gameState.scene->names.count(e)) {
            char buf[64];
            strncpy(buf, gameState.scene->names[e].c_str(), sizeof(buf));
            buf[sizeof(buf)-1] = '\0';
            if (ImGui::InputText("##ename", buf, sizeof(buf)))
                gameState.scene->names[e] = buf;
        }
        ImGui::SameLine();
        ImGui::TextDisabled("(#%u)", e);
        ImGui::Separator();

        ShowProperties(e);

        ImGui::End();
    }

    // ── STATS ────────────────────────────────────────────────────────────────
    void Deimgui::ShowStats() {
        ImGui::SetNextWindowDockID(ImGui::GetID("MyDockSpace"), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
        ImGui::Begin("Stats", &showStats, ImGuiWindowFlags_NoCollapse);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::SameLine(150);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
        ImGui::Separator();

        ImGui::Text("Vertices: 24");  ImGui::SameLine(150); ImGui::Text("Triangles: 12");
        ImGui::Text("Draw Calls: 2"); ImGui::SameLine(150); ImGui::Text("Batches: 1");

        ImGui::Separator();
        if      (gameState.isPlaying && !gameState.isPaused)
            ImGui::TextColored(ImVec4(0.2f,1.0f,0.2f,1.0f), "PLAYING");
        else if (gameState.isPlaying &&  gameState.isPaused)
            ImGui::TextColored(ImVec4(1.0f,0.8f,0.0f,1.0f), "PAUSED");
        else
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.5f,1.0f), "STOPPED");

        ImGui::End();
    }

    // ── PROPERTIES ───────────────────────────────────────────────────────────
    // Only renders components that ALREADY EXIST on the entity.
    // No component is created or destroyed here.
    void Deimgui::ShowProperties(Entity e) {

        // Transform — show if scene has it for this entity
        if (gameState.scene->transforms.count(e)) {
            auto& t = gameState.scene->transforms[e];
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::DragFloat3("Position", &t.position.x, 0.01f);
                ImGui::DragFloat3("Rotation", &t.rotation.x, 1.0f);
                ImGui::DragFloat3("Scale",    &t.scale.x,    0.01f);
                ImGui::Spacing();
                if (ImGui::SmallButton("Reset")) {
                    t.position = glm::vec3(0.0f);
                    t.rotation = glm::vec3(0.0f);
                    t.scale    = glm::vec3(1.0f);
                }
            }
        }

        // MeshRenderer — show if scene has it for this entity
        if (gameState.scene->meshRenderers.count(e)) {
            auto& mr = gameState.scene->meshRenderers[e];
            if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Mesh:     %s", mr.mesh     ? "Assigned" : "None");
                ImGui::Text("Material: %s", mr.material ? "Assigned" : "None");
                if (mr.material)
                    ImGui::ColorEdit3("Color", &mr.material->color.x);
            }
        }

        // Physics — show if scene has it for this entity
        if (gameState.scene->physics.count(e)) {
            auto& ph = gameState.scene->physics[e];
            if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::DragFloat3("Velocity",  &ph.velocity.x, 0.01f);
            }
        }
    }