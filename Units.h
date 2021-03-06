//
// Created by Mike on 15.03.18.
//

#ifndef MYGAME_UNITS_H
#define MYGAME_UNITS_H

#include "Race.h"
#include "Map.h"
#include "Buildings.h"
using std::cin;
using std::min;
using std::max;
/**
 * \brief this class is for speaking with player
 */
class Writer {
public:
    /**
     * ask you to order something
     * @param movepoints current unit movepoints
     */
    static void wChoise(int movepoints) {
        cout << "Movepoints: " << movepoints << endl << endl;
        cout << "Listen to your orders!" << endl;
        cout << "1)Attack" << endl;
        cout << "2)Move" << endl;
        cout << "3)Nothing" << endl;
    }
    /**
     * ask you coordinates
     */
    static void wTakeCoordinates() {
        cout << "Say the coordinares, my Lord!" << endl;
    }
    /**
     * say about error
     */
    static void wWrongInfo() {
        cout << "Wrong Information!" << endl << endl;
    }
    /**
     * say that this unit has dead
     * @param UnitName unit's name
     */
    static void wDeath(string& UnitName) {
        cout << UnitName << " has dead." << endl;
    }
    static void wDamage(string& UnitName, int damage) {
        cout << UnitName << " " << "lost " << damage << " hp." << endl;
    }
    /**
     * say that your unit is healing
     * @param UnitName
     */
    static void wHealing(string& UnitName) {
        cout << UnitName << " is healing." << endl;
    }
    /**
     * ask you to order the way
     */
    static void wMoveInfoWarriorArcher() {
        cout << "Say the string of moving includes 'u','d','r','l': ";
    }
    /**
     * say about an error
     */
    static void wMistake() {
        cout << "Sorry, it was a mistake!" << endl;
    }
    /**
     * ask you to order the way
     */
    static void wMoveInfoSpy() {
        cout << "Say the strings of moving includes 'u','d','r','l' with '!' after the last: ";
    }
    /**
     * print 2 blank lines
     */
    static void wLine() {
        cout << endl << endl;
    }

};
class CUnit {
public:
    CUnit() = default;
    virtual void setIds(int &id) {
        return;
    }
    CUnit(IRace* r, Map* map) {}
    /**
     * your unit's moving
     * @param mp movepoints
     */
    virtual void Move(int &mp)=0;
    /**
     * hit the enemy
     * @param move way to enemy on (x,y)
     * @param x
     * @param y
     */
    virtual void Damage(int &move, int x, int y)=0;
    /**
     * count size of squad
     * @return size
     */
    virtual int getASize()=0;

    /**
     * count size of squad
     * @return size
     */
    virtual CUnit* get(int id) {
        return NULL;
    }
    /**
     *
     * @return last unit in squad
     */
    virtual CUnit* last() {
        return this;
    }
    virtual void setNull(int id) {}
    /**
     * count the damage to unit's health
     * if it bigger than health, unit will die
     * @param damage damage that your unit get from hit
     */
    void TakeDamage(int damage) {
        damage -= defence + _race->getBonusDefence() + defenceWeaponLevel * 5;
        if (damage <= 0)
            damage = 0;
        if (damage > health)
            damage = health;
        if (damage) health -= damage;
        Writer::wDamage(UnitName, damage);
        if (health <= 0)
            Death();
    }
    /**
     * say about the current unit
     */
    void getInfo() const{
        cout << UnitName << endl;
        cout << "Health: " << health << endl;
        cout << "Attack: " << attack + _race->getBonusAttack() + getAWL()*5;
        (attackDistance > 1) ? cout << " Attack Distance: " << attackDistance << endl:cout << endl;
        cout << "Defence: " << defence + _race->getBonusDefence() + getDWL()* powerWeaponLevel << endl;
    }
    /**
     *
     * @return unit's name
     */
    string getName() const { return UnitName; }
    /**
     * unit recieve orders from you
     * - move
     * - fight
     * - nothing
     */
    void takeOrders() {
        int movepoints = move + _race->getBonusMove();
        while (movepoints) {
            getInfo();
            Writer::wChoise(movepoints);
            char choice = '3';
            cin >> choice;
            if (choice == '1') {
                Writer::wTakeCoordinates();
                int cx, cy;
                cin >> cx >> cy;
                if ((*mask)[cx - 1][cy - 1])
                    Damage(movepoints, cx, cy);
                else {
                    Writer::wWrongInfo();
                    break;
                }
            }
            if (choice == '2')
                Move(movepoints);
            if (choice == '3') {
                findChurche(x - 2, y - 1);
                findChurche(x - 1, y - 2);
                findChurche(x - 1, y);
                findChurche(x, y - 1);
                break;
            }
            if (choice == '#') {
                Death();
                break;
            }
        }
    }
    /**
     * set place for unit
     * @param x
     * @param y
     */
    void setPlace(int x, int y) {
        this->x = x;
        this->y = y;
        if (!isEnemy)
          map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        if (!isEnemy)
          look();
    }
    /**
     * get unit's mark on text map
     * @return symbol
     */
    char getSymbol() const{ return symbol;}
    /**
     * get x-coordinate
     * @return x
     */
    int getX() const {return x; }
    /**
     * get y-coordinate
     * @return y
     */
    int getY() const {return y; }
    /**
     * heal unit if it possible
     */
    void healing(){
        if (health != maxhealth)
            Writer::wHealing(UnitName);
        health = min(maxhealth, health + _race->getBonusHealth());
    }
    /**
     *
     * @return health of unit
     */
    int getHealth() const {return health;}
    /**
     *
     * @return maxhealth of unit
     */
    int getMaxHealth() const {return maxhealth;}
    /**
     * set unit's army
     * @param a
     */
    void setArmy(vector<CUnit*> *a) {
        army = a;
    }
    void setId(int i) {
        id = i;
    }
    /**
     * set map with units
     * @param m
     */
    void setMask(vector<vector<CUnit*>> *m) {
        mask = m;
    }
    /**
     * set map with buildings
     * @param bm
     */
    void setBMask(vector<vector<IBuilding*>> *bm) {
        b_mask = bm;
    }
    /**
     *
     * @return weapon's attack level
     */
    int getAWL() const {return attackWeaponLevel;}
    /**
     *
     * @return weapon's defence level
     */
    int getDWL() const {return defenceWeaponLevel;}
    virtual ~CUnit() {}
    vector<vector<CUnit*>> *mask;
    vector<vector<IBuilding*>> *b_mask;
    int move;
    int x;
    int y;
    IRace* _race;

