#include <iostream>
#include "Elements.cc"

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
  cout << tempVec.at(0)->getSpriteID() << endl;

  if(dynamic_cast<DrawableElement*>(tempVec2.at(0)))
    cout << "dc: Player" << endl;
  else
    cout << "dc: Inte Player" << endl;


  return 0;

}
