/*
    Copyright 2016-2020 Arisotura

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#ifndef MAIN_H
#define MAIN_H

#include <string>

class EmuThread
{
    void run() override;

public:
    explicit EmuThread(void* parent = nullptr);

    void initOpenGL();
    void deinitOpenGL();

    void* oglGetProcAddress(const char* proc);

    void changeWindowTitle(char* title);

    // to be called from the UI thread
    void emuRun();
    void emuPause();
    void emuUnpause();
    void emuStop();

    bool emuIsRunning();

//signals:
    void windowUpdate();
    void windowTitleChange(std::string title);

    void windowEmuStart();
    void windowEmuStop();
    void windowEmuPause();
    void windowEmuReset();

    void windowLimitFPSChange();

    void screenLayoutChange();

    void windowFullscreenToggle();

private:
    volatile int EmuStatus;
    int PrevEmuStatus;
    int EmuRunning;
    int EmuPause;

    SDL_Surface* oglSurface;
    SDL_GLContext* oglContext;
};


class ScreenHandler
{

public:
    virtual ~ScreenHandler() {}

protected:
    void screenSetupLayout(int w, int h);

    SDL_Rect screenGetMinSize();

    void screenOnMousePress(SDL_MouseEvent* event);
    void screenOnMouseRelease(SDL_MouseEvent* event);
    void screenOnMouseMove(SDL_MouseEvent* event);

    float screenMatrix[2][6];

    bool touching;
};


class ScreenPanelNative : public ScreenHandler
{

public:
    explicit ScreenPanelNative(void* parent);
    ~ScreenPanelNative();

protected:
    void paintEvent(SDL_Event* event) override;

    void resizeEvent(SDL_Event* event) override;

    void mousePressEvent(SDL_MouseEvent* event) override;
    void mouseReleaseEvent(SDL_MouseEvent* event) override;
    void mouseMoveEvent(SDL_MouseEvent* event) override;

private: // slots:
    void onScreenLayoutChanged();

private:
    void setupScreenLayout();

    SDL_Texture screen[2];
    //QTransform screenTrans[2];
};


class ScreenPanelGL : public ScreenHandler //, protected QOpenGLFunctions_3_2_Core
{

public:
    explicit ScreenPanelGL(void* parent);
    ~ScreenPanelGL();

protected:
    void initializeGL() override;

    void paintGL() override;

    void resizeEvent(SDL_Event* event) override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(SDL_MouseEvent* event) override;
    void mouseReleaseEvent(SDL_MouseEvent* event) override;
    void mouseMoveEvent(SDL_MouseEvent* event) override;

private slots:
    void onScreenLayoutChanged();

private:
    void setupScreenLayout();

    //QOpenGLShaderProgram* screenShader;
    GLuint screenVertexBuffer;
    GLuint screenVertexArray;
    GLuint screenTexture;
};


class MainWindow
{

public:
    explicit MainWindow(void* parent = nullptr);
    ~MainWindow();

    bool hasOGL;
    SDL_GLContext* getOGLContext();

protected:
    void resizeEvent(SDL_Event* event) override;

    void keyPressEvent(SDL_KeyEvent* event) override;
    void keyReleaseEvent(SDL_KeyEvent* event) override;

    void dragEnterEvent(SDL_Event* event) override;
    void dropEvent(SDL_Event* event) override;

//signals:
    void screenLayoutChange();

private: //slots:
    void onOpenFile();
    void onBootFirmware();
    void onSaveState();
    void onLoadState();
    void onUndoStateLoad();
    void onImportSavefile();
    void onQuit();

    void onPause(bool checked);
    void onReset();
    void onStop();
    void onEnableCheats(bool checked);
    void onSetupCheats();
    void onCheatsDialogFinished(int res);

    void onOpenEmuSettings();
    void onEmuSettingsDialogFinished(int res);
    void onOpenInputConfig();
    void onInputConfigFinished(int res);
    void onOpenVideoSettings();
    void onOpenAudioSettings();
    void onAudioSettingsFinished(int res);
    void onOpenWifiSettings();
    void onWifiSettingsFinished(int res);
    void onChangeSavestateSRAMReloc(bool checked);
    void onChangeScreenSize();
    void onChangeScreenRotation(QAction* act);
    void onChangeScreenGap(QAction* act);
    void onChangeScreenLayout(QAction* act);
    void onChangeScreenSizing(QAction* act);
    void onChangeIntegerScaling(bool checked);
    void onChangeScreenFiltering(bool checked);
    void onChangeShowOSD(bool checked);
    void onChangeLimitFramerate(bool checked);
    void onChangeAudioSync(bool checked);

    void onTitleUpdate(QString title);

    void onEmuStart();
    void onEmuStop();

    void onUpdateVideoSettings(bool glchange);

    void onFullscreenToggled();

private:
    void createScreenPanel();

    std::string loadErrorStr(int error);

public:
    /*QWidget* panel;

    QAction* actOpenROM;
    QAction* actBootFirmware;
    QAction* actSaveState[9];
    QAction* actLoadState[9];
    QAction* actUndoStateLoad;
    QAction* actImportSavefile;
    QAction* actQuit;

    QAction* actPause;
    QAction* actReset;
    QAction* actStop;
    QAction* actEnableCheats;
    QAction* actSetupCheats;

    QAction* actEmuSettings;
    QAction* actInputConfig;
    QAction* actVideoSettings;
    QAction* actAudioSettings;
    QAction* actWifiSettings;
    QAction* actSavestateSRAMReloc;
    QAction* actScreenSize[4];
    QActionGroup* grpScreenRotation;
    QAction* actScreenRotation[4];
    QActionGroup* grpScreenGap;
    QAction* actScreenGap[6];
    QActionGroup* grpScreenLayout;
    QAction* actScreenLayout[3];
    QActionGroup* grpScreenSizing;
    QAction* actScreenSizing[4];
    QAction* actIntegerScaling;
    QAction* actScreenFiltering;
    QAction* actShowOSD;
    QAction* actLimitFramerate;
    QAction* actAudioSync;*/
};

#endif // MAIN_H
