#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <Windows.h>
#include <thread>
#include <chrono>

using namespace std;



////////////////////////////////FIELD/////////////////////////////////////

class Field{
protected:
    static bool _isGameInitialized;
    vector<string> _gameField;
    static Field* _field;
    Field() {
        _gameField = {
            "    #    #     #       #      #         #           #          #     ",
            "   # #  # #   # #     # #    # #       # #         # #        # #    ",
            "  # # # # # # # # # # # # # # # # # ## # # # # # ## #  # ##  # # # # ",
            "#####################################################################",
            "#####################################################################",
            "#####################################################################",
            "#####################################################################",
            "---------------------------------------------------------------------",
        };
    }


public:
    static Field* getFieldInstance() {
        if(!_isGameInitialized) {
            _field = new Field();
            _isGameInitialized = true;
        }
            return _field;
    }

    vector<string> gameField() {
        return _gameField;
    }

    void renderField() const {
        for(size_t i = 0; i < _gameField.size(); ++i) {

        }
    }


    ~Field() {
        delete _field;
    }
};
bool Field::_isGameInitialized = false;
Field* Field::_field = nullptr;




////////////////////////////PLAYER/////////////////////////////////

class Player {
protected:
    static bool _isPlayerInitialized;
    vector <string> _hero;
    static Player* _player;
    Player() {
        _hero={
            " @",
            "/|\\",
            " |",
            "/ \\"
        };

        _pos = {6, 80};
    }

    pair<int, int> _pos; //Position of up-left corner of player matrix on


public:
    static Player* getPlayerInstance() {
        if(!_isPlayerInitialized) {
            _player = new Player();
            _isPlayerInitialized = true;
        }
        return _player;
    }

    vector<string> hero() {
        return _hero;
    }

    void setPos(int i, int j) {
        _pos.first = i; _pos.second = j;
    }

     pair<int, int> pos() {
        return _pos;
    }

    ~Player() {
        delete _player;
    }
};
bool Player::_isPlayerInitialized = false;
Player* Player::_player = nullptr;



//////////////////////////////SCENE//////////////////////////

class Scene {
public:
    static unordered_map<string,string> COLORS;

protected:
    Player* _player;
    Field* _field;
    static bool _isSceneInitialized;
    static Scene* _scene;
    vector<string> _gameScene;

    Scene(Player* player, Field* field) {
        _player = player;
        _field = field;
    }

public:
    static Scene* getSceneInstance(Player* player, Field* field) {
        if(!_isSceneInitialized) {
            _scene = new Scene(player, field);
            _isSceneInitialized = true;
        }
            return _scene;
    }

    void makeScene() {
        _gameScene = _field->gameField();
        pair<int, int> r_pos = _player->pos();
        vector<string> r_gameHero = _player->hero();

        if(r_pos.first < 0) {
            _player->setPos(0, r_pos.second);
            r_pos = make_pair(0, r_pos.second);
        }
        if(r_pos.first > _gameScene.size() - r_gameHero.size() + 1) {
            _player->setPos(_gameScene.size() - r_gameHero.size(), r_pos.second);
            r_pos = make_pair(_gameScene.size() - r_gameHero.size(), r_pos.second);
        }
        if(r_pos.second < 0) {
            _player->setPos(r_pos.first, 0);
            r_pos = make_pair(r_pos.first, 0);
        }
        if(r_pos.second > _gameScene[0].size() - r_gameHero[1].size()-1) {
            _player->setPos(r_pos.first, _gameScene[0].size() - r_gameHero[1].size()-1);
            r_pos = make_pair(r_pos.first, _gameScene[0].size() - r_gameHero[1].size()-1);
        }


        int ci = 0;
        for(int i = 0; i < _gameScene.size(); ++i) {
            int cj = 0;
            int j = 0;
            for(; j < _gameScene[i].size(); ++j) {
                if(cj < r_gameHero[ci].size() && ci < r_gameHero.size()) {
                    if(i >= r_pos.first) {
                        if(j >= r_pos.second && j < _gameScene[i].size() - r_gameHero[i].size()) {
                            if(cj == 0) {
                                _gameScene[i].insert(r_pos.second, COLORS["red"]);
                                j += COLORS["red"].size();
                            }

                            _gameScene[i][j] = r_gameHero[ci][cj];
                            ++cj;
                        }
                    }
                } else {
                    ++ci;
                    break;
                }
            }

            if(i >= 0 && i < 4) {
                _gameScene[i].insert(0, COLORS["blue"]);
                j += COLORS["blue"].size();
                _gameScene[i].insert(j, COLORS["blue"]);
            } else if(i >= 4 && i < 7) {
                _gameScene[i].insert(0, COLORS["green"]);
                j+= COLORS["green"].size();
                _gameScene[i].insert(j, COLORS["green"]);
            } else {
                _gameScene[i].insert(0, COLORS["white"]);
                j+=COLORS["white"].size();
                _gameScene[i].insert(j, COLORS["white"]);
            }

            _gameScene[i] += COLORS["end"];
        }
    }