    vector<CUnit*> *army;

    string UnitName;

    int defence;
    int attack;
    int attackDistance = 1;
    int lookDistance;
    int health;
    int maxhealth;
    int attackWeaponLevel = 0;
    int defenceWeaponLevel = 0;

    char symbol;
    Map* map;
    bool isEnemy;
    int id;
    int powerWeaponLevel = 5;
    /**
     * Unit's death: deleting from maps
     */
    void Death() {
        Writer::wDeath(UnitName);
        (*army)[id] = NULL;
        (*mask)[x - 1][y - 1] = NULL;
        map->map[x - 1][y - 1] = '.';
        map->secret[x - 1][y - 1] = '*';
        delete this;
    };
    /**
     * check (tx,ty) church's effect's
     *
     * @param tx
     * @param ty
     */
    void churchEffect(int tx, int ty) {
        if ((*b_mask)[tx][ty]->getPurpose() == "Attack") {
            Builder *b = new Builder(this);
            b->buildAttack((*b_mask)[tx][ty]->getPower());
            delete b;
        }
        if ((*b_mask)[tx][ty]->getPurpose() == "Defence") {
            Builder *b = new Builder(this);
            b->buildDefence((*b_mask)[tx][ty]->getPower());
            delete b;
        }
    }
    /**
     * check is there a churche
     * @param x
     * @param y
     */
    void findChurche(int x, int y) {
        if (y < map->getSize() && y >= 0 && x < map->getSize() && x >=0 && map->map[x][y] == '+') {
            churchEffect(x, y);
        }
    }
    /**
     * change '*' to '.' in unit's area of visibility
     */
    void look(){
        int d = lookDistance + _race->getBonusLookDistance();
        int i0 = max(1, x - d);
        int j0 = max(1, y - d);
        int il = min(map->getSize(), x + d);
        int jl = min(map->getSize(), y + d);
        for (int i = i0; i <= il; i++)
            for (int j = j0; j <= jl; j++)
                if (map->map[i - 1][j - 1] == '*')
                    map->map[i - 1][j - 1] = map->secret[i - 1][j - 1] == '*' ? '.' : map->secret[i - 1][j - 1];
    }
    /**
     * change '.' to '*' in unit's area of visibility when he moves
     */
    void unlook() {
        int d = lookDistance + _race->getBonusLookDistance();
        int i0 = max(1, x - d);
        int j0 = max(1, y - d);
        int il = min(map->getSize(), x + d);
        int jl = min(map->getSize(), y + d);
        for (int i = i0; i <= il; i++)
            for (int j = j0; j <= jl; j++)
                if (map->map[i - 1][j - 1] == '.' || !(map->map[i - 1][j - 1] >= 'A' && map->map[i - 1][j - 1] <='Z'))
                    map->map[i - 1][j - 1] = '*';
        for (size_t i = 0; i < (*army).size();i++)
            if ((*army)[i] != this && (*army)[i])
                (*army)[i]->look();
    }

private:
    /**
     * \brief buffs attack and defence
     */
    class Builder{
    public:
        explicit Builder(CUnit* unit) {
            this->unit = unit;
        }
        void buildAttack(int x) {unit->attackWeaponLevel = x;}
        void buildDefence(int x) {unit->defenceWeaponLevel = x;}
    private:
        CUnit* unit;
    };
};
/**
 *
 * \brief Composite pattern.
 *
 */
