#include "PauseMenu.h"
#include <iostream>

using namespace NCL;
using namespace CSC8503;

void PauseMenu::Draw()
{
    const ImGuiViewport* mainVp = ImGui::GetMainViewport();

    //Draw background
    ImGui::SetNextWindowPos(ImVec2(mainVp->WorkPos.x, mainVp->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);
    
    if (!ImGui::Begin("Pause Background", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    
    //Draw menu
    ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y - 100), ImGuiCond_Always);
    //ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
   
    ImGui::BeginChild("Pause Menu", ImVec2(300, 230), true, ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(1.2);
    float contentWidth = ImGui::GetWindowContentRegionWidth();

    std::string t = "Play";
    if (hasInit) t = "Continue";
    mainLevel = ImGui::Button(t.c_str(), ImVec2(contentWidth, 50));

    settingLevel = ImGui::Button("Settings", ImVec2(contentWidth, 50));

    
    if (!hasInit) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.1, 0.1, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.1, 0.1, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));
    }
    menuClose = ImGui::Button("Cancel", ImVec2(contentWidth, 50));
    if (!hasInit) {
        menuClose = false;
    }
    ImGui::PopStyleColor(3);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.05, 0.05, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6, 0.1, 0.1, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9, 0.1, 0.1, 1));
    quitGame = ImGui::Button("Quit Game", ImVec2(contentWidth, 50));
    ImGui::PopStyleColor(3);

    ImGui::EndChild();

    ImGui::End();
}

void SettingMenu::Draw()
{
    const ImGuiViewport* mainVp = ImGui::GetMainViewport();

    //Draw menu
    ImGui::SetNextWindowPos(ImVec2(mainVp->WorkPos.x, mainVp->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);

    if (!ImGui::Begin("Setting Background", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    //Draw menu
    ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y - 100), ImGuiCond_Always);
    //ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);

    ImGui::BeginChild("Setting Menu", ImVec2(300, 230), true, ImGuiWindowFlags_NoSavedSettings);

    ImGui::SetWindowFontScale(1.2);
    float contentWidth = ImGui::GetWindowContentRegionWidth();

    /*ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.1, 0.1, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));*/
    ImGui::Checkbox("Debbug Mode",&toggleDebug);

    //settingLevel = ImGui::Button("Settings", ImVec2(contentWidth, 50));
    //ImGui::PopStyleColor(3);
    //menuClose = ImGui::Button("Cancel", ImVec2(contentWidth, 50));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.05, 0.05, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6, 0.1, 0.1, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9, 0.1, 0.1, 1));
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 40));
    back = ImGui::Button("Back", ImVec2(contentWidth, 50));
    ImGui::PopStyleColor(3);

    ImGui::EndChild();
    ImGui::End();
}

void GameHUD::Draw()
{
    const ImGuiViewport* mainVp = ImGui::GetMainViewport();

    //Draw menu
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3, 0.05, 0.05, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (!ImGui::Begin("Setting Background", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::End();
        return;
    }

    ImGui::SetWindowFontScale(1.2);
    float contentWidth = ImGui::GetWindowContentRegionWidth();

    //Player 1 HP bar
    if (numPlayer > 0) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Button("Player 1", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * (-0.4 * (1.0 - hp[0])), 0));
        ImGui::Button(" ", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);
    }

    //Player 2 HP bar
    if (numPlayer > 1) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::SetCursorPos(ImVec2(contentWidth * (0.6), 0));
        ImGui::Button("Player 2", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * (0.6 + (0.4 * (1.0 - hp[1]))), 0));
        ImGui::Button(" ", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);
    }

    //Player 3 HP bar
    if (numPlayer > 2) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05,1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::SetCursorPos(ImVec2(0, mainVp->Size.y - 30));
        ImGui::Button("Player 3", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * (-0.4 * (1.0 - hp[2])), mainVp->Size.y -30));
        ImGui::Button(" ", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);
    }

    //Player 4 HP bar
    if (numPlayer > 3) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 1));
        ImGui::SetCursorPos(ImVec2(contentWidth * (0.6), mainVp->Size.y - 30));
        ImGui::Button("Player 4", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 1, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * (0.6 + (0.4 * (1.0 - hp[3]))), mainVp->Size.y - 30));
        ImGui::Button(" ", ImVec2(contentWidth * 0.4, 30));
        ImGui::PopStyleColor(3);
    }

    //Team 1 score
    if (numTeam > 0) {
        float r = 0.45;
        if (numTeam > 2) r = 0.4;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(0, mainVp->Size.y * r));
        ImGui::Button("Team 1:", ImVec2(contentWidth * 0.1, mainVp->Size.y * 0.1));
        ImGui::PopStyleColor(3);
    }

    //Team 2 score
    if (numTeam > 1) {
        float r = 0.45;
        if (numTeam > 2) r = 0.4;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * 0.9, mainVp->Size.y * r));
        ImGui::Button("Team 2:", ImVec2(contentWidth * 0.1, mainVp->Size.y * 0.1));
        ImGui::PopStyleColor(3);
    }

    //Team 3 score
    if (numTeam > 2) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * 0.9, mainVp->Size.y * 0.5));
        ImGui::Button("Team 3:", ImVec2(contentWidth * 0.1, mainVp->Size.y * 0.1));
        ImGui::PopStyleColor(3);
    }

    //Team 4 score
    if (numTeam > 3) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05, 0.05, 0.05, 0.5));
        ImGui::SetCursorPos(ImVec2(contentWidth * 0.9, mainVp->Size.y * 0.5));
        ImGui::Button("Team 4:", ImVec2(contentWidth * 0.1, mainVp->Size.y * 0.1));
        ImGui::PopStyleColor(3);
    }


    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);

    if (debug) {
        ImGui::SetNextWindowPos(ImVec2(0, mainVp->Size.y * 0.8), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x * 0.2, mainVp->Size.y * 0.2), ImGuiCond_Always);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3, 0.05, 0.05, 0.3));
        ImGui::Begin("Debug Tool", NULL, 0);

        // Plot some values
        if(fps.size() > 0) ImGui::PlotLines("FPS: ", fps.data(), IM_ARRAYSIZE(fps.data()));

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = msg.size() - 1; n >= 0; n--)
            ImGui::Text(msg[n].c_str());
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopStyleColor(1);
    }
}

void GameHUD::AddMessage(std::string s) {
    if (msg.size() > msgLimit) {
        msg.erase(msg.begin());
        msg.shrink_to_fit();
        msg.push_back(s);
    }
}
void GameHUD::AddFPS(float s) {
    if (fps.size() > fpsLimit) {
        fps.erase(fps.begin());
        fps.shrink_to_fit();
        fps.push_back(s);
    }
}