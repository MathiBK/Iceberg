#include "config_state.h"
#include <iostream>
#include <sstream>
#include <regex>

static const float VIEW_HEIGHT = 900.f;
static const float VIEW_WIDTH = 1600.f;

Config::Config(StateMachine *stateMachine) : BaseState(stateMachine)
{
    sf::Vector2f pos = sf::Vector2f(this->stateMachine->renderWindow.getSize());
    view.setSize(pos);
    this->stateMachine->renderWindow.setView(view);

    chosenFrame = std::to_string(stateMachine->saveManager.m_framerate);
    std::stringstream s;
    s << std::to_string(stateMachine->saveManager.m_screenWidth) << ", " << std::to_string(stateMachine->saveManager.m_screenHeight);
    chosenRes = s.str();
    fullscreenInt = stateMachine->saveManager.m_fullscreen;

    font.loadFromFile("media/fonts/Manjari-Regular.otf");

    menuPic.loadFromFile("media/Iceberg.png");
    menuPic.setSmooth(true);
    sprite.setTexture(menuPic);

    backButton.setFillColor(sf::Color::Black);
    backButton.setFont( font );
    backButton.setStyle( sf::Text::Bold );
    backButton.setString( "Back" );
    backButton.setCharacterSize(40 );
    options.push_back(&backButton);

    applyButton.setFillColor(sf::Color::Black);
    applyButton.setFont( font );
    applyButton.setStyle( sf::Text::Bold );
    applyButton.setString( "Apply" );
    applyButton.setCharacterSize(40 );
    options.push_back(&applyButton);

    framerate.setFillColor(sf::Color::Black);
    framerate.setStyle( sf::Text::Bold );
    framerate.setString("Framerate:");
    framerate.setFont(font);
    framerate.setCharacterSize(40);

    fullscreen.setFillColor(sf::Color::Black);
    fullscreen.setString("Fullscreen");
    fullscreen.setFont(font);
    fullscreen.setCharacterSize(40);
    options.push_back(&fullscreen);

    res.setFillColor(sf::Color::Black);
    res.setStyle( sf::Text::Bold );
    res.setString("Resolution:");
    res.setFont(font);
    res.setCharacterSize(40);

    resolutionsNumber = {{1920, 1080}, {1680, 1050}, {1600, 1024}, {1600, 900}, {1440, 900}, {1366, 768}, {1360, 768}, {1280, 1024}, {1280, 960}, {1280, 800}, {1280, 768}, {1280, 720}, {1152, 864}, {1024, 768}, {800, 600}};
    framerateNumber = {30, 60, 120, 144, 1000};

    settings.setFillColor(sf::Color::Black);
    settings.setStyle( sf::Text::Bold );
    settings.setString("Settings");
    settings.setFont(font);
    settings.setCharacterSize(64);

    for(int& i : framerateNumber){
        auto s = new sf::Text;
        s->setString(std::to_string(i));
        s->setFont(font);
        s->setFillColor(sf::Color::Black);
        s->setCharacterSize(30);
        framerates.push_back(s);
        options.push_back(s);
    }

    for(sf::Vector2i& r : resolutionsNumber){
        std::stringstream st;
        st << std::to_string(r.x) << ", " << std::to_string(r.y);
        auto s = new sf::Text;
        s->setString(st.str());
        s->setFont(font);
        s->setFillColor(sf::Color::Black);
        s->setCharacterSize(30);
        resolutions.push_back(s);
        options.push_back(s);
    }
    resizeView(stateMachine->renderWindow, view);
}

void Config::resizeView(const sf::RenderWindow& window, sf::View& v)
{
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);

    sf::Vector2f prevSize = v.getSize();

    v.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

