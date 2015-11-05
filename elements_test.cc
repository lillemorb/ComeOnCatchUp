//Test av hur elements fungerar map på virtuella funktioner mm
#include <iostream>
#include "Elements.cc"
#include <vector>

class Class1
{
public:
  vector<DrawableElement*> sendVec()
    {
      vec.push_back(&p);
      vec.push_back(&d);
      vec.push_back(&p2);

      return vec;
    }
  ~Class1() = default;

private:
  Player p;
  Door d;
  Player p2;
  vector<DrawableElement*> vec{};
};

int main()
{
  DrawableElement* pPtr = new Player();
  
  cout << pPtr->tempFun() << endl;
  cout << pPtr->getSpriteID() << endl;  

  delete pPtr;
  pPtr = nullptr;

  Player p;
  DrawableElement* pPtr2= &p;
  cout << pPtr2->tempFun() << endl;
  cout << pPtr2->getSpriteID() << endl;  
  if(dynamic_cast<Player*>(pPtr2))
     cout << "dc: Det är en Player" << endl;
  else
    cout << "dc: Det är inte en Player" << endl;

  cout << "Adresser: " << endl
       << &p << " player" << endl
       << pPtr2 << " pPtr2" << endl;

  vector<DrawableElement*> tempVec{};
  tempVec.push_back(&p);
  cout << tempVec.at(0)->tempFun() << endl;
  cout << tempVec.at(0)->getSpriteID() << endl;

  if(dynamic_cast<Player*>(tempVec.at(0)))
    cout << "dc: Player" << endl;
  else
    cout << "dc: Inte Player" << endl;

  Door d;
  tempVec.push_back(&d);
  
  vector<DrawableElement*> tempVec2{tempVec};
  cout << tempVec2.at(0)->tempFun() << endl;
  cout << tempVec2.at(0)->getSpriteID() << endl;

  if(dynamic_cast<Player*>(tempVec2.at(0)))
    cout << "dc: Player" << endl << endl;
  else
    cout << "dc: Inte Player" << endl << endl;

  Class1 tempObj;
  vector<DrawableElement*> tempVec3{tempObj.sendVec()};
  cout << tempVec3.at(0)->tempFun() << endl;
  cout << tempVec3.at(0)->getSpriteID() << endl;

  if(dynamic_cast<DrawableElement*>(tempVec3.at(1)))
    cout << "dc: DrawableElement" << endl;

  if(dynamic_cast<Player*>(tempVec3.at(1)))
    cout << "dc: Player" << endl;
  else if(dynamic_cast<Door*>(tempVec3.at(1)))
    cout << "dc: Door" << endl;


  return 0;

}
