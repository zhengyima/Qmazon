#ifndef BOOK_H
#define BOOK_H

#include<string>
#include<map>
#include<iostream>
#include<vector>
using namespace std;

class Book {
public:
	Book() {
		cnt = 0;
		sum = 0;
		aveRank=0;
	}
	void deletebook();
	void addbook();
	void rechangebook(string s1, string s2);
	string title, author, publisher;  //标题、作者、出版社
	string year;                      //时间
	string urlS, urlM, urlL;          //三个信息
	double aveRank, sum;              //平均分、总分
	int cnt;                          //评论数量
									  //part3新增
	vector<string> comment;
	vector<string> commentauthor;
	string id;
	map<string, int> userRanked;
	void show() {
	//	if(title!=" "&&year!=" ")
		cout << title << " (" << year << ")" << endl;
	//	else
	//	cout<<id<<endl;
//		if(author!=" ")
		cout << author << endl;
	//	if(aveRank!=0)
		cout << "Average Rating: " << aveRank << endl;
	}
	map<string, int> usersRead;       //被哪些用户看过及评分
};
#endif // BOOK_H


#ifndef USER_H
#define USER_H

#include<string>
#include<map>
using namespace std;
class User {
public:
	User() {
		sum = 0;
		cnt = 0;
		password="iamauser";
	}
	void deleteuser();
	void addusers();
	void rechangeuser(string s1, string s2);
	string town, city, country;
	int age;
	int cnt;                      //评论数量
	string himself;
	string password;
	double aveRank, sum;
	map<string, int> booksRead;   //此用户看过的书名及评分
    //part3新增
	string isbn;
    Book findByName(string ti);
	void rank(int rank, string str);
//	string id;
	void get_init_reco();
    double get_sim(User const& u);
    map<string,User> sim_user;
    void add_comment(Book&, string comm);
	//结束
	void getrecommendation();
	void show(){
		cout<<himself<<";"<<town<<","<<city<<","<<country<<";"<<age<<endl;
	}
};
#endif // USER_H

//深复制问题