class Army : public CUnit {
public:
    Army() {}

    void Move(int &mp) override {}

    int getASize() override {
        int ans = 0;

        for (size_t i = 0; i < army.size(); i++) {
            if (!army[i]) ans++;
            else ans += army[i]->getASize();
        }
        return ans;
    }
    /**
     * set ids for units
     * @param id first id
     */
    void setIds(int &id) override {
        for (size_t i = 0; i < army.size(); i++) {
            if (army[i]->getASize() == 1)
                army[i]->setId(id++);
            else
                army[i]->setIds(id);
        }
    }

    void Damage(int &move, int x, int y) {}
    /**
     * adds unit or squad
     * @param p
     */
    void addUnit(CUnit *p) {
        army.push_back(p);
    }

    CUnit *last() {
        if (army[army.size() - 1]->getASize() == 1) {
            return army[army.size() - 1];
        }
        return army[army.size() - 1]->last();
    }

    CUnit* get(int id) {
        id++;
        for (size_t i = 0; i < army.size(); i++) {
            if (army[i]->getASize() > id)
                return army[i]->get(id - 1);
            if (army[i]->getASize() < id)
                id -= army[i]->getASize();
            else return army[i]->last();
        }
        return NULL;
    }
    /**
     * set nulls by id in case of deaths
     * @param id
     */
    void setNull(int id) {
        id++;
        int t_size;
        for (size_t i = 0; i < army.size(); i++) {
            t_size = army[i] ? army[i]->getASize() : 1;
            if (t_size > id) {
                army[i]->setNull(id - 1);
                return;
            }
            if (t_size < id)
                id -= t_size;
            else if (t_size == 1) {
                army[i] = NULL;
                return;
            } else {
                army[i]->setNull(t_size - 1);
                return;
            }
        }
    }


    ~Army() {
        for (size_t i = 0; i < army.size(); ++i)
            delete army[i];
    }

    vector<CUnit *> army;

};

class Warrior : public CUnit {
public:
    Warrior() {}
    Warrior(IRace* race, Map* m, bool ie) {
        id = 2;
        _race = race;
        map = m;
        UnitName = _race->getNameWarrior();
        move = 6;
        defence = 20;
        attack = 250;
        lookDistance = 4;
        health = 600;
        maxhealth = 600;
        symbol = 'w';
        isEnemy = ie;
        if (!ie)
            symbol += 'A' - 'a';
    }
    int getASize() override {
        return 1;
    }
    void Move(int &mp) override {
        Writer::wMoveInfoWarriorArcher();
        string s;
        cin >> s;
        if (s.length() > (size_t)mp)
            s = s.substr(0, mp);
        unlook();
        map->map[x-1][y-1] = '*';
        map->secret[x-1][y-1] = '*';
        (*mask)[x-1][y-1] = NULL;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[i] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[i] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[i] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
        }
        map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        (*mask)[x-1][y-1] = this;
        look();
        map->showMap();
        mp = 0;
    }

    void Damage(int &move, int cx, int cy) override {
        Move(move);
        if (abs(x - cx) + abs(y - cy) == 1) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * powerWeaponLevel);
        }
        else {
            Writer::wMistake();
            TakeDamage(45);
        }
        map->showMap();
    }

};

class Archer : public CUnit {
public:
    Archer() {}
    Archer(IRace* race, Map* m, bool ie) {
        _race = race;
        map = m;
        UnitName = _race->getNameArcher();
        move = 6;
        defence = 10;
        attack = 120;
        lookDistance = 5;
        health = 400;
        symbol = 'a';
        isEnemy = ie;
        maxhealth = 400;
        attackDistance = 5;
        id = 1;
        if (!ie)
            symbol += 'A' - 'a';
    }
    int getASize() override {
        return 1;
    }
    void Move(int &mp) override {
        Writer::wMoveInfoWarriorArcher();
        string s;
        cin >> s;
        if (s.length() > (size_t)mp)
            s = s.substr(0, mp);
        unlook();
        map->map[x-1][y-1] = '*';
        map->secret[x-1][y-1] = '*';
        (*mask)[x-1][y-1] = NULL;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[i] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[i] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[i] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
        }
        map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        (*mask)[x-1][y-1] = this;
        look();
        map->showMap();
        mp = 0;
    }

    void Damage(int &move, int cx, int cy) override {
        if (move < 3) {
            Writer::wMistake();
            return;
        }

        if (abs(x - cx) + abs(y - cy) <= attackDistance + _race->getBonusArcherDistance()) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else {
            Writer::wMistake();
            TakeDamage(45);
        }
        move = max(0, move - 3);
        map->showMap();
    }


};

