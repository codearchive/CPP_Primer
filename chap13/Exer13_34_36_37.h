#ifndef MESSAGE_FOLDER_H
#define MESSAGE_FOLDER_H
#include <iostream>
#include <string>
#include <set>
using std::set;
using std::string;
class Folder; // incomplete class declaration
class Message {
	friend class Folder;
	friend void swap(Message&, Message&);
public:
	// folders is implicitly initialized to the empty set
	explicit Message(const string &str = "") : contents(str) {}
	// copy control to manage pointers to this Message
	Message(const Message&); // copy constructor
	Message& operator=(const Message&); // copy assignment 
	~Message(); // destructor
	// add/remove this Message from the specified Folder's set of messages
	void save(Folder&);
	void remove(Folder&);
	// insert or remove a given Folder* into folders, required by exercise 13.37
	void addFolder(Folder*);
	void remFolder(Folder*);
private:
	string contents; // actual message text
	set<Folder*> folders; // Folders that have this Message
	// utility functions used by copy constructor, copy-assignment operator and destructor
	// add this Message to the Folders that point to the parameter
	void add_to_Folders(const Message&);
	// remove this Message from every Folder in folders
	void remove_from_Folders();
};
class Folder {
	friend void swap(Folder&, Folder&);
public:
	explicit Folder(const string &s = "") : name(s) {}
	Folder(const Folder&);
	Folder& operator=(const Folder&);
	~Folder();
	void addMsg(Message*);
	void remMsg(Message*);
	void show();
private:
	string name;
	set<Message*> messages;
	void add_messages(const Folder&);
	void remove_messages();
};
void Folder::addMsg(Message *m)
{
	messages.insert(m);
	// m->folders.insert(this);
}
void Folder::remMsg(Message *m)
{
	messages.erase(m);
	// m->folders.erase(this);
}
void Folder::show()
{
	for(auto m : messages)
		std::cout << m->contents << std::endl;
}
void Message::save(Folder &f)
{
	folders.insert(&f); // add the given Folder to our list of Folders
	f.addMsg(this); // add this Message to f's set of Messages
}
void Message::remove(Folder &f)
{
	folders.erase(&f); // take the give Folder out of our list of Folders
	f.remMsg(this); // remove this Message to f's set of Messages
}
// add this Message to Folders that point to m
void Message::add_to_Folders(const Message &m)
{
	for(auto f : m.folders) // for each Folder that holds m
		f->addMsg(this); // add a pointer to this Message to that folders
}
// remove this message from the corresponding Folders
void Message::remove_from_Folders()
{
	for(auto f : folders) // for each pointer in folders
		f->remMsg(this); // remove this Message from that Folder
}
// copy constructor
Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
	add_to_Folders(m); // add this Message to the Folders that point to m
}
// destructor
Message::~Message()
{
	remove_from_Folders();
}
// copy-assignment operator
Message& Message::operator=(const Message& rhs)
{
	// handle self-assignment by removing pointers before inserting them
	remove_from_Folders(); // update existing Folders
	contents = rhs.contents; // copy message contents from rhs
	folders = rhs.folders; // copy Folder pointers from rhs
	add_to_Folders(rhs); // add this Message to those Folders
	return *this;
}
// swap function
void swap(Message &lhs, Message &rhs)
{
	using std::swap; // not strictly needed in this case, but good habit
	for(auto f : lhs.folders)
		f->remMsg(&lhs);
	for(auto f : rhs.folders)
		f->remMsg(&rhs);
	// swap the contents and Folder pointer sets
	swap(lhs.folders, rhs.folders); // use swap(set&, set&)
	swap(lhs.contents, rhs.contents); // swap(string&, string&)
	// add pointer to each Message to their (new) respective Folders
	for(auto f : lhs.folders)
		f->addMsg(&lhs);
	for(auto f : rhs.folders)
		f->remMsg(&rhs);
}
void Message::addFolder(Folder *f)
{
	folders.insert(f);
}
void Message::remFolder(Folder *f)
{
	folders.erase(f);
}
void Folder::add_messages(const Folder &f)
{
	for(auto m : f.messages)
		m->save(*this);
}
void Folder::remove_messages()
{
	for(auto m : messages)
		m->remove(*this);
	messages.clear(); // clear the messages because no message points to this folder
}
Folder::Folder(const Folder &f) : name(f.name), messages(f.messages)
{
	add_messages(f);
}
Folder& Folder::operator=(const Folder &rhs)
{
	remove_messages();
	name = rhs.name;
	messages = rhs.messages;
	add_messages(rhs);
	return *this;
}
Folder::~Folder()
{
	remove_messages();
}
void swap(Folder &lhs, Folder &rhs)
{
	using std::swap;
	for(auto m : lhs.messages)
		m->remove(lhs);
	for(auto m : rhs.messages)
		m->remove(rhs);
	swap(lhs.name, rhs.name);
	swap(lhs.messages, rhs.messages);
	for(auto m : lhs.messages)
		m->save(lhs);
	for(auto m : rhs.messages)
		m->save(rhs);
}
#endif