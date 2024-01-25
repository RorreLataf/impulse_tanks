#ifndef SCREEN_H
#define SCREEN_H


class Screen
{


public:

    int framesCounter = 0;
    int finishScreen = 0;

    Screen();

    //virtual std::string Voice() const = 0;  // чисто виртуальная функция

    // Gameplay Screen Initialization logic
    virtual void Init(void) = 0;

    // Gameplay Screen Update logic
    virtual void Update(void) = 0;

    // Gameplay Screen Draw logic
    virtual void Draw(void) = 0;

    // Gameplay Screen Unload logic
    virtual void Unload(void) = 0;

    // Gameplay Screen should finish?
    virtual int Finish(void) = 0;
};

#endif // SCREEN_H