void Config::draw(const float deltaTime)
{
    stateMachine->renderWindow.setView(view);
    sf::Vector2f c = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f topLeft = sf::Vector2f(c.x - viewSize.x/2.f,c.y - viewSize.y/2.f);
    settings.setPosition(topLeft.x + (view.getSize().x / 10.f) * 6, topLeft.y + 10.0f);
    framerate.setPosition(topLeft.x + (view.getSize().x / 10.f) * 5, topLeft.y + 130.0f);
    fullscreen.setPosition(topLeft.x + (view.getSize().x / 10.f) * 5, topLeft.y + 400.0f);
    res.setPosition(topLeft.x +(view.getSize().x / 10.f) * 8, topLeft.y + 130.0f);
    backButton.setPosition(topLeft.x + (view.getSize().x / 10.f) * 0.5, topLeft.y + 20.0f);
    applyButton.setPosition(topLeft.x + (view.getSize().x / 10.f) * 0.5, topLeft.y + 80.0f);
    sprite.setPosition(topLeft.x + (view.getSize().x / 10.f) * 0.5, topLeft.y);

    if(fullscreenInt == 1){
        fullscreen.setFillColor(sf::Color::Yellow);
    }

    float posY = 190;
    for(sf::Text* t : resolutions){
        if(t->getString() == chosenRes){
            t->setFillColor(sf::Color::Yellow);
        }
        t->setPosition(topLeft.x + (view.getSize().x/10.f)*8, topLeft.y + posY);
        posY += 30;
    }

    posY = 190;
    for(sf::Text* f : framerates){
        if(f->getString() == chosenFrame){
            f->setFillColor(sf::Color::Yellow);
        }
        f->setPosition(topLeft.x + (view.getSize().x/10.f)*5, topLeft.y + posY);
        posY += 30;
    }

    this->stateMachine->renderWindow.clear(sf::Color(129, 161, 188));
    this->stateMachine->renderWindow.draw(sprite);
    for(sf::Text* t : options){
        this->stateMachine->renderWindow.draw(*t);
    }
    this->stateMachine->renderWindow.draw(settings);
    this->stateMachine->renderWindow.draw(fullscreen);
    this->stateMachine->renderWindow.draw(framerate);
    this->stateMachine->renderWindow.draw(res);

}

void Config::inputHandler()
{
    sf::Event event {};

    while(this->stateMachine->renderWindow.pollEvent(event))
    {
        switch(event.type)
        {
            default:
                break;

            case sf::Event::Closed:
            {
                stateMachine->saveManager.save();
                this->stateMachine->renderWindow.close();
                break;
            }
            case sf::Event::Resized:
            {
                this->stateMachine->renderWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                resizeView(stateMachine->renderWindow, view);
                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    stateMachine->saveManager.save();
                    this->stateMachine->renderWindow.close();
                    return;
                }
                break;
            }

            case sf::Event::MouseButtonPressed: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(this->stateMachine->renderWindow);
                sf::Vector2f mouseCorrected = stateMachine->renderWindow.mapPixelToCoords(mousePos);
                for(sf::Text* f : framerates){
                    if (f->getGlobalBounds().contains(mouseCorrected)) {
                        chosenFrame = f->getString();
                    }
                }
                for(sf::Text* r : resolutions){
                    if (r->getGlobalBounds().contains(mouseCorrected)) {
                        chosenRes = r->getString();
                    }
                }
                if (fullscreen.getGlobalBounds().contains(mouseCorrected)) {
                    fullscreenInt *= -1;
                }
                if (applyButton.getGlobalBounds().contains(mouseCorrected)) {
                    apply();
                }

                if(backButton.getGlobalBounds().contains(mouseCorrected)){
                    loadMainMenuState();
                    return;
                }
                break;
            }

            case sf::Event::MouseMoved: {
                sf::Vector2i mousePos = sf::Mouse::getPosition(this->stateMachine->renderWindow);
                sf::Vector2f mouseCorrected = stateMachine->renderWindow.mapPixelToCoords(mousePos);
                for(sf::Text* t : options){
                    if (t->getGlobalBounds().contains(mouseCorrected)) {
                        t->setFillColor(sf::Color::Yellow);
                    } else {
                        t->setFillColor(sf::Color::Black);
                    }
                }
                break;

            }

        }

    }
}

void Config::loadMainMenuState()
{
    stateMachine->popState();
}

void Config::update(const float deltaTime)
{

}

void Config::apply() {
    std::string p = chosenRes;
    std::regex a(R"([0-9]?[0-9]?[0-9]?[0-9])");
    auto p_begin = std::sregex_iterator(p.begin(), p.end(), a);
    auto p_end = std::sregex_iterator();
    std::vector<int> vec;
    for (std::sregex_iterator i = p_begin; i != p_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        vec.push_back(std::stoi(match_str));
    }
    stateMachine->saveManager.m_screenWidth = vec[0];
    stateMachine->saveManager.m_screenHeight = vec[1];
    stateMachine->saveManager.m_framerate = std::stoi(chosenFrame);
    stateMachine->saveManager.m_fullscreen = fullscreenInt;

    if(stateMachine->saveManager.m_fullscreen == 1){
        stateMachine->renderWindow.create(sf::VideoMode(stateMachine->saveManager.m_screenWidth, stateMachine->saveManager.m_screenHeight), "Iceberg", sf::Style::Fullscreen);
    } else {
        stateMachine->renderWindow.create(sf::VideoMode(stateMachine->saveManager.m_screenWidth, stateMachine->saveManager.m_screenHeight), "Iceberg");
    }
    stateMachine->renderWindow.setFramerateLimit(stateMachine->saveManager.m_framerate);
    stateMachine->saveManager.save();
}

Config::~Config()
{
    for(auto t : options){
        delete(t);
        t = nullptr;
    }
}


