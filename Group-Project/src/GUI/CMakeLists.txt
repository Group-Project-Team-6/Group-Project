cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(GUI STATIC
    GameMenu.h
    GameUI.h
    MainMenu.h
    PauseMenu.h
    PushdownMachine.h
    PushdownState.h
    SettingMenu.h
    State.h
    StateMachine.h
    StateTransition.h

    GameUI.cpp
    MainMenu.cpp
    PauseMenu.cpp
    PushdownMachine.cpp
    StateMachine.cpp
)