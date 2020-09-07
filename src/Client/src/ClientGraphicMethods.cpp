#include "./Client.hpp"

int Client::createWindow()
{
    window = new sf::RenderWindow(sf::VideoMode(1200, 600), "", sf::Style::Titlebar | sf::Style::Close);
    view = new sf::View();

    if (!window){
        return (0);
    }
    window->setFramerateLimit(0);
    return (1);
}

void Client::setCamera(Player *player)
{
    view->setCenter(*player->getPos());
    window->setView(*view);
}

void Client::getEvent()
{
    Player *player = world->getPlayer(0);

    if (!player){
        return;
    }

    for (; window->pollEvent(event);){
        switch(event.type){
            case sf::Event::Closed: {
                window->close();
                break;
            }
            case sf::Event::GainedFocus: {
                setFocus(true);
                break;
            }
            case sf::Event::LostFocus: {
                setFocus(false);
                break;
            }
            default:
                break;
        }
    }

    if (!getFocusState()){
        return;
    }

    DIRECTION direction = DIRECTION::UNDEFINED;
    int eventId = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        window->close();
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (player->getDirection() != DIRECTION::LEFT){
            player->setSpeed(0);
        }
        direction = DIRECTION::LEFT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (player->getDirection() != DIRECTION::RIGHT){
            player->setSpeed(0);
        }
        direction = DIRECTION::RIGHT;
    }

    if (direction == DIRECTION::UNDEFINED){
        if (player->getDirection() != DIRECTION::UNDEFINED){
            player->setSpeed(0);
            eventId = 2;
        }
    } else {
        eventId = 1;
    }

    if (eventId){
        SERVER_MESSAGES actionId;
        RakNet::BitStream out;

        switch (eventId){
            case 1: {
                player->setMoving(true);
                player->setDirection(direction);
                actionId = SERVER_MESSAGES::ID_ACTION_MOVE;
                player->addSpeed(0.50);
                if (player->getSpeed() > 16.0){
                    player->setSpeed(16.0);
                }
                break;
            }
            case 2: {
                player->setSpeed(0);
                player->setMoving(false);
                player->setDirection(DIRECTION::UNDEFINED);
                actionId = SERVER_MESSAGES::ID_ACTION_STOP_MOVE;
                break;
            }
        }
        out.Write((unsigned char) actionId);
        if (eventId == 1){
            out.Write((int) direction);
            out.Write((long) getSinceEpochMs());
        }
        mainPeerInterface->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverGuid, false);
        player->move();
        setCamera(player);
    }
}

void Client::startFrame()
{
    frameClock = std::chrono::high_resolution_clock::now();
}

void Client::endFrame()
{
    std::chrono::_V2::system_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> work_time = now - frameClock;

    if (work_time.count() < 50.0){

        long delta_ms_duration = 50-work_time.count();

        //std::cout << work_time.count() << " " << std::to_string(delta_ms_duration) << std::endl;

        if (delta_ms_duration > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration));
        }
    }
}