    void renderScene() const {
        for(int i = 0; i < _gameScene.size(); ++i) {
            for(int j = 0; j < _gameScene[i].size(); ++j) {
                cout << _gameScene[i][j];
            }
            cout << endl;
        }
    }

    ~Scene() {
        delete _player;
        delete _field;
        delete _scene;
    }
};
bool Scene::_isSceneInitialized = false;
Scene* Scene::_scene = nullptr;
unordered_map<string, string> Scene::COLORS = {
    {"blue","\x1B[1;34;40m"},
    {"green","\x1B[1;32;40m"},
    {"red","\x1B[1;31;40m"},
    {"white", "\x1B[1;37;40m"},
    {"end","\x1B[0m"}
};


//////////////////////////////HeroActionsController//////////////////////




class HeroActionsController{

private:
    static HeroActionsController* _HAR;
    Player* _player;

    HeroActionsController(Player* player){
        _player = player;
    }

public:
    static HeroActionsController* getHeroActionsControllerInstance(Player* player){
        if(_HAR == nullptr){
            _HAR = new HeroActionsController(player);
        }
        return _HAR;
    }

    void moveLeft(){
        _player->setPos(_player->pos().first, _player->pos().second - 1);
    }

    void moveRight(){
        _player->setPos(_player->pos().first, _player->pos().second + 1);
    }
    void jump(int stageOfAnimation){
        if(stageOfAnimation == 0)
            _player->setPos(_player->pos().first-1, _player->pos().second);
        if(stageOfAnimation == 1)
            _player->setPos(_player->pos().first+1, _player->pos().second);

    }

};

HeroActionsController* HeroActionsController::_HAR = nullptr;






//////////////////////////////GAME///////////////////////////////////////

class Game {
protected:
    static Game* _game;
    static bool _isGameInitialized;
    Scene* _gameScene;
    Game(Scene* scene) {
        _gameScene = scene;
    }

public:
    static Game* getGameInstance(Scene* scene) {
        if(!_isGameInitialized) {
            _game = new Game(scene);
            _isGameInitialized = true;
        }
        return _game;
    }

    void render() {
        _gameScene->makeScene();
        _gameScene->renderScene();
    }

    ~Game() {
        delete _game;
        delete _gameScene;
    }
};

bool Game::_isGameInitialized = false;
Game* Game::_game = nullptr;


void GetKEY(bool KEY[256])
{
    int i = 0;
    while(i < 256){
        if(GetAsyncKeyState(i)) KEY[i] = 1; else KEY[i] = 0;
            i++;
    }
}




int main() {
    Game* game = Game::getGameInstance(Scene::getSceneInstance(Player::getPlayerInstance(), Field::getFieldInstance()));
    HeroActionsController* hac = HeroActionsController::getHeroActionsControllerInstance(Player::getPlayerInstance());


    int stageOfAnimation = 0;
    while(true){
        bool KEY[256];
        GetKEY(KEY);
        if(KEY[65])
        {
            hac->moveLeft();
        }
        if(KEY[87])
        {
            hac->jump(stageOfAnimation);
            ++stageOfAnimation;

            if(stageOfAnimation == 2) stageOfAnimation = 0;
        }
        if(KEY[68])
        {
            hac->moveRight();
        }


        
        game->render();
        this_thread::sleep_for(chrono::milliseconds(100));
        system("cls");
    }

    return 0;
}