class Spy : public CUnit {
public:
    Spy() {}
    Spy(IRace* race, Map* m, bool ie) {
        _race = race;
        map = m;
        UnitName = _race->getNameSpy();
        move = 8;
        defence = 20;
        attack = 1090;
        lookDistance = 7;
        health = 300;
        maxhealth = 300;
        symbol = 's';
        isEnemy = ie;
        id = 0;
        if (!ie)
            symbol += 'A' - 'a';
    }
    int getASize() override {
        return 1;
    }
    void Move(int &mp) override {
        Writer::wMoveInfoSpy();
        int d = mp;
        map->map[x-1][y-1] = '*';
        string s="";
        int j = 0;
        for (int i = 0; i < d; i++) {
            map->map[x-1][y-1] = '*';
            map->secret[x-1][y-1] = '*';
            (*mask)[x-1][y-1] = NULL;
            unlook();
            mp--;
            if (j == (int)s.length()) {
                j = 0;
                cin >> s;
            }
            if (s[j] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[j] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[j] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[j] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
            map->map[x - 1][y - 1] = symbol;
            map->secret[x-1][y-1] = symbol;
            (*mask)[x-1][y-1] = this;
            look();
            if (s[j] == '!') {
                mp++;
                map->showMap();
                return;
            }
            j++;
            if (j == (int)s.length() || i == d - 1) {
                Writer::wLine();
                map->showMap();
            }
        }
    }

    void Damage(int &move, int cx, int cy) override {
        move -= 2;
        if (move > 0) {
            Move(move);
        }
        if (abs(x - cx) + abs(y - cy) == 1) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * 5);
        }
        else
            TakeDamage(45);
        map->showMap();
    }


};

class Hero : public CUnit {
public:
    Hero() {}
    Hero(IRace* race, Map* m, bool ie) {
        id = 3;
        _race = race;
        map = m;
        UnitName = _race->getNameWarrior();
        move = 16;
        defence = 250;
        attack = 1350;
        lookDistance = 5;
        health = 1600;
        maxhealth = 1600;
        symbol = 'h';
        isEnemy = ie;
        if (!ie)
            symbol += 'A' - 'a';
    }
    int getASize() override {
        return 1;
    }
    void Move(int &mp) override {
        Writer::wMoveInfoWarriorArcher();
        string s;
        cin >> s;
        if (s.length() > (size_t)mp)
            s = s.substr(0, mp);
        unlook();
        map->map[x-1][y-1] = '*';
        map->secret[x-1][y-1] = '*';
        (*mask)[x-1][y-1] = NULL;
        for (size_t i = 0; i < s.length(); i++) {
            if (s[i] == 'r' && y < map->getSize() && map->secret[x - 1][y] == '*')
                y++;
            if (s[i] == 'l' && y > 1 && map->secret[x - 1][y - 2] == '*')
                y--;
            if (s[i] == 'u' && x > 1 && map->secret[x - 2][y - 1] == '*')
                x--;
            if (s[i] == 'd' && x < map->getSize() && map->secret[x][y - 1] == '*')
                x++;
        }
        map->map[x-1][y-1] = symbol;
        map->secret[x-1][y-1] = symbol;
        (*mask)[x-1][y-1] = this;
        look();
        map->showMap();
        mp = 0;
    }

    void Damage(int &move, int cx, int cy) override {
        Move(move);
        if (abs(x - cx) + abs(y - cy) == 1) {
            (*mask)[cx - 1][cy - 1]->TakeDamage(attack + _race->getBonusAttack() + attackWeaponLevel * powerWeaponLevel);
        }
        else {
            Writer::wMistake();
            TakeDamage(45);
        }
        map->showMap();
    }

};

/**
 * \brief Unit's factory
 */
class Factory {
public:
    static CUnit* create(string type, IRace* race, Map* m, bool ie) {
        if (type == "Spy")
            return new Spy(race, m, ie);
        if (type == "Archer")
            return new Archer(race, m, ie);
        if (type == "Warrior")
            return new Warrior(race, m, ie);
        if (type == "Hero")
            return new Hero(race, m, ie);
        return NULL;
    }
};



#endif //MYGAME_UNITS_H
