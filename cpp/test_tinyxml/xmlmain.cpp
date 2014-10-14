#include <string>
#include <iostream>
#include <iomanip>
/*
#ifndef TIXML_USE_STL
    #define TIXML_USE_STL
#endif*/
#include "tinyxml.h"

int main(){
	using namespace std;
	const char *filename = "e:/test.xml";

	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("persions");
	doc.LinkEndChild(root);

	TiXmlElement *persion = new TiXmlElement("persion");
	root->LinkEndChild(persion);

	TiXmlElement *name = new TiXmlElement("name");
	persion->LinkEndChild(name);

	TiXmlText *text = new TiXmlText("zsl");
	name->LinkEndChild(text);

	TiXmlElement *sex = new TiXmlElement("sex");
	persion->LinkEndChild(sex);
	text = new TiXmlText("male");
	sex->LinkEndChild(text);

	persion = new TiXmlElement("persion");
	root->LinkEndChild(persion);

	name = new TiXmlElement("name");
	persion->LinkEndChild(name);
	name->LinkEndChild(new TiXmlText("zal"));
	sex = new TiXmlElement("sex");
	persion->LinkEndChild(sex);
	sex->LinkEndChild(new TiXmlText("female"));


	doc.SaveFile(filename);

	doc.LoadFile(filename);


	if (doc.FirstChild("name"))
		cout<<"找到了\n";
	else cout<<"没有找到\n";

	TiXmlElement *pElem = doc.FirstChildElement();
	if (pElem == NULL)  return 0;

	cout<<pElem->Value()<<endl;
	pElem = pElem->FirstChildElement("persion");
	for (; pElem != NULL; pElem = pElem->NextSiblingElement()){
		cout<<"    persion\n";

		TiXmlElement *celem = pElem->FirstChildElement();
		if (celem==NULL){
		}
		else{
			for (; celem; celem = celem->NextSiblingElement()){
		        cout<<"        ";
				cout<<celem->Value()<<":"<<celem->GetText()<<endl;
			}
		}
	}

}